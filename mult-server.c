#include "csapp.h"
#include "stdlib.h"

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

void *thread(void *vargp){
    int connfd = *((int*)vargp);
    //线程脱离
    Pthread_detach(pthread_self());
    Free(vargp);
    echo(connfd);
    Close(connfd);
    return NULL;
}

int main(int argc, char **argv){
    int listenfd,*connfdp;
    pthread_t pid;
    struct addrinfo clientaddr;
    socklen_t clientlen;
    memset(&clientaddr,0,sizeof(clientaddr));


    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    listenfd = Open_listenfd(atoi(argv[1]));
    //不断监听请求
    while(1){
    clientlen = sizeof(clientaddr);
    connfdp = Malloc(sizeof(int));
    *connfdp = Accept(listenfd,&clientaddr,(SA *) &clientlen);
    Pthread_create(&pid, NULL, thread, connfdp);
}

    return 0;
}