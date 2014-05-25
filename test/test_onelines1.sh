#!/bin/sh

# test online messages with -M
# Issue# 45
# muquit@muquit.com Feb-01-2014 
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

./mailsend -v -sub "testing oneline messages no encoding type" \
        -from $FROM -to $TO \
        -smtp smtp.gmail.com -port 587 -starttls -auth \
        -user $FROM \
        -M "Three one line messages including this one" \
        -M "one line message 1" \
        -M "one line message 2"
