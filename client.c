#include"csapp.h"

int main(int argc , char **argv) {
    char* p = (char*)"format ./myhostinfo www.website.com";
    if(argc != 3)
        unix_error(p);
    char* hostName = argv[1];
    char* port     = argv[2];

    int clientfd = Open_clientfd(hostName , atoi(argv[2]));
    char buf[MAXLINE];
    char* tmp = NULL;
    //缓存绑定服务
    rio_t rio;
    Rio_readinitb(&rio,clientfd);

    while((tmp = Fgets(buf , MAXLINE , stdin)) != NULL){
        //手动输入数据写入到fd中  就进行发送
        Rio_writen(rio.rio_fd,buf,strlen(buf));
        //从fd中读取一行数据，然后打印  第二次 从fd中读取一行数据然后打印
        Rio_readlineb(&rio,buf,MAXLINE);
        printf("%s",buf);
    }
    exit(EXIT_SUCCESS);
}
