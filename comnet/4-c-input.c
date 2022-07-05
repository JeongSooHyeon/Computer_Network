//클라의 클라이언트
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_ADDR "./sock_addr_c"
#define BUF_SIZE 256

int main(int argc, char* argv[]) {
	int sock, len;
    struct sockaddr_un sock_addr;
    char buf[BUF_SIZE] = {0, };

    sock_addr.sun_family = AF_UNIX;
    strcpy(sock_addr.sun_path, SOCK_ADDR);

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock == -1)
        perror("Socket Error\n");
    
    if(connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1){
        perror("Connet Error\n");
        exit(1);
    }
    else
        printf("[Info] Unix socket : connected to the server\n");

    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    while(1)
    {
        printf("> Enter: ");
        fgets(buf, BUF_SIZE, stdin);
        int len = strlen(buf);
        if(buf[len-1] == '\n'){
            buf[len - 1] = '\0';
            len--;
        }

        if(send(sock, buf, BUF_SIZE, 0) == -1){
            perror("Send error\n");
            exit(1);
        }

        if(strncmp(buf, "quit", strlen("quit")) == 0){
            printf("Terminate...\n");
            sleep(1);
            break;
        }
        memset(buf, 0, BUF_SIZE);
    }


    recv(sock, buf, BUF_SIZE, 0);
    if(strncmp(buf, "ok", strlen("ok"))){
            printf(": Success\n");
    }
    
    printf("[Info] Closing socket\n");
	close(sock);

    return 0;
}