#include "shared.h"

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

void* producer_thread(void* arg) {
    shared_table* table = (shared_table*) arg;
    sem_t* mutex = sem_open("/mutex_sem", 0);
    sem_t* empty = sem_open("/empty_sem", 0);
    sem_t* full  = sem_open("/full_sem", 0);

    int item = 0;
    while (1) {
        sleep(1); // simulate production time
        item++;

        sem_wait(empty);
        sem_wait(mutex);

        table->buffer[table->in] = item;
        printf("Produced item %d at index %d\n", item, table->in);
        table->in = (table->in + 1) % BUFFER_SIZE;

        sem_post(mutex);
        sem_post(full);
    }
    return NULL;
}

int main() {
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared_table));

    shared_table* table = mmap(0, sizeof(shared_table),
                               PROT_READ | PROT_WRITE, MAP_SHARED,
                               shm_fd, 0);

    table->in = 0;
    table->out = 0;

    sem_t* mutex = sem_open("/mutex_sem", O_CREAT, 0666, 1);
    sem_t* empty = sem_open("/empty_sem", O_CREAT, 0666, BUFFER_SIZE);
    sem_t* full  = sem_open("/full_sem", O_CREAT, 0666, 0);

    pthread_t tid;
    pthread_create(&tid, NULL, producer_thread, (void*) table);
    pthread_join(tid, NULL);

    return 0;
}
