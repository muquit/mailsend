#!/bin/sh

# muquit@muquit.com Jan-21-2013  
#set -x
ME=`basename $0`
NARG=$#
if [ $NARG -ne 2 ]; then
    echo "usage: $ME <TO> <FROM>"
    echo "Please specify password with SMTP_USER_PASS env var"
    exit 1
fi
TO="$1"
YOU="$2"

if [ x"$SMTP_USER_PASS" = x ]; then
    echo "Please specify password with SMTP_USER_PASS env var"
    exit 1
fi
PASS=$SMTP_USER_PASS
echo "To: $TO"
echo "From: $YOU"

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
VALGRIND="valgrind -v --leak-check=no --log-file=$VALGRIND_LOG"
/bin/rm -f $VALGRIND_LOG
else
VALGRIND=""
fi
set -x

$VALGRIND $BINARY -to $TO -from $YOU \
 -v \
 -starttls -port 587 -auth \
 -smtp $SMTP \
 -cs "utf-8" \
 -H "X-Priority: 1" -H "Importance: high" \
 -sub test +cc +bc \
 -user $YOU -pass $PASS \
 -enc "none" \
 -M "one line attachment 1" \
 -enc "none" \
 -M "one line attachment 2" \
 -mime-type "text/html" \
 -enc "none" \
 -M "<b>this is a test</b><br>" \
 -separator ":" \
 -attach "test.txt:text/plain:i:mittagpausa.txt:none:none" \
 -separator "," \
 -attach "test.gif,image/gif,i,test.gif,none,base64" \
 -separator ":" \
 -attach "test.gif:image/gif:a:bar.gif:none:base64" \
 -show_attach
