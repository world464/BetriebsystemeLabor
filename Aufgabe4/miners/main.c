#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_MINERS 9
#define MINE_CAPACITY 1000

#define MAX_CARRY 20
#define MAX_WORK 3
#define MAX_REST 2
#define MIN_REST 1

int32_t gems_in_mine = MINE_CAPACITY;
bool dragon_is_awake = false;

pthread_mutex_t mine_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t dragon_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t dragon_sleeps_cond = PTHREAD_COND_INITIALIZER;

void* miner(void* arg) {
    int miner_id = *(int *) arg;
    while (1) {
        pthread_mutex_lock(&mine_mutex);
        while(dragon_is_awake)
            pthread_cond_wait(&dragon_sleeps_cond,&dragon_mutex);
        if (dragon_is_awake) {
            printf("Miner %d got eaten.\n", miner_id);
            pthread_exit(NULL);
        }
        if (gems_in_mine > 0) {
            int gems_mined = rand() % MAX_CARRY + 1;

            if (gems_mined > gems_in_mine)
                gems_mined = gems_in_mine;

            sleep(rand() % MAX_WORK + 1);  // Working
            gems_in_mine -= gems_mined;
            printf("Miner %d mined %d gems. Gems left in mine: %d\n", miner_id, gems_mined, gems_in_mine);
        } else {
            printf("Mine is empty! Miner %d can't mine anymore!\n", miner_id);
        }
        pthread_mutex_unlock(&mine_mutex);

        //sleep(rand() % MAX_REST);  // Alter Waiting Code
          sleep(MIN_REST + rand() % MAX_REST); //Neuer Waiting Code
    }
}

void* dragon(void* arg) {
    while (1) {
        sleep(rand() % 20 + 20);  // Sleeping

        pthread_mutex_lock(&dragon_mutex);
        dragon_is_awake = true;
        printf("Dragon is awake!\n");
        pthread_mutex_unlock(&dragon_mutex);

        sleep(rand() % 2 + 2);  // Wandering around
        pthread_cond_signal(&dragon_sleeps_cond); // Dragon tries to lure a miner in the mine
        sleep(rand() % 3 + 3);

        pthread_mutex_lock(&dragon_mutex);
        dragon_is_awake = false;
        printf("Dragon went back to sleep.\n");
        pthread_cond_broadcast(&dragon_sleeps_cond);
        pthread_mutex_unlock(&dragon_mutex);
    }
}

int main() {
    pthread_t miners[NUM_MINERS];
    pthread_t dragon_thread;
    int miner_ids[NUM_MINERS];

    pthread_create(&dragon_thread, NULL, &dragon, NULL);

    for (int i = 0; i < NUM_MINERS; i++) {
        miner_ids[i] = i+1;
        pthread_create(&miners[i], NULL, &miner, &miner_ids[i]);
    }

    for (int i = 0; i < NUM_MINERS; i++) {
        pthread_join(miners[i], NULL);
    }

    return 0;
}
