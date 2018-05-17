#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  int param = atoi(argv[1]);
  if(param == 1){
    printf("Memória livre: \n");
    system("free -g | awk '{print $3}'");
  }
  else if(param == 2){
    printf("Memória usada: \n");
    system("free -g | awk '{print $2}'");
  }
  else{
    printf("Parâmetro inválido! \n");
  }
  return 0;
}
