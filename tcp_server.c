#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

// for close()
#include <unistd.h>

void closeSockets();

int main() {

    // Message to send
    char server_message[256] = "Server of Emilio O.G.";
    int num_to_send = 94;
    num_to_send = htonl(num_to_send);
    
    // create the server socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9314);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    int listen_result = listen(server_socket, 5);
    if (listen_result == 0) {
        printf("[Server] listening on port 9314\n");
    } else {
        printf("[Server] listen failed\n");
        close(server_socket);
        printf("[Server] exiting gracefully\n");
        return 0;
    }

    // accept connection
    // file descriptor assigned to client socket
    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);
    printf("[Server] connection accepted\n");
    
    // receive string data from client
    char message_from_client[256];
    recv(client_socket, message_from_client, sizeof(message_from_client), 0);
    printf("[Server] received char[256] from client: %s\n", message_from_client);
    printf("[Server] Connection [ %s | Server of Emilio O.G. ]\n", message_from_client);

    // receive integer data from client
    int num_from_client;
    recv(client_socket, &num_from_client, sizeof(num_from_client), 0);
    num_from_client = ntohl(num_from_client);
    printf("[Server] received integer from client: %d\n", num_from_client);

    if (num_from_client < 1 || num_from_client > 100) {
        printf("[Server] received an integer outside the range of 1 to 100: %d\n", num_from_client);
        printf("[Server] closing server socket\n");
        close(server_socket);
        printf("[Server] server closing socket\n");
        printf("[Server] Exiting gracefully\n");
        return 0;
    }

    printf("[Server] current integers [ Client %d | Server %d ]\n", num_from_client, ntohl(num_to_send));

    // send char[256] message to client socket
    send(client_socket, server_message, sizeof(server_message), 0);
    printf("[Server] sent char[256] message to client: \"%s\"\n", server_message);

    // send int message to client socket
    send(client_socket, &num_to_send, sizeof(num_to_send), 0);
    printf("[Server] sent integer message to client: %d\n", ntohl(num_to_send));


    // Printing sum
    printf("[Server] sum of integers: %d\n", (ntohl(num_to_send) + num_from_client));


    // close socket now that we're done
    printf("[Server] closing server socket\n");
    close(server_socket);
    printf("[Server] server closing socket\n");
    printf("[Server] Exiting gracefully\n");
    return 0;
}

