#!/bin/sh
#
# create a gzip compressed tar archive of the current working directory.
# the name of the file will be dir.tar.gz
#

. ./VERSION

pwd=`pwd`
pwd_head=`basename $pwd`

#
# first check if we'r at the corrent directory
#

if [ $pwd_head != $SLL_VERSION ]; then
    echo "You are in wrong base directory"
    echo "Expected: $SLL_VERSION"
    echo "Found:    $pwd_head"
    echo "exiting ..."
    exit 1
fi


tar_filename=$pwd_head.tar
#
# remove the tar file if any in the current working directoyr
#
if [ -f ./$tar_filename.gz ]; then
    echo "removing $tar_filename.gz"
    rm -f ./$tar_filename.gz
fi

make clean > /dev/null 2>&1
#
# go up
#

cd ..
tar -cf /tmp/$tar_filename ./$pwd_head
if [ $? -eq 0 ]; then
    gzip -f -v -9 /tmp/$tar_filename
    mv /tmp/$tar_filename.gz ./$pwd_head
    cd $pwd
    ls -l $tar_filename.gz
fi
