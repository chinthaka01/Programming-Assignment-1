# Programming-Assignment-1

## Description:

### Implements the Producer-Consumer Problem using:

    POSIX threads
    POSIX semaphores
    Shared memory

## Design Overview:

**Resource |	Type |	Purpose**  

/table_shm |	POSIX shared memory |	Shared buffer for items  
/mutex_sem |	POSIX semaphore |	Protect shared access (mutual exclusion)  
/empty_sem |	POSIX semaphore |	Counts empty slots in buffer  
/full_sem |	POSIX semaphore |	Counts full slots in buffer

    Table capacity: 2

## Shared Memory Structure:

#define BUFFER_SIZE 2

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_table;

    in – index for next produced item
    out – index for next consumed item

## Build:

    gcc producer.c -pthread -lrt -o producer  
    gcc consumer.c -pthread -lrt -o consumer

## Run:

    ./producer & ./consumer &

## Expected Output:

    Produced item 1 at index 0  
    Produced item 2 at index 1  
    Consumed item 1 from index 0  
    Produced item 3 at index 0  
    Consumed item 2 from index 1
    ...

## Notes:

    This design uses shared memory to represent the buffer.

    Semaphores ensure proper synchronization.

    Threads simulate continuous production and consumption.

    Mutual exclusion guarantees only one thread modifies the buffer at a time.
