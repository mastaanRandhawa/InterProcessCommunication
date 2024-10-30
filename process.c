#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "globalVars.h"

int pidnum = 1;

void init_process_management()
{
    highPriority = List_create();
    normalPriority = List_create();
    lowPriority = List_create();
    jobQueue = List_create(); //MASTER QUEUE FOR EASY LOOK UPS
}

void currentList(List *Q)
{
    PCB *pcb;
    for (pcb = List_first(Q); pcb != NULL; pcb = List_next(Q))
    {
        if (pcb->state == 'u')
        {
            break;
        }
    }
}

int create(int priority)
{
    PCB *controlBlock = (PCB *)malloc(sizeof(PCB));
    if (!controlBlock)
    {
        perror("Failed to allocate memory for PCB");
        return -1;
    }

    controlBlock->pid = pidnum++;
    controlBlock->priority = priority;
    controlBlock->state = (List_count(jobQueue) == 0) ? 'u' : 'r';

    List *targetQueue = NULL;
    switch (priority)
    {
    case 0:
        targetQueue = highPriority;
        break;
    case 1:
        targetQueue = normalPriority;
        break;
    case 2:
        targetQueue = lowPriority;
        break;
    default:
        free(controlBlock);
        printf("Invalid Priority! Priority can be 0, 1, or 2.");
        return -1;
    }

    List_append(targetQueue, controlBlock);
    List_append(jobQueue, controlBlock);

    currentList(jobQueue);
    currentList(highPriority);
    currentList(normalPriority);
    currentList(lowPriority);

    printf("Process created with PID %d and priority %d\n", controlBlock->pid, priority);
    return controlBlock->pid;
}

void removeProcessByID(List *queue, int pid)
{
    PCB *pcb;
    for (pcb = List_first(queue); pcb != NULL; pcb = List_next(queue))
    {
        if (pcb->pid == pid)
        {
            List_remove(queue); // Remove the process from its priority queue
            return;             // Exit the function once the process is found and removed
        }
    }
}

PCB* find_current_running_process() {
    PCB *currentProcess = NULL;
    // Assuming jobQueue contains all processes in the system
    for (currentProcess = List_first(jobQueue); currentProcess != NULL; currentProcess = List_next(jobQueue)) {
        if (currentProcess->state == 'u') {  // 'u' indicates the process is currently running
            return currentProcess;  // Return the currently running process
        }
    }
    return NULL; // Return NULL if no running process is found
}

bool try_set_next_process_running(List *priorityQueue)
{
    PCB *pcb;
    for (pcb = List_first(priorityQueue); pcb != NULL; pcb = List_next(priorityQueue))
    {
        if (pcb->state == 'r')
        {
            pcb->state = 'u';
            printf("Process with PID %d from priority queue is now running.\n", pcb->pid);
            return true;
        }
    }
    return false;
}

int select_next_process_to_run()
{
    if (!try_set_next_process_running(highPriority))
    {
        if (!try_set_next_process_running(normalPriority))
        {
            if (!try_set_next_process_running(lowPriority))
            {
                printf("No ready processes available in any queue.\n");
                return -1;
            }
        }
    }
    
    return 0;
}

int kll(int id)
{
    bool found = false;
    PCB *pcb;
    for (pcb = List_first(jobQueue); pcb != NULL; pcb = List_next(jobQueue))
    {
        if (pcb->pid == id)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        printf("Process ID %d not found.\n", id);
        return 0;
    }

    List_remove(jobQueue); // Assumes pcb is the current item
    
    if (pcb->priority == 0)
    {
        removeProcessByID(highPriority, pcb->pid);
    }
    else if (pcb->priority == 1)
    {
        removeProcessByID(normalPriority, pcb->pid);
    }
    else if (pcb->priority == 2)
    {
        removeProcessByID(lowPriority, pcb->pid);
    }

    free(pcb); // Safely free the PCB memory
    printf("Process ID %d killed.\n", id);

    select_next_process_to_run();

    return 1;
}

void killCurrent()
{
    PCB *pcb;
    for (pcb = List_first(jobQueue); pcb != NULL; pcb = List_next(jobQueue))
    {
        if (pcb->state == 'u')
        { 
            List_remove(jobQueue); // Remove from jobQueue

            if (pcb->priority == 0)
            {
                removeProcessByID(highPriority, pcb->pid);
            }
            else if (pcb->priority == 1)
            {
                removeProcessByID(normalPriority, pcb->pid);
            }
            else if (pcb->priority == 2)
            {
                removeProcessByID(lowPriority, pcb->pid);
            }

            free(pcb); // Safely free the PCB memory
            printf("Current process killed.\n");

            select_next_process_to_run();
            return;
        }
    }
    printf("No currently running process to kill.\n");
}

