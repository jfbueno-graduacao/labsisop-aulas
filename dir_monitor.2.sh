#!/bin/bash

DIR=$1

if [ ! -d "$DIR" ]; then
    >&2 echo $DIR não existe ou não é um diretório
else
    while true; do
        ls $DIR -B
        sleep 2
    done
fi