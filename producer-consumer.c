#include "csapp.h"
#include "stdlib.h"

//生产者-消费者模型  生产者可以向缓冲区中增加物品 消费者可以从缓冲区中拿走物品  同一时间只能增加物品或者拿走物品  一个时间只能一个用户拿走物品
int i = 0;

typedef struct{
    int *buf;    //存储数据
    int n;       //int max size
    int front;
    int rear;
    sem_t mutex; //lock
    sem_t slots; //available slots
    sem_t items; //available items
}sbuf_t;

void sbuf_init(sbuf_t *sbuf , int n){
    sbuf -> buf = Calloc(n , sizeof(int));
    sbuf -> n = n;
    sbuf -> front = sbuf -> rear = 0;
    Sem_init(&sbuf -> mutex , 0 , 1);
    Sem_init(&sbuf -> slots , 0 , n);
    Sem_init(&sbuf -> items , 0 , 0);
}

void sbuf_deinit(sbuf_t* sbuf){
    Free(sbuf -> buf);
}
//  P  如果非0 减一 然后立刻返回 如果等于0 就挂起线程  知道非0

//将item插入槽中
void sbuf_insert(sbuf_t *sbuf , int item){
    //插入的时候 先看有没有空位
    P(&sbuf ->slots);
    //进行加锁
    P(&sbuf -> mutex);
    //插入item
    sbuf->buf[(++sbuf -> rear) % sbuf -> n] = item;
    //解锁
    V(&sbuf -> mutex);
    //增加item的数量
    V(&sbuf -> items);
}

//移除槽的最后一个
int sbuf_remove(sbuf_t *sbuf){

    int item;
    printf("执行了第%d次remove函数\n",++i);
    //如果有物品的话
    P(&sbuf -> items);
    //解锁
    P(&sbuf -> mutex);
    //进行操作
    item = sbuf -> buf[(++sbuf -> front) % sbuf -> n];
    //加锁
    V(&sbuf -> mutex);
    //增加空位
    V(&sbuf -> slots);
    return item;
}
int main(){
    sbuf_t sbuf;
    sbuf_init(&sbuf,4);
    //先remove  然后进行插入
    int rtv;
    pthread_t tid;
    Pthread_create(&tid,NULL,sbuf_remove,(void*)&sbuf);
    sbuf_insert(&sbuf , 2);
    Pthread_join(tid,&rtv);
    printf("%d",rtv);
    sbuf_deinit(&sbuf);
    exit(EXIT_SUCCESS);
}