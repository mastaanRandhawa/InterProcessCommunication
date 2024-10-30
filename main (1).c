#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "process.h"
#include "ipc.h"
#include "utils.h"
#include "semaphore.h"
#include "globalVars.h"

int main()
{

    init_process_management();
    init_ipc_module();
    init_semaphore_module();

    printf("\n=== Simple OS Simulation ===\n");
    printf("Enter 'X' as a command to exit the program.\n\n");

    char command;
    int priority, pid, semaphoreID, initialVal, sendTo;
    char message[256]; // Assuming messages are strings up to 255 characters

    bool run = true;
    printf("\nCommand (C, K, T, X, I, E, Q, F, N, P, V, S, R, Y): ");
    while (run)
    {
        command = getMenuResponse();

        switch (command)
        {
        case 'C': // Create a new process
            printf("Enter Process Priority (0-High, 1-Medium, 2-Low): ");
            scanf("%d", &priority);
            while (getchar() != '\n')
                ; // Clear the input buffer
            create(priority);
            break;

        case 'K': // Kill a process by PID
            printf("Enter Process ID to kill: ");
            scanf("%d", &pid);
            kll(pid);
            break;

        case 'T': // Display total information about all processes
            totalinfo();
            break;

        case 'X': // Exit the simulation
            printf("Exiting simulation.\n");
            // Before exiting the application
            // cleanup_process_management();
            // cleanup_ipc_module();

            run = false;
            break;

        case 'I': // Display information for a specific process
            printf("Enter Process ID for information: ");
            scanf("%d", &pid);
            procinfo(pid);
            break;

        case 'E': // Kill the currently running process
            killCurrent();
            break;

        case 'Q': // Simulate a quantum of CPU time
            quantum();
            break;

        case 'F': // Fork the currently running process
            fork();
            break;

        case 'N': // Create a new semaphore
            printf("Enter initial semaphore value: ");
            scanf("%d", &initialVal);
            create_semaphore(semaphoreCount, initialVal); // Assuming semaphoreCount tracks the next available ID
            break;

        case 'P': // Semaphore P operation
            printf("Enter Semaphore ID for P operation: ");
            scanf("%d", &semaphoreID);
            semaphore_p(semaphoreID);
            break;

        case 'V': // Semaphore V operation
            printf("Enter Semaphore ID for V operation: ");
            scanf("%d", &semaphoreID);
            semaphore_v(semaphoreID);
            break;

        case 'S': // Send a message
            printf("Enter recipient Process ID: ");
            scanf("%d", &sendTo);
            printf("Enter your message: ");
            scanf("%s", message); // Note: This is a simplistic approach, consider using fgets for spaces


            send_message(find_current_running_process()->pid, sendTo, message);

            break;

        case 'R': // Receive a message

            receive_message(find_current_running_process()->pid);
            break;

        case 'Y': // Reply to a message
            printf("Enter the Process ID you are replying to: ");
            scanf("%d", &sendTo);
            printf("Enter your message: ");
            scanf("%s", message); // Note: This is a simplistic approach, consider using fgets for spaces

            reply_message(sendTo, message);
            break;

        default:
            printf("Unknown command.\n");
            printf("\nCommand (C, K, T, X, I, E, Q, F, N, P, V, S, R, Y): ");
        }
    }

    return 0;
}
