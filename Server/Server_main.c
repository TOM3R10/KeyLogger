#include "../HEADERS/Server.h"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // init server
    server = (server_t *)malloc(sizeof(server_t));
    memset(server, 0, sizeof(server_t)); // Initialize server data

    // Initialize mutexes
    pthread_mutex_init(&mutex_array_lock, NULL);
    pthread_mutex_init(&mutex_server_data_lock, NULL);

    // Create a socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Setup the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to bind");
        close(server_fd);
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Failed to listen");
        close(server_fd);
        return 1;
    }

    printf("Waiting for a connection\n");
    
    while (clients_connected <= SERVER_MAX_CLIENTS) {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("Failed to accept connection");
            continue;
        }

        printf("[SERVER] Connection captured\n");

        // Init a new client struct
        client_t *client = (client_t *)malloc(sizeof(client_t));
        if (!client) {
            perror("Malloc failed");
            close(client_fd);
            continue;
        }

        client->socket_fd = client_fd;

        args_t *args = (args_t *)malloc(sizeof(args_t));
        if (!args) {
            perror("Malloc failed");
            free(client);
            close(client_fd);
            continue;
        }

        args->client = client;


	// Add the client to the server data
        pthread_mutex_lock(&mutex_array_lock);
        for (int i = 0; i < SERVER_MAX_CLIENTS; i++) {
            if (server->clients_arr[i] == NULL) {
                server->clients_arr[i] = client;
                break;
            }
        }
        clients_connected++;
        pthread_mutex_unlock(&mutex_array_lock);


        pthread_t handle_connection;
        if (pthread_create(&handle_connection, NULL, pthread_handle_connection, args) != 0) {
		perror("Failed to create thread");
		free(client);
		free(args);
		close(client_fd);
        } 
	else {
		pthread_detach(handle_connection); // Detach the thread
		pthread_mutex_lock(&mutex_array_lock);
		clients_connected++;
		pthread_mutex_unlock(&mutex_array_lock);
        }
    }

    close(server_fd);
    pthread_mutex_destroy(&mutex_array_lock);
    pthread_mutex_destroy(&mutex_server_data_lock);

    return 0;
}

