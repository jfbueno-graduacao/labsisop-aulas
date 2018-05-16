//Cristiano Farias - Laboratorio de Sistemas - Lista4

#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {
if(argc <= 2){
printf("Passagem de parametros inválida!\n");
}
else{
printf("MEMORIA USADA: %s%%\n",argv[1]);
printf("MEMORIA LIVRE: %s%%\n",argv[2]);
}
}
