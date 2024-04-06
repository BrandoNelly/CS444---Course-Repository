#include <stdio.h>
#include <pthread.h>


void *run(void *arg)
{

    int *thread_id = (int *)arg;

    for(int i = 0; i < 5; i ++){
        printf("thread %d : %d\n", *thread_id, i);
    }

    return NULL;
}

int main()
{
    pthread_t threads[2];
    int thread_id[2] = {1, 2};
    int status;

    puts("Launching Threads");

    for (int i = 0; i < 2; i++){
        status = pthread_create(&threads[i], NULL, run, (void*)&thread_id[i]);
    }

    if (status == -1)
        perror("pthread_create");


    for (int i = 0; i < 2; i++){
        status = pthread_join(threads[i], NULL);
    }


    if (status == -1)
        perror("pthread_join");

    puts("Threads Complete");
}