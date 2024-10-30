#include <stdio.h>
#include <stdlib.h>
#include "semaphore.h"
#include "globalVars.h"
#include "process.h"

void init_semaphore_module() {
    for (int i = 0; i < MAX_SEMAPHORES; ++i) {
        semaphores[i].sid = -1; // Indicates an unused semaphore slot
        semaphores[i].value = 0;
        semaphores[i].blocked = List_create();
    }
}

int create_semaphore(int semaphoreID, int initialVal) {
    if (semaphoreCount >= MAX_SEMAPHORES) {
        printf("Maximum semaphore limit reached.\n");
        return -1;
    }

    semaphores[semaphoreCount].sid = semaphoreID;
    semaphores[semaphoreCount].value = initialVal;
    semaphores[semaphoreCount].blocked = List_create();
    if (semaphores[semaphoreCount].blocked == NULL) {
        fprintf(stderr, "Failed to create a queue for semaphore %d.\n", semaphoreID);
        return -1;
    }

    printf("Semaphore %d created with initial value %d.\n", semaphoreID, initialVal);
    semaphoreCount++;
    return semaphoreID;
}

void semaphore_p(int semaphoreID) {
    for (int i = 0; i < semaphoreCount; ++i) {
        if (semaphores[i].sid == semaphoreID) {
            semaphores[i].value--;
            if (semaphores[i].value < 0) {
                printf("Process is blocked on semaphore %d.\n", semaphoreID);
                block_current_process_and_switch(semaphoreID);
            }
            return;
        }
    }
    printf("Semaphore %d not found.\n", semaphoreID);
}

void semaphore_v(int semaphoreID) {
    for (int i = 0; i < semaphoreCount; ++i) {
        if (semaphores[i].sid == semaphoreID) {
            semaphores[i].value++;
            if (semaphores[i].value <= 0) {
                printf("A process is unblocked on semaphore %d.\n", semaphoreID);
                unblock_process(semaphoreID);
            }
            return;
        }
    }
    printf("Semaphore %d not found.\n", semaphoreID);
}
