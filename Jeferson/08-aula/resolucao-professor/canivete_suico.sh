#!/bin/bash

while [ 1 ]; do
	clear
	
	echo "Digite a opção desejada:"
	echo "1 - Calculadora"
	echo "2 - Texto"
	echo "3 - Operações"
	echo "4 - Processo"
	echo "5 - Sair"
	read opcao 

	case "$opcao" in
		1 )
			./calculadora
		;;
		2 )
			echo "Digite uma palavra:"
			read palavra 
			./texto $palavra
		;;
		3 )
			echo "Digite os 3 comandos: "
			read argumento
			./operacao $argumento
		;;
		4 )
			echo "Digite a URL: "
			read url
			./processo $url
		;;
		5 )
			exit 0
	esac  
	sleep 3
done
