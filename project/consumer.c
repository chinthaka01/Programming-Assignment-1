#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define SHM_NAME "/table_shm"
#define BUFFER_SIZE 2

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_table;

void* consumer_thread(void* arg) {
    shared_table* table = (shared_table*) arg;
    sem_t* mutex = sem_open("/mutex_sem", 0);
    sem_t* empty = sem_open("/empty_sem", 0);
    sem_t* full  = sem_open("/full_sem", 0);

    while (1) {
        sem_wait(full);
        sem_wait(mutex);

        int item = table->buffer[table->out];
        printf("Consumed item %d from index %d\n", item, table->out);
        table->out = (table->out + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(empty);
        sleep(2); // simulate consumption time
    }
    return NULL;
}

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    shared_table* table = mmap(0, sizeof(shared_table),
                               PROT_READ | PROT_WRITE, MAP_SHARED,
                               shm_fd, 0);

    pthread_t tid;
    pthread_create(&tid, NULL, consumer_thread, (void*) table);
    pthread_join(tid, NULL);

    return 0;
}
