#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <semaphore.h>
#include <stdio.h>

typedef struct {
    pthread_mutex_t mutex;
    uint32_t counter;
} shared_data;

int main() {
    int fd = shm_open("/my_shm", O_RDWR, 0666);
    shared_data *ptr = mmap(0, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // Open the semaphore
    sem_t *sem = sem_open("/my_sem", 0);

    // Decrease the counter by 1 every 2 seconds
    while (ptr->counter > 0) {
        pthread_mutex_lock(&ptr->mutex);
        ptr->counter--;
        printf("Counter: %d\n", ptr->counter);
        pthread_mutex_unlock(&ptr->mutex);
        sleep(2);
    }

    printf("Counter reached 0. Signal other process to continue.\n")
    // Signal the first process to continue
    sem_post(sem);

    // Close access to semaphore
    sem_close(sem);

    // Clean up
    munmap(ptr, sizeof(shared_data));

    return 0;
}
