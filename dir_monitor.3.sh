DIR=$1
COM_PARAM=$2

if [ ! -d "$DIR" ]; then
    >&2 echo $DIR não existe ou não é um diretório
else
    while true; do
        ls $DIR
        sleep 2
    done
fi