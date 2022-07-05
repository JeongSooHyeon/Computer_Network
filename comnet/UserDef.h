#define SOCK_ADDR "./sock_addr"
#define BUF_SIZE 256

void handler(int res){
    close(server);
    close(client);
    unlink(SOCKPATH); // 유닉스 소켓 파일 없애주기
    exit(1);
}