#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define EXCLUDE_NULL_CHAR 1 // Define EXCLUDE_NULL_CHAR to exclude the null char at the end of the prompt.
#define MAX_COMMAND_LENGTH 100 // Define the maximal size of a command

int main() {

    // Instead of using a printf, we're gonna use the read-write logic
    const char welcome_message[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
    const char prompt[] = "enseash % ";

    // Using of sizeof(welcome_message) to determinate the size of the prompt.
    write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - EXCLUDE_NULL_CHAR);

    char command[MAX_COMMAND_LENGTH];

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

        // If nothing typed
        if (command_length == 0) {
            pid_t pid = fork();

            if (pid == 0) { // Child process
                execlp("date", "date", NULL);
                // If execlp returns, there was an error
                write(STDOUT_FILENO, "Error executing date\n", strlen("Error executing date\n"));
                exit(EXIT_FAILURE);
            }

            else if (pid > 0) { // Parent process
                // Wait for the child to finish
                waitpid(pid, NULL, 0);
            }

            else { // Fork failed
                write(STDOUT_FILENO, "Error creating child process\n", strlen("Error creating child process\n"));
                exit(EXIT_FAILURE);
            }
        }

        // If the command is "fortune"
        else if (strcmp(command, "fortune") == 0) {
            pid_t pid = fork();

            // Child process
            if (pid == 0) {
                execlp("fortune", "fortune", NULL);
                // If execlp returns, there was an error
                write(STDOUT_FILENO, "Error executing fortune\n", strlen("Error executing fortune\n"));
                exit(EXIT_FAILURE);
            }

            // Parent process
            else if (pid > 0) {
                waitpid(pid, NULL, 0);
            }

            // Fork failed
            else {
                write(STDOUT_FILENO, "Error creating child process\n", strlen("Error creating child process\n"));
                exit(EXIT_FAILURE);
            }
        }

        // If the command is not recognized, proceed with the general command execution logic
        else {
            pid_t pid = fork();

            if (pid == 0) { // Child process
                execlp(command, command, NULL);
                // If execlp returns, there was an error
                write(STDOUT_FILENO, "Command not found\n", strlen("Command not found\n"));
                exit(EXIT_FAILURE);
            } else if (pid > 0) { // Parent process
                // Wait for the child to finish
                waitpid(pid, NULL, 0);
            } else { // Fork failed
                write(STDOUT_FILENO, "Error creating child process\n", strlen("Error creating child process\n"));
                exit(EXIT_FAILURE);
            }
        }
    }
    return 0;
}
