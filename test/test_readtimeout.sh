#!/bin/sh

# test read timeout, use -startls and specify port 465
# Issue #61
# muquit@muquit.com Aug-17-2014 
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

N=3
echo "mailsend should timeout after $N seconds"
sleep 1
# It should timeout after N seconds
./mailsend -v -sub "testing read timeout" \
        -from $FROM -to $TO \
        -smtp smtp.gmail.com -port 465 -starttls -auth \
        -user $FROM \
        -read-timeout $N \
        -M "This mail will never be sent"
echo "Exit code: $?"
