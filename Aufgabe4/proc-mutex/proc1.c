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
    int fd = shm_open("/my_shm", O_CREAT | O_RDWR, 0666);
    ftruncate(fd, sizeof(shared_data));
    shared_data *ptr = mmap(0, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    // Initialize a process-shared mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&ptr->mutex, &attr);
    pthread_mutexattr_destroy(&attr);

    // Create a named semaphore
    sem_t *sem = sem_open("/my_sem", O_CREAT, 0666, 0);

    // Increase the counter until it reaches 15
    while(ptr->counter < 15) {
        pthread_mutex_lock(&ptr->mutex);
        ptr->counter++;
        printf("Counter: %d\n", ptr->counter);
        pthread_mutex_unlock(&ptr->mutex);
        sleep(1);
    }

    printf("Wait for other thread to signal ...\n");
    // Wait for the second process to finish
    sem_wait(sem);
    printf("Signal received. Start clean up.\n");

    // Clean up
    pthread_mutex_destroy(&ptr->mutex);
    munmap(ptr, sizeof(shared_data));
    shm_unlink("/my_shm");
    sem_close(sem);
    sem_unlink("/my_sem");

    return 0;
}