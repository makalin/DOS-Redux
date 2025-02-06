/**
 * DOS Redux Shell
 * Command-line interface implementation
 */

#include <stdio.h>
#include <string.h>

#define MAX_CMD_LENGTH 256
#define PROMPT "DOS>"

void shell_loop() {
    char command[MAX_CMD_LENGTH];
    
    while (1) {
        printf("%s ", PROMPT);
        if (fgets(command, MAX_CMD_LENGTH, stdin) != NULL) {
            command[strcspn(command, "\n")] = 0;
            process_command(command);
        }
    }
}

void process_command(const char* cmd) {
    // TODO: Implement command processing
    if (strcmp(cmd, "exit") == 0) {
        // Handle exit command
    } else if (strcmp(cmd, "dir") == 0) {
        // Handle directory listing
    } else if (strcmp(cmd, "help") == 0) {
        // Display help information
    } else {
        printf("Unknown command: %s\n", cmd);
    }
} 