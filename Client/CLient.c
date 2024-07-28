#include "../HEADERS/CLient.h"

volatile sig_atomic_t exit_case = 1;

void handle_sigint(int sig) {
    exit_case = 0;
}
