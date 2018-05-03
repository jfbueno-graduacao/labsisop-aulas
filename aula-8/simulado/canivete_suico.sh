echo "### Canivete Suico ###"

echo "1 - Calculadora"
echo "2 - Texto"
echo "3 - Operacoes"
echo "4 - Processo"
echo "5 - Sair"

read -p "Digite a opcão desejada: " opcao

case $opcao in
1) 
    gcc calculadora.c -o calculadora
    chmod u+x calculadora
    ./calculadora
    ;;
2)
    gcc texto.c -o texto

    read -p "Digite o parametro para enviar ao programa: " par

    chmod u+x texto
    ./texto $par
    ;;
3)
    gcc operacoes.c -o operacoes
    chmod u+x operacoes

    read -p "Digite os tres comandos para serem executados: " a b c

    ./operacoes $a $b $c

    echo "Muito obrigado! Hoje é " $(date)

    ;;
4)
    gcc processo.c -o processo
    chmod u+x processo
    
    read -p "Digite a URL: " url

    ./processo $url
    ;;
5)
    echo "Finalizando"
    exit 0
    ;;
*)
    echo "Opcão inválida"
esac