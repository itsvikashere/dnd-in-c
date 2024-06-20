#include "../include/server_header.h"

extern const char *log_levels[];

void openFile(int client_socket, int client_id, char choice, char global, char num_groups[]) {
    FILE *csv_file;
    csv_file = fopen("data.csv", "r+"); // opening file for r, w, a
    // Check if file opened successfully
    if (csv_file == NULL) {
        LOG(LOG_LEVEL_FATAL, "Error opening file!");
        exit(1);
    }

    // Check if the client is new
    bool isNewClient = true;
    int stored_client_id;
    rewind(csv_file); // Move file pointer to the beginning
    int id;
    char c, g;
    char num[50];
    char line[256]; // Assuming the maximum line length won't exceed 256 characters

    while (fgets(line, sizeof(line), csv_file)) {
        int len = strlen(line);
        char *token = strtok(line, ",");
        if (token != NULL) {
            // Extract and store client_id
            id = atoi(token);
            if (id == client_id) {
                isNewClient = false;
                token = strtok(NULL, ",");
                if (token != NULL) {
                    // Extract and store choice
                    c = token[0];
                    if (choice == c) {
                        warningToClient(client_socket, c, choice);
                        break;
                    } else {
                        updateFile(client_socket, csv_file, len, c, client_id, choice, global, num_groups);
                        break;
                    }
                }
            }
        }
    }
    if (isNewClient) {
        newClient(client_socket, csv_file, client_id, choice, global, num_groups);
    }
    fclose(csv_file); // file closed
}

