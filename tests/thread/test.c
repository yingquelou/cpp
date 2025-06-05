#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
void *fun(void *tmp)
{
    int i = 0;
    for (; i < 10; ++i)
    {pthread_self();
        printf("pthread %#x keep %d,iter %d.\n", pthread_self(),*(int *)tmp,i);
    }
}
int main(void)
{
    pthread_t th1, th2;
    _Atomic int a = 0, b = 1;
    pthread_create(&th2, NULL, fun, &b);
    pthread_create(&th1, NULL, fun, &b);
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}