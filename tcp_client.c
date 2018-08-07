#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
//tcp_client ip port

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s is server_ip server_port",argv[0]);
        return 1;
    }
    
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)
    {
        perror("socket");
        return 2;
    }
    printf("socket:%d\n",sock);

    //填充本地信息
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));//端口号
    server.sin_addr.s_addr = inet_addr(argv[1]);//IP地址

    if(connect(sock, (struct sockaddr*)&server,sizeof(server)) < 0)
    {
        perror("connect");
        return 3;
    }

    char buf[128];
    while(1)
    {
        printf("Please Enter:");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf)-1);
        if(s > 0)
        {
            buf[s-1] = 0;
            write(sock,buf,strlen(buf));
            read(sock,buf,sizeof(buf)-1);
            printf("server echo: %s\n",buf);
        }
    }
    
}
