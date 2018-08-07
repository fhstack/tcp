#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

//tcp_server ip port
//多进程
void serviceIO(int sock)
{
    
    char buf[128];
    while(1)
    {
        ssize_t s = read(sock,buf,sizeof(buf)-1);
        if(s > 0)
        {
            buf[s] = 0;
            printf("client: %s\n",buf);
            write(sock,buf,strlen(buf));
        }
        else if(s == 0)//连接关闭
        {
            printf("quit!\n");
            break;
        }
        else
        {
            perror("read");
            break;
        }
    }
    close(sock);
}

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("Usage:%s is ip port",argv[0]);
        return 1;
    }
    
    int listen_sock = socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock < 0)
    {
        perror("listen_socket");
        return 2;
    }
    printf("listen_socket:%d\n",listen_sock);

    //填充本地信息
    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(atoi(argv[2]));//端口号
    local.sin_addr.s_addr = inet_addr(argv[1]);//IP地址

    if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
        perror("bind");
        return 3;
    }

    if(listen(listen_sock,5) < 0)
    {
        perror("listen");
        return 4;
    }

    while(1)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);   //阻塞的
        if(new_sock < 0)
        {
            perror("accept");
            continue;
        }
         
        printf("get new link![%s:%d]\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

        pid_t id = fork();
        if(id == 0)    //child
        {
            if(fork() > 0)
            {
                exit(0);
            }
            close(listen_sock);
            serviceIO(new_sock);
            exit(0);
        }
        else           //father
        {       
            //close(new_sock);
            waitpid(id,NULL,0);         //绝对不会阻塞了，因为子进程立即退出了，父进程立马可以回收它
        }
    }

}
