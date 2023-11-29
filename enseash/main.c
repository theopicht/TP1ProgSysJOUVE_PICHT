#include <unistd.h>

int main() {
    //Instead of using a printf, we're gonna use the read-write logic
    const char welcome_message[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
    write(STDOUT_FILENO, welcome_message, sizeof(welcome_message) - 1);

    const char prompt[] = "enseash % ";
    write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);

    while (1) {
        //We're gonna place all the REPL logic there
    }

    //test

    return 0;
}
