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
#define NUM_CUSTOMERS 5
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
                if(finish[i] = can_allocate(i)){
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

    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] -= request[i];
        allocation[n_customer][i] += request[i];
        need[n_customer][i] -= request[i];
    }
    if(safe()){
        return true;
    }
    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] += request[i];
        allocation[n_customer][i] -= request[i];
        need[n_customer][i] += request[i];
    }
    return true;
}

// Release resources, returns true if successful
bool release_res(int n_customer, int release[]){
    for(int i = 0; i < NUM_RESOURCES; i++){
        allocation[n_customer][i] -= release[i];
    }
    return true;
}

void * thread(void * arg){
    bool request;
    pthread_t pid;
    pid = pthread_self();
    printf("%lu\n", pid);
    // while(true){
    //     request = request_res();
        
    // }
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

    pthread_t pid[NUM_CUSTOMERS];

    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < NUM_RESOURCES; i++){
        available[i] = atoi(argv[i+1]);
        for(int j = 0; j < NUM_RESOURCES; j++){
            maximum[j][i] = rand() % atoi(argv[i+1]);
            allocation[j][i] = 0;
            need[j][i] = 0;
        }
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        pthread_create(&pid[i], NULL, thread, NULL);
    }

    for(int i = 0; i < NUM_CUSTOMERS; i++){
        pthread_join(pid[i], NULL);
    }


    return EXIT_SUCCESS;
}