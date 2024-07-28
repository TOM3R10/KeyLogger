#include "../HEADERS/CLient.h"
int main() {
    int fd, sock_fd;
    char user_buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;

    // Handle SIGINT for graceful exit
    signal(SIGINT, handle_sigint);

    // Create a socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Failed to create socket");
        return 1;
    }

    // Setup the server address struct
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(sock_fd);
        return 1;
    }

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to the server failed");
        close(sock_fd);
        return 1;
    }

    // Open the device file
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        close(sock_fd);
        return 1;
    }

    // Continuous reading and sending
    while (exit_case) {
	usleep(CLIENT_SLEEP_TIME);
        // Read from the device file
        ssize_t bytesRead = read(fd, user_buffer, BUFFER_SIZE);
        if (bytesRead < 0) {
            perror("Failed to read from device");
            exit_case = 0;
        }

        if (bytesRead > 0) {
            // Send data through the socket
            ssize_t bytesSent = send(sock_fd, user_buffer, bytesRead, 0);
            if (bytesSent < 0) {
                perror("Failed to send data to server");
                exit_case = 0;
            }
        }

        // Clear the buffer
        memset(user_buffer, 0, BUFFER_SIZE);
    }

    // Close the device file and socket
    close(fd);
    close(sock_fd);

    return 0;
}
