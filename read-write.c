#include "csapp.h"
#include "stdlib.h"

//读写模型  写的时候不能被其他的用户读  只要有人在读 就不能别的人进行写
int readcnt = 0;    /* Initially = 0  读的人的数量*/
sem_t mutex, w; /* Both initially = 1 mutex锁  写锁 */
int data = 1;
void reader(void){
    while(1){
        //readcnt是非原子性的操作  ，需要加锁
        P(&mutex);
        readcnt++;
        if (readcnt == 1) /* First in, 加写锁  别人不能进行写，只有所有人读完之后才能写   也成为读者优先锁 */
            P(&w);
        V(&mutex);

        /* Critical section */
        printf("reading : data的值时%d\n",data);
        /* Reading happens  */

        P(&mutex);
        readcnt--;
        if (readcnt == 0) /* Last out 关闭写锁  允许别人进行写操作*/
            V(&w);
        V(&mutex);
        sleep(1);
    }
}

void writer(int* _tmp){
    Pthread_detach(Pthread_self());
    while(1){
        //一只循环  监听请求
        P(&w);
        int tmp = *_tmp;
        /* Critical section */
        data += tmp;
        /* Writing happens  */
        printf("writing : data的值时%d\n",data);
        V(&w);
        sleep(1);
    }
}


int main(){
    sem_init(&w,0,1);
    sem_init(&mutex,0,1);
    //创建线程进行写
    int t[5] = {1,2,3,4,5};
    pthread_t w_tid[5];
    for (int i = 0; i <5; ++i) {
        Pthread_create(&(w_tid[i]),NULL,writer,(void*)(&t[i]));
    }
    pthread_t r_tid[5];
    for (int i = 0; i <5; ++i) {
        Pthread_create(&(r_tid[i]),NULL,reader,NULL);
    }
    sleep(100);
    exit(EXIT_SUCCESS);
}