void totalinfo()
{
    PCB *pcb;
    int count = 0;
    printf("--JOBQUEUE--\n");
    for (pcb = List_first(jobQueue); pcb != NULL; pcb = List_next(jobQueue))
    {
        printf("PID: %d, Priority: %d, State: %c\n", pcb->pid, pcb->priority, pcb->state);
        count++;
    }
    if (count == 0)
    {
        printf("No processes in the Jobqueue.\n");
    }

    // Display information for other queues similar to JobQueue...

    // Ensure each queue is iterated through and information is printed.
    // Repeat the process for highPriority, normalPriority, and lowPriority queues.
}

int fork()
{
    PCB *currentProcess;
    for (currentProcess = List_first(jobQueue); currentProcess != NULL; currentProcess = List_next(jobQueue))
    {
        if (currentProcess->state == 'u')
        {                                           
            return create(currentProcess->priority); // Re-create a process with the same priority
        }
    }
    printf("No process to fork.\n");
    return -1;
}

int quantum()
{
    PCB *currentProcess = NULL;
    for (currentProcess = List_first(jobQueue); currentProcess != NULL; currentProcess = List_next(jobQueue))
    {
        if (currentProcess->state == 'u')
        {
            select_next_process_to_run();
            currentProcess->state = 'r'; // Set the currently running process to ready
            return 0;
        }
    }
    return -1; // Indicates no running process was found
}

void procinfo(int id)
{
    PCB *pcb;
    for (pcb = List_first(jobQueue); pcb != NULL; pcb = List_next(jobQueue))
    {
        if (pcb->pid == id)
        {
            printf("PID: %d, Priority: %d, State: %c\n", pcb->pid, pcb->priority, pcb->state);
            return;
        }
    }
    printf("Process ID %d not found.\n", id);
}

void cleanup_process_management()
{
    // Free all processes in each queue and then the queues themselves.
    List_free(jobQueue, free);
    List_free(highPriority, free);
    List_free(normalPriority, free);
    List_free(lowPriority, free);
}
           

void block_current_process_and_switch(int semaphoreID)
{
    PCB *currentProcess = NULL;
    for (currentProcess = List_first(jobQueue); currentProcess != NULL; currentProcess = List_next(jobQueue))
    {
        if (currentProcess->state == 'u')
        {                                // 'u' represents running
            currentProcess->state = 'b'; // 'b' represents blocked

            // Now, currentProcess points to the process that needs to be blocked.
            // Remove the current process from jobQueue.
            List_remove(jobQueue);

            // Also, remove the current process from its priority queue.
            switch (currentProcess->priority)
            {
            case 0:
                removeProcessByID(highPriority, currentProcess->pid);
                break;
            case 1:
                removeProcessByID(normalPriority, currentProcess->pid);
                break;
            case 2:
                removeProcessByID(lowPriority, currentProcess->pid);
                break;
            default:
                printf("Invalid priority while blocking process.\n");
                return;
            }

            // Append the blocked process to the semaphore's blocked queue.
            List_append(semaphores[semaphoreID].blocked, currentProcess);
            printf("Process with PID %d blocked and added to semaphore %d's blocked queue.\n", currentProcess->pid, semaphoreID);

            // Try to run the next available process
            select_next_process_to_run();
            return;
        }
    }
    printf("No currently running process to block.\n");
}

void unblock_process(int semaphoreID)
{
    if (List_count(semaphores[semaphoreID].blocked) > 0)
    {
        PCB *process = List_remove(semaphores[semaphoreID].blocked);
        if (process)
        {
            process->state = 'r'; // 'r' represents ready
            List *targetQueue = NULL;
            switch (process->priority)
            {
            case 0:
                targetQueue = highPriority;
                break;
            case 1:
                targetQueue = normalPriority;
                break;
            case 2:
                targetQueue = lowPriority;
                break;
            default:
                printf("Invalid priority during unblocking.\n");
                return;
            }
            List_append(targetQueue, process);
            List_append(jobQueue, process);
            printf("Process PID %d unblocked and set to ready, added to priority %d queue.\n", process->pid, process->priority);
            select_next_process_to_run(); // In case no process is running, try to run the next available process
        }
    }
    else
    {
        printf("No processes are blocked in semaphore %d.\n", semaphoreID);
    }
}

