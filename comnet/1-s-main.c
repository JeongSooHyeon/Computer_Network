//서버의 서버
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
#define SOCK_ADDR "./sock_addr"
#define BUF_SIZE 256

int main(int argc, char* argv[])
{
	int server, server_in, s_client, c_client, len, sc, cc;
	struct sockaddr_un server_addr;
    struct sockaddr_in server_addr_in;
    struct sockaddr client_addr, client_addr_in;
	char buf[BUF_SIZE] = {0, };

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_ADDR);

    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_addr.s_addr = INADDR_ANY;
    server_addr_in.sin_port = htons(TIME_PORT);

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
    
    s_client = accept(server, &client_addr, &len); // 서버의 클라이언트
    if(s_client == -1){
        printf("Accept Error\n");
        exit(1);
    }        
    else
        printf("[Info] Unix socket : client connected\n");
    
    server_in = socket(AF_INET, SOCK_STREAM, 0);
    if(server_in == -1)
    {
        perror("Server Error\n");
        exit(1);
    }  
    if (bind(server_in, (struct sockaddr*)&server_addr_in,  sizeof(server_addr_in)) == -1){
        perror("bind");
		exit(1);
    }

    if (listen(server_in, 5) == -1){
        perror("listen");
		exit(1);
    }
    else
        printf("[Info] Inet socket : wating for conn req\n");    

    c_client = accept(server_in, &client_addr_in, &len); // 클라의 클라이언트
    if(c_client == -1){
        printf("Accept Error\n");
        exit(1);
    }        
    else
        printf("[Info] Inet socket : client connected\n");
    
    int flags = fcntl(s_client, F_GETFL, 0);
    fcntl(s_client, F_SETFL, flags | O_NONBLOCK);
    int flags1 = fcntl(c_client, F_GETFL, 0);
    fcntl(c_client, F_SETFL, flags1 | O_NONBLOCK);

    int q = 1;
    while(1) 
    {   
        if(recv(s_client, buf, BUF_SIZE, 0) != -1){ // 내 클라한테 받아
            printf("[ME] : %s\n", buf);
            send(c_client, buf, BUF_SIZE, 0); // 큰 클라한테 줘
            if(strncmp(buf, "quit", strlen("quit")) == 0){
                printf("[Server] quit\n");
                break;
            }
        }
        else if(q && recv(c_client, buf, BUF_SIZE, 0) != -1){ // 큰 클라한테 받아
            printf("[YOU] : %s\n",buf);
            if(strncmp(buf, "quit", strlen("quit")) == 0)
                q = 0;        
        }        
        memset(buf,0,BUF_SIZE);
    }
    
	send(s_client, "ok", BUF_SIZE, 0);

    printf("[Infor] Closing sockets\n");
    close(server);
    close(s_client);
}