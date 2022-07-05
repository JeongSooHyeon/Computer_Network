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
#define N_CLIENT 3

static int maxArr(int[], int);
static void handler(int);
int server, client, len;

int main(int argc, char* argv[])
{
	int ret, i;
	struct sockaddr_in server_addr;
    struct sockaddr client_addr; 
	char buf[BUF_SIZE] = {0, };
    int accp_sock[N_CLIENT];
    fd_set readfds;

    signal(SIGINT, handler);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(TIME_PORT);

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

    for(i = 0; i < N_CLIENT; i++){
        if((accp_sock[i] = accept(server, (struct sockaddr *)&client_addr, &len)) == -1){
            perror("accept");
            break;
        } // 새로운 소켓 기술자 리턴
        else printf("Client #%d connected.\n", i+1);
    }    

    while(1) 
    {
        FD_ZERO(&readfds);
        for(i = 0; i < N_CLIENT; i++)
            FD_SET(accp_sock[i], &readfds);
        printf("wating at select...\n");
        ret = select(maxArr(accp_sock, N_CLIENT)+1, &readfds, NULL, NULL, NULL);
        printf("select returned: %d\n", ret);
        switch(ret){
            case -1:
                perror("select err");break;
            case 0:
                printf("select returns: 0\n"); break;
            default:
                i = 0;
                while(ret > 0){
                    if(FD_ISSET(accp_sock[i], &readfds)){
                        memset(buf,0,BUF_SIZE); 
                        if(recv(accp_sock[i], buf, BUF_SIZE, 0) == -1){
                            perror("recv");
                            break;
                        }
                        ret--;
                        printf("MSG from client %d: %s\n", i, buf);
                    } else;
                    i++;
                }
                break;
        }
               
    }
	
    close(server);
    close(client);
    return 0;
}
static int maxArr(int arr[], int n){
    int max = 0;
    for(int j = 0; j < n; j++)
        if(max < arr[j])
            max = arr[j];
    return max;
}
static void handler(int sig){
    close(server);
    close(client);
    exit(EXIT_SUCCESS);
}