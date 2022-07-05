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
#include <time.h> 
#include <arpa/inet.h>

#define BUF_SIZE 256
#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010



int main(int argc, char* argv[]) {
	int sock;
    struct sockaddr_in server;
    char buf[BUF_SIZE] = {0, };

    FILE* fp;

    int Flag = -1;

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

    
    
    while(1)
    {       
        memset(buf,0,BUF_SIZE);
        if(recv(sock, buf, BUF_SIZE, 0) != -1)
        {
            printf("%s", buf);
            memset(buf, 0, BUF_SIZE);
        }

        scanf("%s", buf);
        printf("\n");

        if(strncmp(buf, "quit", strlen("quit")) == 0){
            break;
        }

        if(buf[0]-'0' == 1){
            memset(buf, 0, BUF_SIZE);
            send(sock, "\\service 1", BUF_SIZE, 0);
            recv(sock, buf, BUF_SIZE, 0);
            printf ("Current local time and date : %s\n", buf);
            memset(buf, 0, BUF_SIZE);
        }

        else if(buf[0]-'0' == 2){
            memset(buf, 0, BUF_SIZE);
            send(sock, "\\service 2", BUF_SIZE, 0);

            recv(sock, buf, BUF_SIZE, 0); // fileList
            printf ("%s", buf);
            memset(buf, 0, BUF_SIZE); 

            scanf("%s", buf); // 1 or 2 send
            printf("\n");
            send(sock, buf, BUF_SIZE, 0);


            if(buf[0]-'0' == 1){
                fp = fopen("/home/kali/Desktop/MultiService/client/Book.txt", "w");              
            }
            else if(buf[0]-'0' == 2){
                fp = fopen("/home/kali/Desktop/MultiService/client/HallymUniv.jpg", "wb");     
            }
            else{
                Flag = 1;
            }
            if(Flag == 1){
                Flag = -1;
                continue;
            }
            int nbyte = BUF_SIZE;
            while(nbyte >= BUF_SIZE){
                    nbyte = recv(sock, buf, BUF_SIZE, 0);
                    fwrite(buf, sizeof(char), nbyte, fp);
            }
            printf("downloaded\n");
            fclose(fp);           
        }

        else if(buf[0]-'0' == 3){
            memset(buf, 0, BUF_SIZE);
            send(sock, "3", BUF_SIZE, 0);
            while(1) {
                printf("Message : ");
                scanf("%s", buf);
                printf("\n");

                if(send(sock, buf, BUF_SIZE, 0) == -1){
                    perror("Send error\n");
                    exit(1);
                }
                if(strncmp(buf, "quit", strlen("quit")) == 0)
                    break;
                memset(buf,0,BUF_SIZE);

                recv(sock, buf, BUF_SIZE, 0);
                printf("[You] %s", buf);
                memset(buf, 0, BUF_SIZE);
            }
        }
        memset(buf,0,BUF_SIZE);
    }
	close(sock);

    return 0;
}