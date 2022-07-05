//서버
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

struct ChatInfo {
    struct ChatRoom room; // 채팅방에 관한 정보 저장
    struct UserSockets new_users; // 채팅방에 새로 참여하는 사용자들
    struct UserSockets returned_users; // 채팅방에서 탈퇴한 사용자
};
struct ChatRoom {
    int ID; // 0,1,2
    char title[BUF_SIZE]; // 방제목
}
struct UserSockets {
    int max_users = 5;
    
}

#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010
#define SOCK_ADDR "./sock_addr"
#define BUF_SIZE 256

int main(int argc, char* argv[])
{
	int server, client, len;
    struct sockaddr_in server_addr_in; // inet
    struct sockaddr client_addr;
	char buf[BUF_SIZE] = {0, };

    pthread_t p_thread[3];
    int thr_id;
    

    signal(SIGINT, handler);


    // 3개의 채팅방 관리 스레드 생성
    for(int i = 0; i < 3; i++){
        thr_id = pthread_create(p_thread[0], NULL, create_func, 매개변수);
        chatinfo[i].room.ID = i;
    }
    void create_func(ChatInfo chatinfo){
        int i = 0;
        while(true){
            // 새로운 채팅방 참가자 있는지 확인
            if(chatinfo[i].new_users.){
                chatinfo[i].room
            }

            // 참여자 중 send한 사람이 있는지 확인 select
        }
    }




    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SOCK_ADDR);

    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_addr.s_addr = INADDR_ANY;
    server_addr_in.sin_port = htons(TIME_PORT);

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
    else
        printf("[Info] Unix socket : wating for conn req\n"); 

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

static void handler(int sig){
    close(server);
    close(client);
    exit(EXIT_SUCCESS);
}

