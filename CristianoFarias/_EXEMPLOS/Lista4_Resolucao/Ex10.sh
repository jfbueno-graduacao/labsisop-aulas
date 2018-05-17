#!/bin/bash
echo -e "Digite 1 para visualizar a memória livre e 2 para visualizar a memória ocupada:";
read opcao;
echo -e "O programa para exibição do status da RAM está sendo executado...";
gcc -Wall Ex10.c -o Ex10;
./Ex10 $opcao;
          
