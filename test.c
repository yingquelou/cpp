#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
void *fun(void *tmp)
{
    int i = 0;
    for (; i < 10; ++i)
        printf("this is %d.\n", *(int *)tmp);
}
int main(void)
{
    pthread_t th1, th2;
    int a = 0, b = 1;
    pthread_create(&th1, NULL, fun, &a);
    pthread_create(&th2, NULL, fun, &b);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}