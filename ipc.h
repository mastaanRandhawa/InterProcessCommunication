#ifndef IPC_H
#define IPC_H

#include "list.h"

typedef struct MESSAGE {
    int messageID;      // Unique identifier for the message-reply pair
    int senderID;
    int recipientID;
    char message[41];   // Message text, with space for null-terminator
    bool isReply;       // Flag to indicate if this is a reply message
} MESSAGE;


// Function prototypes
void init_ipc_module();
void send_message(int senderID, int recipientID, const char* message);
void receive_message(int recipientID);
void reply_message(int recipientID, const char* message);
void cleanup_ipc_module();  // Declare the cleanup function.

#endif // IPC_H
