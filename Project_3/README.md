# Brandon Nelson -- Project 3: Producer/Consumer Threads

## Building

Command line:

* `make` to build. An executable called `pc` will be produced.

## Files


* `pc.c`: The main file of the program.
* `eventbuf.c`: A file containing the definition of for an event buffer structure for adding and receiving events and it's accompanying utility methods.
* `eventbuf.h`: Header file to eventbuf.c
* `Makefile`: used to compile the program to obtain executable.

## Data

* Integers represented as:
    producer threads count,
    consumer threads count,
    outstanding (produced and not yet consumed) events count,
    A flag for controlling when it is time to quit all work and join the remaining threads


* Semaphore types for controlling the acquiring and releasing of mutex locks and signaling the state of data between threads
    sem_t *mutex
    sem_t *items
    sem_t *spaces

* An "event buffer" structure for temporarily storing events added by the producer threads and a point for receiving the events by the consumer threads.



## Functions

* `main()`Executes the main function of the program. Handles command line parsing. 
  * `sem_open_temp()`: A function used for creating semaphores and listens for the sem_unlink() function to close the semaphore.
  * `*producer()`: called in the main function by pthread_create() to start up a producer thread. Produces events and signals to the consumer sem_wait(items) 
  * `*consumer*`: called in the main function by pthread_create() to start up a consumer thread. Consumes events that are passed in by producer threads and signals the resultant empty spaces.

## Notes

* On the command line, following the name of the executable, enter the amounts in order of: # Producer threads, # Consumer threads, How many events produced by producer threads, The maximum of outstanding events.

e.g. ./pc 2 4 5 2