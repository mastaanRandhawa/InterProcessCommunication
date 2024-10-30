#include "globalVars.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Define the global variables with initial values as appropriate
List* highPriority;
List* normalPriority;
List* lowPriority;
List* jobQueue;
List* messageQueue;
List* blockedQueue;

int highPriorityCount = 0;
int normalPriorityCount = 0;
int lowPriorityCount = 0;
int jobQueueCount = 0;
SEMAPHORE semaphores[MAX_SEMAPHORES];  // Only one correct declaration
int semaphoreCount = 0;
int messageQueueCount = 0;
