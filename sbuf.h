//
// Created by lenovo on 2022/9/28.  手动实现sbuf（生产者-消费者模型）
//

#ifndef SERVER_SBUF_H
#define SERVER_SBUF_H
#include "csapp.h"
typedef struct sbuf{
    int *buf;    //存储数据
    int n;       //int max size
    int front;
    int rear;
    sem_t mutex; //lock
    sem_t slots; //available slots
    sem_t items; //available items
}sbuf_t;
void sbuf_init(sbuf_t *sbuf , int n);
void sbuf_deinit(sbuf_t* sbuf);
void sbuf_insert(sbuf_t *sbuf , int item);
int sbuf_remove(sbuf_t *sbuf);

#endif //SERVER_SBUF_H
