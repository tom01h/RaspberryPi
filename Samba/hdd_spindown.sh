#!/bin/sh
 
if [ $# -lt 2 ]; then
        echo "not enough arguments supplied."
        echo "hdd_spindown.sh [UUID] [INTERVAL]"
        exit 0
fi
 
disk=`findfs UUID="$1"`
interval=$2
 
count=$interval
up=1
 
if [ -z $disk ]; then
        #echo "findfs error"
        exit 1
fi
 
disk=`echo "$disk" |sed -e "s@/dev/@@"`
 
state=`grep " $disk " /proc/diskstats`
 
while [ true ]; do
        sleep 10
        count=$(($count-10))
        newstate=`grep " $disk " /proc/diskstats`
        if [ "$state" = "$newstate" ]; then
                if [ $count -lt 0 ]; then
                        count=$interval
                        if [ $up = 1 ]; then
                                #echo -e "spin-down"
                                sync
                                state=`grep " $disk " /proc/diskstats`
                                hdparm -y /dev/$disk > /dev/null 2>&1
                                up=0
                        fi
                fi
        else
                #echo -e "drive is up"
                count=$interval
                state="$newstate"
                up=1
        fi
done
