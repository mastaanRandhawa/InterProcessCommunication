#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globalVars.h"
#include "process.h"

// Function prototypes to ensure functions are known before first use
PCB *find_process_by_id(int pid);
void remove_process_from_blocked_queue(int pid);
PCB *find_process_in_blocked_queue(int pid);

void init_ipc_module()
{
    messageQueue = List_create();
    if (!messageQueue)
    {
        perror("Failed to initialize IPC module queues");
        exit(EXIT_FAILURE);
    }
    blockedQueue = List_create();
    if (!blockedQueue)
    {
        perror("Failed to initialize blocked queue");
        exit(EXIT_FAILURE);
    }
}

void free_message(void *msg)
{
    if (msg)
    {
        free(msg);
    }
}

void cleanup_ipc_module()
{
    // Frees all messages still in the messageQueue.
    MESSAGE *msg;
    while ((msg = (MESSAGE *)List_trim(messageQueue)) != NULL)
    {
        free_message(msg);
    }
    List_free(messageQueue, free_message);
    List_free(blockedQueue, NULL); // Assuming no need to free individual PCBs as they are managed elsewhere
}

void send_message(int senderID, int recipientID, const char *message)
{
    MESSAGE *newMessage = (MESSAGE *)malloc(sizeof(MESSAGE));
    if (!newMessage)
    {
        perror("Failed to allocate memory for a new message");
        return;
    }

    newMessage->senderID = senderID;
    newMessage->recipientID = recipientID;
    strncpy(newMessage->message, message, sizeof(newMessage->message) - 1);
    newMessage->message[sizeof(newMessage->message) - 1] = '\0';
    newMessage->isReply = false;

    if (List_append(messageQueue, newMessage) == LIST_FAIL)
    {
        fprintf(stderr, "Failed to send message from %d to %d\n", senderID, recipientID);
        free(newMessage);
    }
    else
    {
        printf("Message sent from %d to %d: %s\n", senderID, recipientID, message);
        PCB *senderPCB = find_process_by_id(senderID);
        if (senderPCB)
        {
            printf("Sender Blocked!");
            senderPCB->state = 'b';
            List_append(blockedQueue, senderPCB);
            select_next_process_to_run();
        }
    }
}

void receive_message(int recipientID)
{
    MESSAGE *msg = NULL;
    bool messageFound = false;

    for (msg = List_first(messageQueue); msg != NULL; msg = List_next(messageQueue))
    {
        if (msg->recipientID == recipientID)
        {
            messageFound = true;
            printf("Message received by %d from %d: %s\n", recipientID, msg->senderID, msg->message);

            PCB *senderPCB = find_process_in_blocked_queue(msg->senderID);
            if (senderPCB)
            {
                remove_process_from_blocked_queue(msg->senderID);
                senderPCB->state = 'r';
                printf("Sender %d unblocked\n", msg->senderID);
            }

            List_remove(messageQueue); // Remove the message from the queue after processing
            free_message(msg);
            break;
        }
    }

    if (!messageFound)
    {
        printf("No message for recipient %d. Process blocked.\n", recipientID);
        PCB *recipientPCB = find_process_by_id(recipientID);
        if (recipientPCB && recipientPCB->state != 'b')
        {
            recipientPCB->state = 'b'; // Ensure the state is updated to 'b' here
            List_append(blockedQueue, recipientPCB);
            select_next_process_to_run();
        }
    }
}

void reply_message(int originalSenderPID, const char *message)
{
    // Assume we somehow find the original message or have the originalSenderPID accurately
    PCB *originalSenderPCB = find_process_in_blocked_queue(originalSenderPID);
    if (!originalSenderPCB)
    {
        printf("Original sender %d not found or not blocked.\n", originalSenderPID);
        return;
    }

    MESSAGE *replyMessage = (MESSAGE *)malloc(sizeof(MESSAGE));
    if (!replyMessage)
    {
        perror("Failed to allocate memory for the reply message");
        return;
    }

    replyMessage->senderID = originalSenderPCB->pid; // This should be the recipient of the reply
    replyMessage->recipientID = originalSenderPID;   // The original sender
    strncpy(replyMessage->message, message, sizeof(replyMessage->message) - 1);
    replyMessage->message[sizeof(replyMessage->message) - 1] = '\0';
    replyMessage->isReply = true;

    if (List_append(messageQueue, replyMessage) == LIST_FAIL)
    {
        fprintf(stderr, "Failed to send reply to %d\n", originalSenderPID);
        free(replyMessage);
    }
    else
    {
        printf("Reply sent to %d: %s\n", originalSenderPID, message);
        originalSenderPCB->state = 'r'; // Unblocking the original sender
        remove_process_from_blocked_queue(originalSenderPID);
    }
}

PCB *find_process_by_id(int pid)
{
    PCB *pcb;
    for (pcb = List_first(jobQueue); pcb != NULL; pcb = List_next(jobQueue))
    {
        if (pcb->pid == pid)
        {
            return pcb;
        }
    }
    return NULL; // Process not found
}

PCB *find_process_in_blocked_queue(int pid)
{
    PCB *pcb;
    for (pcb = List_first(blockedQueue); pcb != NULL; pcb = List_next(blockedQueue))
    {
        if (pcb->pid == pid)
        {
            return pcb; // Found the process in the blocked queue
        }
    }
    return NULL; // Process not found in the blocked queue
}

void remove_process_from_blocked_queue(int pid)
{
    PCB *pcb;
    for (pcb = List_first(blockedQueue); pcb != NULL; pcb = List_next(blockedQueue))
    {
        if (pcb->pid == pid)
        {
            List_remove(blockedQueue); // This assumes pcb is the current item in the blocked queue
            printf("Process %d removed from blocked queue.\n", pid);
            return;
        }
    }
    printf("Process %d not found in blocked queue.\n", pid);
}
