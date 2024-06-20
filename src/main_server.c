#include "../include/server_header.h"

const char *log_levels[] = {
    "FATAL",
    "INFO",
    "WARNING",
    "DEBUG"
};

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t addrlen = sizeof(server_address);

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
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen);
        if (client_socket < 0) {
            LOG(LOG_LEVEL_FATAL, "Accept Error!");
            exit(EXIT_FAILURE);
        }

        processCommand(client_socket);

        close(client_socket);
    }

    return 0;
}

