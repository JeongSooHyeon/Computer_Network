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
#include <arpa/inet.h>

#define BUF_SIZE 256
#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010

int main(int argc, char* argv[]) {
	int sock;
    struct sockaddr_in server;
    char buf[BUF_SIZE] = {0, };

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(TIME_SERVER);
    server.sin_port = htons(TIME_PORT);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        perror("Socket Error\n");

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
        perror("Connet Error\n");
        exit(1);
    }

    while(1){
        printf("Enter message: ");
        fgets(buf, BUF_SIZE, stdin);
        if(send(sock, buf, BUF_SIZE, 0) == -1){
            perror("send");
            exit(1);
        }
        memset(buf,0,BUF_SIZE);
        
        if(recv(sock, buf, strlen(buf), 0) == -1){
            perror("recv");
            exit(1);
        }
        printf("Recv from server:\n%s\n", buf);
   
        memset(buf,0,BUF_SIZE);
    }
	close(sock);

    return 0;
}