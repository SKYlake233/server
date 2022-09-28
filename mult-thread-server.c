#include "csapp.h"
#include "sbuf.h"
#define NTHREADS 4
#define SBUFSIZE 16

static int byte_cnt; /* Byte counter */
static sem_t mutex;  /* and the mutex that protects it */

sbuf_t sbuf;
void *thread(void *vargp);
void echo_cnt(int connfd);

int main(int argc, char **argv){
    int listenfd,connfd;
    struct addrinfo clientaddr;
    socklen_t clientlen;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    //开始监听
    listenfd = Open_listenfd(atoi(argv[1]));

    //创建线程然后不断接受请求，然后返回
    sbuf_init(&sbuf, SBUFSIZE);
    for (int i = 0; i < NTHREADS; i++)  /* Create worker threads */
        Pthread_create(&tid, NULL, thread, NULL);

    while (1) {
        clientlen = sizeof(struct addrinfo);
        connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
        sbuf_insert(&sbuf, connfd); /* Insert connfd in buffer */
    }
}

void *thread(void *vargp)
{
    Pthread_detach(pthread_self());
    while (1) {
        int connfd = sbuf_remove(&sbuf); /* Remove connfd from buffer */
        echo_cnt(connfd);                /* Service client */
        Close(connfd);
    }
}

static void init_echo_cnt(void)
{
    Sem_init(&mutex, 0, 1);
    byte_cnt = 0;
}

void echo_cnt(int connfd)
{
    int n;
    char buf[MAXLINE];
    rio_t rio;
    static pthread_once_t once = PTHREAD_ONCE_INIT;

    //多线程中函数执行一次
    Pthread_once(&once, init_echo_cnt);

    Rio_readinitb(&rio, connfd);

    while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        P(&mutex);
        byte_cnt += n;
        printf("server received %d (%d total) bytes on fd %d\n",
               n, byte_cnt, connfd);
        V(&mutex);
        Rio_writen(connfd, buf, n);
    }
}