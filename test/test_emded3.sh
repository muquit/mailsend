#!/bin/sh

#
# muquit@muquit.com Dec-27-2013
set -x

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

./mailsend -sub "Embed 2 images in HTML, one below the other" \
        -from $FROM -to $TO \
        -smtp smtp.gmail.com -port 587 -starttls -auth \
        -user $FROM -cs ISO-8859-1 \
        -embed-image "test/blue.png" \
        -embed-image "test/green.png"
