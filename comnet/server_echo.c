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
#include <signal.h>

#define BUF_SIZE 256
#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010
#define EXIT_SUCCESS 0

static void handler(int);
int server, client, len;

int main(int argc, char* argv[])
{
	
	struct sockaddr_in server_addr;
    struct sockaddr client_addr; 
	char buf[BUF_SIZE] = {0, };
    int client_cnt = 0; // 접속한 클라 수
    int accp_sock[3];

    signal(SIGINT, handler);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(TIME_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;    

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server == -1)
    {
        perror("Server Error\n");
        exit(1);
    }

	if (bind(server, (struct sockaddr*)&server_addr,  sizeof(server_addr)) == -1){
        perror("bind");
		exit(1);
    }

	if (listen(server, 5) == -1){
        perror("listen");
		exit(1);
    }
    

    while(1) 
    {
        if(client_cnt < 3){
        if((client = accept(server, (struct sockaddr *)&client_addr, &len)) == -1){
            perror("accept fail");
            exit(1);
        }
        client_cnt++;
        printf("New Client !\n");
        printf("Number of service client : %d\n", client_cnt);
        }
        if(fork() == 0){
            // 자식 프로세스
            close(server);

            while(1){
                // 받기
                if(recv(client, buf, BUF_SIZE, 0) == -1){
                    perror("recv");
                    exit(1);
                }
                printf("Recv from client: %s\n", buf);

                // 되돌려주기
                if(send(client, buf, BUF_SIZE, 0) == -1){
                    perror("send");
                    exit(1);
                }
            
                memset(buf,0,BUF_SIZE);   
            }
            close(client);
            exit(0);
        }
        close(client);          
    }   
    close(server);    
    return 0;
}
static void handler(int sig) {
    close(server);
    close(client);
    exit(EXIT_SUCCESS);
}