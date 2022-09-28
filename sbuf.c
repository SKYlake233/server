#include "sbuf.h"

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