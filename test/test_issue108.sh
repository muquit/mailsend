#!/bin/sh

# muquit@muquit.com Jan-21-2013  
#set -x
ME=`basename $0`
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

if [ x"$Cc" != x ]; then
    CC="-cc $Cc"
fi

if [ x"$bcc" != x ]; then
    bcc="-bc ${bcc}"
fi
echo "BCc: $bcc"

pass=$SMTP_USER_PASS
if [ x"$pass" = x ]; then
    echo "SMTP_USER_PASS environment variable is not not"
    exit 1
fi

#SMTP=smtp.comcast.net
SMTP=smtp.gmail.com

BINARY="./mailsend"
echo "$OS"
if [ x"$OS" = x"Windows_NT" ]; then
    BINARY="./mailsend.exe"
fi
os=`uname`
os="foo"
if [ x"$os" = x"Linux" ]; then
VALGRIND_LOG=/tmp/valgrind.log
VALGRIND="valgrind -v --tool=memcheck --leak-check=yes --error-limit=yes --log-file=$VALGRIND_LOG"
/bin/rm -f $VALGRIND_LOG
else
VALGRIND=""
fi
set -x

$VALGRIND $BINARY -to $TO ${CC} ${bcc} -from $FROM \
 -v \
 -starttls -port 587 -auth \
 -smtp $SMTP \
 -cs "utf-8" \
 -sub "testing issue 108" \
 -user "$FROM" -pass "$PASS" \
 -cs "utf-8" \
 -mime-type "text/plain" \
 -enc-type "base64" \
 -content-disposition "inline" \
 -M "one line attachment base64 encoding" \
 -mime-type "application/pdf" \
 -attach "/home/muquit/junk/test.pdf" 
