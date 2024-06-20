#include "../include/server_header.h"

const char *log_levels[] = {
    "FATAL",
    "INFO",
    "WARNING",
    "DEBUG"
};

void *handleClient(void *arg) {
    int client_socket = *((int *)arg);
    free(arg);  // Free the dynamically allocated memory for client socket
    processCommand(client_socket);
    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(client_address);

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOG(LOG_LEVEL_FATAL, "Socket Failed!");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        LOG(LOG_LEVEL_FATAL, "Bind Failed!");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        LOG(LOG_LEVEL_FATAL, "Listen Failed!");
        exit(EXIT_FAILURE);
    }

    LOG(LOG_LEVEL_INFO, "Server listening on port %d...", PORT);

    while (1) {
        int *client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen)) < 0) {
            LOG(LOG_LEVEL_FATAL, "Accept Error!");
            exit(EXIT_FAILURE);
        }

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handleClient, (void *)client_socket) != 0) {
            LOG(LOG_LEVEL_FATAL, "Failed to create thread");
            free(client_socket);  // Free memory in case of thread creation failure
            close(*client_socket);
        }
        pthread_detach(thread_id);  // Detach thread to free resources when done
    }

    close(server_socket);
    return 0;
}

