#!/bin/bash
MON_DIR=/home/ccf/Desktop
CHECK=0
COMANDO=$1
CONTEUDO="";
CONT=0
CONTBACK=0;
TEMP="";
while [ $CHECK == 0 ]; do
	if [ -e "$MON_DIR" ]; then		
		TEMP=$(ls $MON_DIR)
		if [ $CONT == 0 ]; then
			CONTEUDO=$TEMP
			CONT=$((CONT+1))
		elif [ "$TEMP" != "$CONTEUDO" ]; then
			$COMANDO $2 $3 $4 &
			CONTBACK=$((CONTBACK+1))
			if [ $CONTBACK == 5 ]; then
				exit 5
			fi
		fi
		CONTEUDO=$TEMP
		sleep 2
		else
			echo "$MON_DIR Não Existe ou não é um diretório" >/dev/stderr
			exit 1
	fi
done
