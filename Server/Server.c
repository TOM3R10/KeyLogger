#include "../HEADERS/Server.h"
server_t *server;
int clients_connected = SERVER_STARTING_CLIENTS_COUNT;


void handle_client_buffer(int socket_fd, client_t *client) {
    printf("Client %d Buffer: %s\n", socket_fd, client->buffer_array);
    printf("Save client's %d buffer to file? (1 for yes, 0 for no): ", socket_fd);
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        char filename[SERVER_MAX_FILE_NAME_LENGTH];
        snprintf(filename, sizeof(filename), "%s/client_%d_buffer.txt", SERVER_FOLDER_PATH, socket_fd);
        FILE *file = fopen(filename, "w");
        if (file) {
            fwrite(client->buffer_array, sizeof(char), client->keys, file);
            fclose(file);
            printf("Buffer saved to %s\n", filename);
        } else {
            perror("Failed to save buffer to file");
        }
    }
}


int add_char_to_buffer(client_t *client, char chr) {
	if (client->keys == 0) {
		client->buffer_array = (char *)malloc(sizeof(char));
		if (!client->buffer_array) {
			perror("Malloc failed");
			return -1;
		}
	} 
	else {
		char *temp = (char *)realloc(client->buffer_array, sizeof(char) * (client->keys + 1));
		if (!temp) {
			perror("Realloc failed");
			return -1;
		}
		client->buffer_array = temp;
	}
	client->buffer_array[client->keys] = chr;
	client->keys++;
	return 0;
}


void *pthread_handle_connection(void *args) {
	int exit_case = 1;
	int bytes_received;

	pthread_mutex_lock(&mutex_server_data_lock);

	args_t *arg = (args_t *)args;
	client_t *client = arg->client;
	int socket_fd = client->socket_fd;
	client->keys = SERVER_INIT_KEYS;

	pthread_mutex_unlock(&mutex_server_data_lock);
	
	char buffer[BUFFER_SIZE];
	while (exit_case) {
		bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0);
		if (bytes_received <= 0) break; // Check for errors or disconnection
		buffer[bytes_received] = '\0'; // Null-terminate the received data
		printf("Recv from %d: %s\n", socket_fd, buffer); // Print the received data

		// Checking for max keys in user buffer
		if (client->keys >= SERVER_MAX_KEYS_BUFFER) {
			// Reseting buffer and printing / storing
			handle_client_buffer(socket_fd, client);
			client->keys = 0;
			free(client->buffer_array);
		}

		add_char_to_buffer(client, buffer[0]);
	}

	printf("Client %d disconnected\n", socket_fd);

	if (client->buffer_array) {
		handle_client_buffer(socket_fd, client);
		free(client->buffer_array);
	}

    close(socket_fd);
    free(client);
    free(arg);

	pthread_mutex_lock(&mutex_array_lock);
	clients_connected--;
	pthread_mutex_unlock(&mutex_array_lock);

	return NULL;
}


