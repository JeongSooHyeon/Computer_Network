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
#include "UserDef.h"

main(int argc, char* argv[])
{
	int server, client, len;
	struct sockaddr_un server_addr;
    struct sockaddr client_addr;
	char buf[BUF_SIZE] = {0, };

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
    client = accept(server, &client_addr, &len);

    while(1) 
    {
        if(recv(client, buf, BUF_SIZE, 0) == -1)
        {
            perror("Recv Error\n");
            exit(1);
        }
        printf("Server recv message :$s\n",buf);

        if(strncmp(buf, "quit", strlen("quit"))){
            break;
        }
        memset(buf,0,BUF_SIZE);
    }
	
    close(server);
    close(client);
}