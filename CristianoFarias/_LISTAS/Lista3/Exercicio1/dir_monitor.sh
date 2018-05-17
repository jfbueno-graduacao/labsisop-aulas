#!/bin/bash
MON_DIR=/home/ccf/Desktops
if [ -e "$MON_DIR" ]
then
echo " O diretorio existe"
else
echo "$MON_DIR Não Existe ou não é um diretório" >/dev/stderr
exit 1
fi
