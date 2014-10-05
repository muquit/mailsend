#!/bin/sh
#
#
# muquit@muquit.com Dec-27-2013
TO=$TO
if [ x"$TO" = x ]; then
    echo "TO environment variable is not not"
    exit 1
fi
FROM=$FROM
if [ x"$FROM" = x ]; then
    echo "FROM environment variable is not not"
    exit 1
fi

pass=$SMTP_USER_PASS
if [ x"$pass" = x ]; then
    echo "SMTP_USER_PASS environment variable is not not"
    exit 1
fi

./mailsend -v -sub "Embed images in a HTML table. Uses explicit Conteny-Type header" \
        -from $FROM -to $TO \
        -smtp smtp.gmail.com -port 587 -starttls -auth \
        -user $FROM -cs ISO-8859-1 \
        -content-type "multipart/related" \
        -mime-type text/html \
        -disposition inline -enc-type "none" \
        -attach "test/embed_image2.html" \
        -mime-type "image/png" -enc-type "base64" \
        -disposition inline \
        -content-id "img1_deadbeefcafe" -cs "none" \
        -attach "test/blue.png" \
        -content-id "img2_deadbeefcafe" \
        -attach "test/green.png"
