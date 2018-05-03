# Jéferson Bueno

while [ 1 ]; do

    echo "### Sistema Integrado ###"
    echo "Digite a opção desejada"
    echo "1 - Contabilidade"
    echo "2 - Vendas"
    echo "3 - Calendário"
    echo "4 - Sair"
    echo "*******************************"

    read -p "Digite a opcão desejada: " opcao

    case $opcao in
    1) 
        gcc contabilidade.c -o contabilidade
        chmod u+x contabilidade
        ./contabilidade
        ;;
    2)
        gcc vendas.c -o vendas
        chmod u+x vendas
        ./vendas
        ;;
    3)
        cal
        ;;
    4)
        echo "Obrigado por utilizar o Sistema Integrado"
        exit 0
        ;;
    *)
        echo "Opcão inválida"
    esac
done