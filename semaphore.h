// semaphore.h
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "list.h" // For managing queues of blocked processes.
#define MAX_SEMAPHORES 5

typedef struct SEMAPHORE {
    int sid;       // Semaphore ID
    int value;     // Semaphore value
    List* blocked; // Queue of PCBs blocked on this semaphore
} SEMAPHORE;

// Function prototypes
void init_semaphore_module();
int create_semaphore(int semaphoreID, int initialVal);
void semaphore_p(int semaphoreID);
void semaphore_v(int semaphoreID);

#endif // SEMAPHORE_H
