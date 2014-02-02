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

$VALGRIND $BINARY -to $TO -from $FROM \
 -v \
 -starttls -port 587 -auth \
 -smtp $SMTP \
 -cs "utf-8" \
 -H "X-Priority: 1" -H "Importance: high" \
 -sub "testing mailsend" +cc +bc \
 -user "$FROM" -pass "$PASS" \
 -enc-type "none" \
 -mime-type "text/plain" \
 -M "one line attachment 1" \
 -enc-type "none" \
 -mime-type "text/plain" \
 -M "one line attachment 2" \
 -mime-type "text/html" \
 -enc-type "none" \
 -M "<b>this is a test</b><br>" \
 -cs "utf-8" \
 -mime-type "text/plain" \
 -enc-type "base64" \
 -M "Ich lerne seit ungefähr zwei Jahren Deutsch" \
 -cs "iso-8859-1" \
 -enc-type "8bit" \
 -mime-type "text/plain" \
 -content-disposition "inline" \
 -attach "test/test.txt" \
 -attach "test/test.gif" \
 -cs "Big5" \
 -enc-type "base64" \
 -mime-type "text/plain" \
 -M "中文測試"
