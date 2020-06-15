#!/bin/sh

HOME=/www/cgi-bin

cd $HOME

SHM_ID=`cat ./shm_id`

echo "kill prev shm_id:" $SHM_ID

ipcrm -m $SHM_ID

./parseServer &
