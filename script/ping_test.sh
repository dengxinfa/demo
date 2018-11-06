#! /bin/sh
  
count=0  
number=0
SAVE_NAME=/ping.out
LOC_TIME=0;

echo "system restart" >> $SAVE_NAME

sleep 30
while  [ 1 ] 
do  
   ping -c 10000 baidu.com |grep loss >>$SAVE_NAME
   if [ $? -eq 0 ]
   then
	let number=number+1
   	echo  "$number" >>$SAVE_NAME
   	date +%m-%d\ %H:%M:%S >>$SAVE_NAME
	echo "#####################" >>$SAVE_NAME
   fi
done  
