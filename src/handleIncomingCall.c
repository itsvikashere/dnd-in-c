#include "../include/server_header.h"
extern const char *log_levels[];

void handleIncomingCall(int client_socket,int my_id, int client_id) {
    FILE *csv_file;
    csv_file = fopen("data.csv", "r");
    if (csv_file == NULL) {
        LOG(LOG_LEVEL_FATAL, "Error opening file!\n");
        exit(1);
    }
    
    char line[256]; // Assuming the maximum line length won't exceed 256 characters
    while (fgets(line, sizeof(line), csv_file)) {
        char *token = strtok(line, ",");
        if (token != NULL) {
            // Extract and store client_id
            int stored_client_id = atoi(token);
            if (stored_client_id == client_id) {
                token = strtok(NULL, ",");
                if (token != NULL && token[0] == 'A') {
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        if (token[0] == 'S') {
                            // Selective DND mode
                            token = strtok(NULL, ",");
                            while (token != NULL) {
                                if (isdigit(token[0])) {
                                    int blocked_id = atoi(token);
                                    if (my_id == blocked_id) {
                                        sendToClient(client_socket, "Client is in DND mode - Calls are Blocked\n");
                                        fclose(csv_file);
                                        return;
                                    }
                                }
                                token = strtok(NULL, ",");
                            }
                            // If client_id is not in the selective block list
                            sendToClient(client_socket, "Incoming calls allowed\n");
                            fclose(csv_file);
                            return;
                        } else if (token[0] == 'G') {
                            // Global DND mode
                            sendToClient(client_socket, "Client is in DND mode - Calls are Blocked\n");
                            fclose(csv_file);
                            return;
                        }
                    }
                }
                // If 'A' is not followed by 'S' or 'G'
                sendToClient(client_socket, "Incoming calls allowed\n");
                fclose(csv_file);
                return;
            }
        }
    }
    sendToClient(client_socket, "This id is not registered\n");
    fclose(csv_file);
}

