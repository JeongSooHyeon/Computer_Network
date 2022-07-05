#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define EXIT_SUCCESS 0

static void handler(int);

int main(){

    signal(SIGINT, handler);
    
    printf("Sleep begins!\n");
    sleep(1000);
    printf("Wake up\n");
}
static void handler(int sig) {
    printf("Handler is called.\n");
    exit(EXIT_SUCCESS);
}