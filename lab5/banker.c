/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "banker.h"

// Put any other macros or constants here using #define
// May be any values >= 0
#define NUM_CUSTOMERS 2
#define NUM_RESOURCES 3


// Put global environment variables here
// Available amount of each resource
int available[NUM_RESOURCES];

// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];

// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];

// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

int work[NUM_RESOURCES];
bool finish[NUM_CUSTOMERS];

pthread_mutex_t mutex;

bool is_safe(bool finish[]){
    for(int j = 0; j < NUM_CUSTOMERS; j++){
        if(!finish[j]){
            return false;
        }
    }
    return true;
}

bool can_allocate(int n_customer){
    for(int j = 0; j < NUM_RESOURCES; j++){
        if(need[n_customer][j] > work[j]){
            return false;
        }
    }
    return true;
}

bool safe(){
    for(int i = 0; i < NUM_RESOURCES; i++){
        work[i] = available[i];
    }
    for(int i = 0; i < NUM_CUSTOMERS; i++){
        finish[i] = false;
    }

    for(int k = 0; k < NUM_CUSTOMERS; k++){
        for(int i = 0; i < NUM_CUSTOMERS; i++){
            if(!finish[i]){
                finish[i] = can_allocate(i);
                if(finish[i]){
                    for(int j = 0; j < NUM_RESOURCES; j++){
                        work[i] += allocation[i][j];
                    }
                }else{
                    if(is_safe(finish)){
                        return true;
                    }
                }
            }
        }
    }
    return is_safe(finish);
}

// Define functions declared in banker.h here
bool request_res(int n_customer, int request[]){
    for(int i = 0; i < NUM_RESOURCES; i++){
        if(request[i] > need[n_customer][i]){
            return false;
        }
    }

    for(int i = 0; i < NUM_RESOURCES; i++){
        if(request[i] > available[i]){
            return false;
        }
    }
    // pthread_mutex_lock(&mutex);
    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] -= request[i];
        allocation[n_customer][i] += request[i];
        need[n_customer][i] -= request[i];
    }
    if(safe()){
        // pthread_mutex_unlock(&mutex);
        printf("System is in safe state\n");
        return true;
    }else{
        // pthread_mutex_unlock(&mutex);
        printf("System is in unsafe state\n");
        // pthread_mutex_lock(&mutex);
        for(int i = 0; i < NUM_RESOURCES; i++){
            available[i] += request[i];
            allocation[n_customer][i] -= request[i];
            need[n_customer][i] += request[i];
        }
        // pthread_mutex_unlock(&mutex);
        return false;
    }
}

// Release resources, returns true if successful
bool release_res(int n_customer, int release[]){
    printf("Thread %d releasing: ", n_customer);
    // pthread_mutex_lock(&mutex);
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ", release[i]);
        allocation[n_customer][i] -= release[i];
        available[i] += release[i];
        need[n_customer][i] += release[i];
    }
    printf("\n");
    // pthread_mutex_unlock(&mutex);
    return true;
}

void * thread(void * arg){
    bool request;
    int id = (int) arg;
    int req_resource[NUM_RESOURCES];
    int rel_resource[NUM_RESOURCES];
    int ran_num;
    while(true){
        sleep(1);
        printf("Available: ");
        for(int i = 0; i < NUM_RESOURCES; i++){
            printf("%d ", available[i]);
        }    

        printf("\nNeed: \n");
        for(int j = 0; j <NUM_CUSTOMERS; j++){
            printf("Thread %d: ", j);
            for(int i = 0; i < NUM_RESOURCES; i++){
                printf("%d ", need[j][i]);
            }
            printf("\n");
        }

        printf("\nAllocated: \n");
        for(int j = 0; j <NUM_CUSTOMERS; j++){
            printf("Thread %d: ", j);
            for(int i = 0; i < NUM_RESOURCES; i++){
                printf("%d ", allocation[j][i]);
            }
            printf("\n");
        }
        printf("Thread %d requesting: ", id);
        for(int i = 0; i < NUM_RESOURCES; i++){
            if(need[id][i] == 0){
                req_resource[i] = 0;
            }else{
                req_resource[i] = rand() % (need[id][i]) + 1;
            }
            printf("%d ", req_resource[i]);
        }
        printf("\n");
        pthread_mutex_lock(&mutex);
        request = request_res(id, req_resource);
        pthread_mutex_unlock(&mutex);
        if(request){
            printf("Request granted.\n");
        }else{
            printf("Request denied.\n");
        }
        for(int i = 0; i < NUM_RESOURCES; i++){
            if(allocation[id][i] == 0){
                rel_resource[i] = 0;
            }else{
                rel_resource[i] = rand() % (allocation[id][i]) + 1;
            }
        }
        printf("\n");
        pthread_mutex_lock(&mutex);
        release_res(id, rel_resource);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    // ==================== YOUR CODE HERE ==================== //

    // Read in arguments from CLI, NUM_RESOURCES is the number of arguments   
    
    // Allocate the available resources

    // Initialize the pthreads, locks, mutexes, etc.

    // Run the threads and continually loop

    // The threads will request and then release random numbers of resources

    // If your program hangs you may have a deadlock, otherwise you *may* have
    // implemented the banker's algorithm correctly
    
    // If you are having issues try and limit the number of threads (NUM_CUSTOMERS)
    // to just 2 and focus on getting the multithreading working for just two threads

    pthread_t tid[NUM_CUSTOMERS];
    pthread_attr_t attr;
    int error;

    pthread_mutex_init(&mutex, NULL);

    pthread_attr_init(&attr);

    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] = atoi(argv[i+1]);
        for(int j = 0; j < NUM_CUSTOMERS; j++){
            maximum[j][i] = rand() % (atoi(argv[i+1]) - 1) + 1;
            allocation[j][i] = 0;
            need[j][i] = maximum[j][i];
        }
    }
    printf("Available: ");
    for(int i = 0; i < NUM_RESOURCES; i++){
        printf("%d ", available[i]);
    }    

    printf("\nMaximum: \n");
    for(int j = 0; j <NUM_CUSTOMERS; j++){
        printf("Thread %d: ", j);
        for(int i = 0; i < NUM_RESOURCES; i++){
            printf("%d ", maximum[j][i]);
        }
        printf("\n");
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        error = pthread_create(&tid[i], &attr, thread, (void*) i);
        if(error){
            printf("Error creating thread %d: %d\n", i, error);
            exit(-1);
        }
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        error = pthread_join(tid[i], NULL);
        if(error){
            printf("Error joining thread %d: %d\n", i, error);
            exit(-1);
        }
    }
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);

    pthread_exit(NULL);

    return EXIT_SUCCESS;
}