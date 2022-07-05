//클라이언트의 서버
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

#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010
#define SOCK_ADDR "./sock_addr_c"
#define BUF_SIZE 256

int main(int argc, char* argv[])
{
	int server, client, len, main_s, ss, cc;
	struct sockaddr_un server_addr;
    struct sockaddr_in server_addr_in;
    struct sockaddr client_addr;
	char buf[BUF_SIZE] = {0, };

    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_addr.s_addr = inet_addr(TIME_SERVER);
    server_addr_in.sin_port = htons(TIME_PORT);

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_ADDR);

    server = socket(AF_UNIX, SOCK_STREAM, 0);
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
    else
        printf("[Info] Unix socket : wating for conn req\n");

    client = accept(server, &client_addr, &len); // 클라이언트
    if(client == -1){
        printf("Accept Error\n");
        exit(1);
    }        
    else
        printf("[Info] Unix socket : client connected\n");
    
    main_s = socket(AF_INET, SOCK_STREAM, 0);
    if(main_s == -1)
        perror("Socket Error\n");
    
    if(connect(main_s, (struct sockaddr*)&server_addr_in, sizeof(server_addr_in)) == -1){
        perror("Connect Error\n");
        exit(1);
    }
    else
        printf("[Info] Inet socket : connected to the server\n");
    
    int flags = fcntl(client, F_GETFL, 0);
    fcntl(client, F_SETFL, flags | O_NONBLOCK);

    int flags1 = fcntl(main_s, F_GETFL, 0);
    fcntl(main_s, F_SETFL, flags1 | O_NONBLOCK);


    int q = 1;
    while(1) 
    {
        if(recv(client, buf, BUF_SIZE, 0) != -1){ // 내 클라한테 받아
            printf("[ME] : %s\n",buf);
            send(main_s, buf, BUF_SIZE, 0); // 서버에 보내
            if(strncmp(buf, "quit", strlen("quit")) == 0){
                printf("[Server] quit\n");
                break;
            }
        }
        else if(q && recv(main_s, buf, BUF_SIZE, 0) != -1){ // 서버한테 받아
            printf("[YOU] : %s\n",buf);
            if(strncmp(buf, "quit", strlen("quit")) == 0)
                q = 0;            
        }  
        memset(buf,0,BUF_SIZE);
    }
    
	send(client, "ok", BUF_SIZE, 0);

    printf("[Infor] Closing sockets\n");
    close(server);
    close(client);
}