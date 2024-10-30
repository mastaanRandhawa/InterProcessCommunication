#include "utils.h"
#include <stdio.h>

// Gets the response from the user for the menu selection
char getMenuResponse() {
    printf("\nEnter your command: ");
    char input = getchar();
    // Consume any extra characters in the input buffer.
    while (getchar() != '\n');
    return input;
}
