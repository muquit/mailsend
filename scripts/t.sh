#!/bin/sh
# test
to="muquit@gmail.com"
from="muquit@muquit.com"
mailsend="./mailsend"
date=`date`
#$mailsend -v -d localhost -t $to +cc +bc -sub test \
# -from $from -a nf.jpg,image/jpeg,a -sub test
$mailsend -d localhost -t $to +cc +bc \
 -from $from -sub "test-$date" ${1+"$@"}


