#!/bin/bash

DIR=$MON_DIR

if [ ! -d "$DIR" ]; then    
    echo $DIR não existe ou não é um diretório 2> /dev/stderr
    exit 1
else
    echo $DIR é um diretório válido
    exit 0
fi