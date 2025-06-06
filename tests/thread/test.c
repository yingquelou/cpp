#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
void *fun(void *tmp) {
  int i = 0;
  for (; i < 10; ++i) {
    printf("pthread %#llx keep %d,iter %d.\n", pthread_self(), *(int *)tmp, i);
  }
  return NULL;
}
int main(void) {
  pthread_t th1, th2;
  _Atomic int a = 0, b = 1;
  pthread_create(&th2, NULL, fun, &b);
  pthread_create(&th1, NULL, fun, &b);
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  return 0;
}