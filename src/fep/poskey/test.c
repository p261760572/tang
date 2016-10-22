nclude <stdio.h>;
#include <stdlib.h>;
#include <sys/errno.h>;
#include <pthread.h>;
#include <unistd.h>;

extern "C" void * Thread_handler(void *);

int main(void)
{
pthread_t thread1 = 0 ;
int RetCode;

RetCode=pthread_create(&thread1, NULL,Thread_handler, NULL);

printf("RetCode=%d, tid=%d\n",RetCode,thread1);
sleep(2);
exit(0);
}

void * Thread_handler(void * arg)
{
printf("Hello kid!\n";
return NULL;
}

