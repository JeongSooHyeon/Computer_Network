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

#define BUF_SIZE 256
#define TIME_SERVER "127.0.0.1"
#define TIME_PORT 5010


int main(int argc, char* argv[])
{
	int server, client, len;
	struct sockaddr_in server_addr;
    struct sockaddr client_addr; 
	char buf[BUF_SIZE] = {0, };
    char serviceList[BUF_SIZE];
    char fileList[BUF_SIZE];
    strcpy(serviceList, "[Service List]\n1. Get Current Time\n2. Download File\n3. Echo Server\nEnter: ");
    strcpy(fileList, "[Available File List]\n1. Book.txt\n2. HallymUniv.jpg\n3. Go back\nEnter: ");
    
    FILE* fp;

    time_t rawtime;
    struct tm * timeinfo;

    int Flag = -1;

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
    client = accept(server, (struct sockaddr *)&client_addr, &len);
    

    while(1) 
    {
        memset(buf,0,BUF_SIZE);  
        if(client != -1){
            
            send(client, serviceList, BUF_SIZE, 0);
        }
        if(recv(client, buf, BUF_SIZE, 0) == -1)
        {
            perror("Recv Error\n");
            exit(1);
        }
        if(strncmp(buf, "quit", strlen("quit")) == 0){
            break;
        }

        if(strncmp(buf, "\\service 1", strlen("\\service 1")) == 0){
            printf("service 1\n");
            memset(buf,0,BUF_SIZE);
            time (&rawtime);
            timeinfo = localtime (&rawtime);
            strcpy(buf, asctime(timeinfo));
            send(client, buf, BUF_SIZE, 0);
            
        }
     
        else if(strncmp(buf, "\\service 2", strlen("\\service 2")) == 0){
            printf("service 2\n");
            memset(buf,0,BUF_SIZE);
            send(client, fileList, BUF_SIZE, 0); // fileList

            recv(client, buf, BUF_SIZE, 0);
            if(buf[0]-'0' == 1)
                fp = fopen("Book.txt", "r");                 
            else if(buf[0]-'0' == 2)
                fp = fopen("HallymUniv.jpg", "rb");  
            else
                Flag = 1;
                
            if(Flag == 1){
                Flag = -1;
                continue;
            }
            size_t fsize;
            fseek(fp, 0, SEEK_END);
            fsize = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            int nsize = 0;
            while(nsize != fsize){
                int fpsize = fread(buf, 1, BUF_SIZE, fp);
                nsize += fpsize;
                send(client, buf, fpsize, 0);
                usleep(1000);
            }             
        }

        else if(strncmp(buf, "3", strlen("3")) == 0){
            memset(buf,0,BUF_SIZE);    
            while(1){
                if(recv(client, buf, BUF_SIZE, 0) == -1)
                {
                    perror("Recv Error\n");
                    exit(1);
                }
                if(strncmp(buf, "quit", strlen("quit")) == 0)
                    break;
                send(client, buf, BUF_SIZE, 0);               
                
                memset(buf,0,BUF_SIZE);
            }           

        }
        memset(buf,0,BUF_SIZE);        
    }
	
    close(server);
    close(client);
}