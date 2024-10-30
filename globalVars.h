#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "list.h"
#include "semaphore.h"  // Ensure SEMAPHORE type is recognized

// Declarations for global variables
extern List* highPriority;
extern List* blockedQueue;
extern List* normalPriority;
extern List* lowPriority;
extern List* jobQueue;
extern List* messageQueue;
extern List* waitingToSendQueue;
extern List* waitingToReceiveQueue;

extern int highPriorityCount;
extern int normalPriorityCount;
extern int lowPriorityCount;
extern int jobQueueCount;
extern int messageQueueCount;

extern SEMAPHORE semaphores[MAX_SEMAPHORES];  // Only one correct declaration
extern int semaphoreCount;

#endif // GLOBALVAR_H
