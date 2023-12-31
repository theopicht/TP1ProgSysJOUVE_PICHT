// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>

// Constants
#define EXCLUDE_NULL_CHAR 1 // Define EXCLUDE_NULL_CHAR to exclude the null char at the end of the prompt.
#define LINE_BREAK_DELETE_OFFSET 1 // Define LINE_BREAK_DELETE_OFFSET to remove the line break.

#define MAX_COMMAND_LENGTH 100 // Define the maximal size of a command.
#define MAX_ARG_SIZE 20 // Define the maximal size of arguments.
#define MAX_STATUS_BUFFER_SIZE 512 // Define the maximum statusBuffer size.

#define PERMISSION_DEFINITION 0666 // Define the read and write permission for each user on the system

#define TV_SEC_CONVERTER 1000 // Conversion second to millisecond.
#define TV_NSEC_CONVERTER 1000000 // Conversion nanosecond to millisecond.

void displayWelcomeMessage() {
    const char welcome_message[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";

    // Using of sizeof(welcome_message) to determinate the size of the prompt.
    write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - EXCLUDE_NULL_CHAR);
}

void executeCommand(const char *command) {
    // Create a child process
    pid_t pid = fork();
    int status;
    // Prompt buffer
    char statusBuffer[MAX_STATUS_BUFFER_SIZE];

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // If the process is a child process
    if (pid == 0) {
        // Split the command into tokens
        char *args[MAX_ARG_SIZE];
        int arg_count = 0;

        char *token = strtok((char *)command, " ");
        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL; // Null-terminate the array

        // Check for input and output redirection
        for (int i = 0; i < arg_count; ++i) {
            if (strcmp(args[i], "<") == 0) {
                // Input redirection
                int fd_in = open(args[i + 1], O_RDONLY); // Open the file specified for input redirection in read-only mode
                if (fd_in == -1) {
                    // Error handler
                    perror("Error opening input file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_in, STDIN_FILENO); // Duplicate the file descriptor for input to stdin
                close(fd_in); // Close the original file descriptor
                args[i] = NULL; // Remove '<' from arguments to exclude it from command execution
            } else if (strcmp(args[i], ">") == 0) {
                // Output redirection
                int fd_out = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, PERMISSION_DEFINITION); // Open the file for output redirection with write-only, create, and truncate flags
                if (fd_out == -1) {
                    // Error handler
                    perror("Error opening output file");
                    exit(EXIT_FAILURE);
                }
                dup2(fd_out, STDOUT_FILENO); // Duplicate the file descriptor for output to stdout
                close(fd_out); // Close the original file descriptor
                args[i] = NULL; // Remove '>' from arguments to exclude it from command execution
            }
        }

        // Execute the command with arguments
        execvp(args[0], args);

        // If execvp returns, there was an error
        perror("Error while executing command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // If the process is a parent process
        // Wait for the child process to finish and get the exit status
        waitpid(pid, &status, 0);

        clock_gettime(CLOCK_MONOTONIC, &end_time);

        // Calculate the execution time in milliseconds
        long execution_time = (end_time.tv_sec - start_time.tv_sec) * TV_SEC_CONVERTER +
                              (end_time.tv_nsec - start_time.tv_nsec) / TV_NSEC_CONVERTER;

        // Print the prompt with the exit status or the signal information
        if (WIFEXITED(status)){ // If the process exited normally
            sprintf(statusBuffer, "enseash [exit:%d|%ld ms] %% ", WEXITSTATUS(status), execution_time);
            write(STDIN_FILENO, statusBuffer, strlen(statusBuffer));
        } else if(WIFSIGNALED(status)){ // If the process was killed by a signal
            sprintf(statusBuffer, "enseash [sign:%d|%ld ms] %% ", WTERMSIG(status), execution_time);
            write(STDIN_FILENO, statusBuffer, strlen(statusBuffer));
        }
    } else { // Fork failed
        perror("Error while creating child process");
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

// The main function is the entry point of the program. It is called when the program is executed.
int main() {
    // Instead of using a printf, we are going to use the read-write logic

    // Display the welcome message
    displayWelcomeMessage();

    // Define the prompt and the command
    const char prompt[] = "enseash % ";
    char command[MAX_COMMAND_LENGTH];

    // Display the prompt
    write(STDOUT_FILENO, prompt, sizeof(prompt) - EXCLUDE_NULL_CHAR);
    while (1) {
        // We are going to write all the REPL logic there

        // Reading of the typed command
        ssize_t command_length = read(STDIN_FILENO, command, sizeof(command));

        // Check for end of file (Ctrl+D to exit)
        if (command_length == 0) {
            // If the command is "exit", display a bye bye message and exit the program
            const char exit_message[] = "Bye bye...";
            write(STDOUT_FILENO, exit_message, sizeof(exit_message) - EXCLUDE_NULL_CHAR);
            break;
        }

        // Line break delete
        if (command[command_length - LINE_BREAK_DELETE_OFFSET] == '\n') {
            command_length--;
            command[command_length] = '\0';
        }

        // Exit if the command is "exit"
        if (strcmp(command, "exit") == 0) {
            // If the command is "exit", display a bye bye message and exit the program
            const char exit_message[] = "Bye bye...";
            write(STDOUT_FILENO, exit_message, sizeof(exit_message) - EXCLUDE_NULL_CHAR);
            break;
        }

        // If nothing typed, display date
        if (command_length == 0 || strcmp(command, "date") == 0) {
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