#include "csapp.h"
#include "stdio.h"
#include "stdlib.h"

//信号量  初始化为 0
sem_t task1IdDone,task2IsDone;

void* foo1(){
    printf("task1 id done\n");
    V(&task1IdDone); // 第一步： 1 0
    //1执行完成
    //等待2执行完成
    P(&task2IsDone); //第四部：1 1
    printf("task3 id done\n");
    return NULL;
}

void* foo2(){
    //等1执行完成之后执行2
    P(&task1IdDone);// 第二步：0 0
    printf("task2 id done\n");
    V(&task2IsDone);//第三部： 0 1
    //2执行完成
    return NULL;
}

int main(){
    //创建爱你两个线程执行 进行123的顺序
    pthread_t tid1;
    pthread_t tid2;
    Pthread_create(&tid1,NULL,&foo1,NULL);
    Pthread_create(&tid2,NULL,&foo2,NULL);

    //主线程等待这两个线程执行完成之后然后再退出程序
    Pthread_join(tid1,NULL);
    Pthread_join(tid2,NULL);
    printf("hello");


    exit(0);
}