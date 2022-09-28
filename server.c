#include "csapp.h"
#include "stdio.h"

void echo(int connfd){
    //RIO健壮包
    rio_t rio;
    //缓冲区
    char buf[MAXLINE];
    char rtn[MAXLINE] = "拉屎去吧\n";
    //RIO与fd描述符绑定
    Rio_readinitb(&rio,connfd);
    //先从fd中读取buf中
    int n;
    while((n = Rio_readlineb(&rio,buf,MAXLINE)) > 0){
        //打印输出
        printf("buf : %s",buf);
        //从buf读取到fd中  ->  流回写  返回信息
        Rio_writen(rio.rio_fd , rtn , strlen(rtn));
        printf("rio -> buf : %s",rio.rio_buf);
    }
}

int main(int argc , char** argv){
    //开始初始化
    if(argc != 2)
        unix_error("wrong format");
    //监听端口
    int listenfd = Open_listenfd(atoi(argv[1]));

    while(1){
        //创建addrinfo
        struct addrinfo clientAddr;
        //设置addrlen
        socklen_t addrlen = sizeof(clientAddr);
        //主机名称
        char hostName[MAXLINE];
        //内存设置为0
        memset(&clientAddr,0,addrlen);
        //conn连接描述符号   监听端口listenfd   将用户信息放在client中
        int connfd = Accept(listenfd,&clientAddr,&addrlen);

        //获取名称hostName
        getnameinfo(&clientAddr,addrlen,hostName,MAXLINE,NULL,0,0);
        printf("receive from %s\n",hostName);
        //开始响应
        echo(connfd);
        //关闭连接
        close(connfd);
    }

    exit(EXIT_FAILURE);
}