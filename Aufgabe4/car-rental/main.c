#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_CARS 6
#define NUM_CUSTOMERS 100
#define MAX_RENT_TIME 5
#define NUM_PREFERRED_CARS 3

sem_t available_cars;
sem_t preferedcar0;
sem_t preferedcar1;
sem_t preferedcar2;

typedef struct {
    int customer_id;
    int preferred_car_id;
} Customer;

void *rent_car(void *arg) {
    Customer *customer = (Customer *) arg;

    // Wait until a car is available and then rent it
    int preferedCarId = customer->preferred_car_id;
    int chosenCar = -2;

    if (preferedCarId == 0) {
        sem_wait(&preferedcar0);
        printf("Customer %d with preference %d is renting preferenced car 0\n", customer->customer_id, customer->preferred_car_id);
        chosenCar = 0;
    } else if (preferedCarId == 1) {
        sem_wait(&preferedcar1);
        printf("Customer %d with preference %d is renting preferenced car 1\n", customer->customer_id, customer->preferred_car_id);
        chosenCar = 1;
    } else if (preferedCarId == 2) {
        sem_wait(&preferedcar2);
        printf("Customer %d with preference %d is renting preferenced car 2\n", customer->customer_id, customer->preferred_car_id);
        chosenCar = 2;
    } else { //customerId == -1
        if (sem_trywait(&available_cars) == -1)
            chosenCar = -1;
        else if (sem_trywait(&preferedcar0) == -1)
            chosenCar = 0;
        else if (sem_trywait(&preferedcar1) == -1)
            chosenCar = 1;
        else if(sem_trywait(&preferedcar2) == -1)
            chosenCar = 2;
        else {
            sem_wait(&available_cars);
            chosenCar = -1;
        }
        printf("Customer %d with preference %d is renting a car\n", customer->customer_id, customer->preferred_car_id);
    }


    // The car is used for some time
    sleep(rand() % MAX_RENT_TIME + 1);

    printf("Customer %d has returned their car\n", customer->customer_id);

    switch (chosenCar) {
        case -1:
            sem_post(&available_cars);
            break;
        case 0:
            sem_post(&preferedcar0);
            break;
        case 1:
            sem_post(&preferedcar1);
            break;
        case 2:
            sem_post(&preferedcar2);
            break;
        default:
            printf("Fehler");
            break;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t customers[NUM_CUSTOMERS];
    Customer customer_data[NUM_CUSTOMERS];

    // Initialize the semaphore with the number of available cars
    sem_init(&available_cars, 0, NUM_CARS - NUM_PREFERRED_CARS);
    sem_init(&preferedcar0, 0, 1);
    sem_init(&preferedcar1, 0, 1);
    sem_init(&preferedcar2, 0, 1);

    // Create threads for customers
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_data[i].customer_id = i;
        int rand_num = rand();
        customer_data[i].preferred_car_id =
                rand_num % 2 == 0 ? -1 : rand_num % NUM_PREFERRED_CARS; // 50% chance of no preference
        pthread_create(&customers[i], NULL, rent_car, &customer_data[i]);
    }

    // Wait until all customers have returned their cars
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    printf("Every customer has been served.\n");

    sem_destroy(&available_cars);

    return 0;
}
