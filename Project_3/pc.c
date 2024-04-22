#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "eventbuf.h"

int events_per_producer;
int producers_count;
int consumers_count;
int max_outstanding_events;
int quitting_time = 0;
struct eventbuf *eb;


sem_t *mutex;
sem_t *items;
sem_t *spaces;

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}


void *producer(void *arg){

    (void)arg;

    int producer_id = *(int *)arg;
   // int event_number;

    for (int event_number = 0; event_number < events_per_producer; event_number++) {
        // wait to see if theres enough space
        sem_wait(spaces);

        // lock around eventbuf
        sem_wait(mutex);
        printf("P%d: adding event %d\n", producer_id, producer_id * 100 + event_number);

        // add event  to the event buf
        eventbuf_add(eb, producer_id * 100 + event_number);

        // unlock mutex
        sem_post(mutex);

        // signal to waiting consumer threads that event is ready 
        sem_post(items);
    }


    printf("P%d: exiting\n", producer_id);

    return NULL;
}


void *consumer(void *arg){
    int id = *(int *)arg;

    while(1) {
        // wait to see if any events to consume
        sem_wait(items);

        // lock around eventbuf
        sem_wait(mutex);

        if (eventbuf_empty(eb)){
            printf("C%d: buffer empty\n", id);
            sem_post(mutex);
            break;
        }

        // its not empty, get an event
        int event = eventbuf_get(eb);

        printf("C%d: got event %d\n", id, event);

        // unlock mutex
        sem_post(mutex);

        // post to indicate there is now free space
        sem_post(spaces);
    }


    printf("C%d: exiting\n", id);
    return NULL;

}



int main (int argc, char *argv[]){


    if (argc < 5) {
        printf("invalid number of arguments");
        return EXIT_FAILURE;
    }

    producers_count = atoi(argv[1]);
    consumers_count = atoi(argv[2]);
    events_per_producer = atoi(argv[3]);
    max_outstanding_events = atoi(argv[4]);
    
    // --- end parse ---


    mutex = sem_open_temp("pc-mutex", 1);
    items = sem_open_temp("pc-items", 0);
    spaces = sem_open_temp("pc-spaces", max_outstanding_events);
    
    //create even buffer
    eb = eventbuf_create();

    pthread_t producers[producers_count];
    pthread_t consumers[consumers_count];

    //start correct number of  producer threads
    for (int i = 0; i < producers_count; i++){
        int *prod_id_count = malloc(sizeof(int));
        *prod_id_count = i;
        pthread_create(&producers[i], NULL, producer, prod_id_count);
    }

    //start correct number of consumer threads
    for (int i = 0; i < consumers_count; i++){
        int *consumer_id_count = malloc(sizeof(int));
        *consumer_id_count = i;
        pthread_create(&consumers[i], NULL, consumer, consumer_id_count);
    }


    // wait for all producer threads to complete
    for (int i = 0; i < producers_count; i++){
        pthread_join(producers[i], NULL);
    }
    
    //notify all consumers threads they are done
    for (int i = 0; i < consumers_count; i++){
        sem_post(items);
    }    

    quitting_time = 1;

    //wait for all consumer threads to complete
    for (int i = 0; i < consumers_count; i++){
        pthread_join(consumers[i], NULL);
    }

    // free event buffer
    eventbuf_free(eb);

    sem_close(mutex);
    sem_close(items);
    sem_close(spaces);

    sem_unlink("pc-mutex");
    sem_unlink("pc-items");
    sem_unlink("pc-spaces");

    return 0;

}