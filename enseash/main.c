#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define EXCLUDE_NULL_CHAR 1 // Define EXCLUDE_NULL_CHAR to exclude the null char at the end of the prompt.
#define MAX_COMMAND_LENGTH 100 // Define the maximal size of a command

void displayWelcomeMessage() {
    const char welcome_message[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";

    // Using of sizeof(welcome_message) to determinate the size of the prompt.
    write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - EXCLUDE_NULL_CHAR);
}

void executeCommand(const char *command) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        execlp(command, command, NULL);
        // If execlp returns, there was an error
        write(STDOUT_FILENO, "Error executing command\n", strlen("Error executing command\n"));
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        // Wait for the child to finish
        waitpid(pid, NULL, 0);
    } else { // Fork failed
        write(STDOUT_FILENO, "Error creating child process\n", strlen("Error creating child process\n"));
        exit(EXIT_FAILURE);
    }
}

// Function to display the current date
void displayDate() {
    executeCommand("date");
}

// Function to display a fortune
void displayFortune() {
    executeCommand("fortune");
}

int main() {

    // Instead of using a printf, we're gonna use the read-write logic
    const char prompt[] = "enseash % ";
    char command[MAX_COMMAND_LENGTH];

    displayWelcomeMessage();

    while (1) {
        write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
        // We are gonna write all the REPL logic there

        // Reading of the typed command
        ssize_t command_length = read(STDIN_FILENO, command, sizeof(command));

        // Line break delete
        if (command[command_length - 1] == '\n') {
            command_length--;
            command[command_length] = '\0';
        }

        // If nothing typed, display date
        if (command_length == 0) {
            displayDate();
        } else if (strcmp(command, "fortune") == 0) {
            // If the command is "fortune", display a fortune
            displayFortune();
        } else {
            // If the command is not recognized, proceed with the general command execution logic
            executeCommand(command);
        }
    }

    return 0;
}
