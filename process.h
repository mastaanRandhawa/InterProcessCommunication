// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include "list.h"

typedef struct PCB {
    int pid;        // Process ID
    int priority;   // Process priority
    char state;     // Process state: 'u' for running, 'r' for ready, etc.
} PCB;

// Initializes the process management module
void init_process_management();

// Creates a new process with the given priority
int create(int priority);

// Kills a process with the given process ID
int kll(int id);

// Kills the currently running process
void killCurrent();

// Displays information about all processes
void totalinfo();

// Forks the currently running process
int fork();

// Simulates a quantum of CPU time passing for the current process
int quantum();

// Displays state information of a process
void procinfo(int id);

// Blocks current process
void block_current_process_and_switch(int semaphoreID);

// Unlbocks blocked process
void unblock_process(int semaphoreID);

// Free
void cleanup_process_management();

void removeProcessByID(List *queue, int pid);

int select_next_process_to_run();

PCB* find_current_running_process();

#endif // PROCESS_H
