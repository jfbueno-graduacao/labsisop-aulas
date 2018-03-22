#!/bin/bash

DIR=$MON_DIR
COM_PARAM=$1

if [ ! -d "$DIR" ]; then
    echo $DIR não existe ou não é um diretório 2> /dev/stderr
else    
    ST_INIT=$(ls $DIR -l)
    while true; do
        ST_ACT=$(ls $DIR -l)
        if [ "$ST_INIT" != "$ST_ACT" ]; then
            bash -c $* &
            ST_INIT=$ST_ACT
        fi
        sleep 2
    done
fi