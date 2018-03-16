#!/bin/bash

DIR=$MON_DIR

if [ ! -d "$DIR" ]; then    
    >&2 echo $DIR não existe ou não é um diretório
fi