#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>
#include "filas.h"
 
int main(int argc, char const *argv[])
{
    while(1){
        writeFila();
    }

    return 0;
}
