#!/bin/bash
MON_DIR=/home/ccf/Desktop
CHECK=0
while [ $CHECK == 0 ]; do
if [ -e "$MON_DIR" ]
then
ls $MON_DIR
sleep 2
else
echo "$MON_DIR Não Existe ou não é um diretório" >/dev/stderr
exit 1
fi
done
