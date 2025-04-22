#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>

// Used to get the "struct sockaddr_in"
#include <netinet/in.h>

// for close()
#include <unistd.h>

// for inet_addr(), htons()
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    // argc: holds the count of arguments
    // argv: an array of strings (char *'s), where each string represents an individual argument

    if (argc != 2) {
        printf("[Client] Please provide the IP address of the server as an argument\n");
        printf("[Client] This program should receive only one argument\n");
        return 1;
    } else {
        printf("[Client] IP provided by user: %s\n", argv[1]);
    }

    // Initialize our messages to the server
    char message_to_server[256] = "Client of Emilio O.G.";
    int num_from_user;
    printf("[Client] Please provide a number between 1 and 100: ");
    scanf("%d", &num_from_user);

    if (num_from_user < 1 || num_from_user > 100) {
        printf("[Client] Number needs to 1 <= x <= 100\n");
        printf("[Client] Exiting gracefully\n");
        return 0;
    }


    // create a socket
    // socket(int domain, int type, int protocol);
    // domain: specifies the communication domain (address family)
    //      AF_INET -> IPv4 | AF_INET6 -> IPv6
    // type: Specifies the type of communication the socket will use
    //      SOCK_STREAM -> used for connection-oriented communication stream (TCP)
    //      SOCK_DGRAM -> used for connectionless connection (UDP)
    // protocol: Specifies the protocol to use
    //      0 (which means the default protocol for the given address family and socket type).
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify an address for the socket
    // htons:  "host to network short". It is a function in C used to convert a 
    //      16-bit integer (short) from the host byte order to the network byte order.
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9314);
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    // returns an integer letting us know if the connection was successful or not
    int connection_status = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    // check for error with the connection
    if (connection_status == -1 ) {
        printf("[Client] There was an issue connecting to the remote socket\n");
        printf("[Client] closing client socket\n");
        close(network_socket);
        printf("[Client] client socket closed\n");
        printf("[Client] Exiting gracefully\n");
        return 0;
    }


    // send string to server
    send(network_socket, message_to_server, sizeof(message_to_server), 0);
    printf("[Client] sent char[256] message to server: \"%s\"\n", message_to_server);
    
    // send num to server
    num_from_user = htonl(num_from_user);
    send(network_socket, &num_from_user, sizeof(num_from_user), 0);
    printf("[Client] sent integer message to server: %d\n", ntohl(num_from_user));


    // receive data from the server
    char message_from_server[256];
    recv(network_socket, &message_from_server, sizeof(message_from_server), 0);
    message_from_server[255] = '\0';
    printf("[Client] received char[256] from server: \"%s\"\n", message_from_server);

    int num_from_server;
    recv(network_socket, &num_from_server, sizeof(num_from_server), 0);
    num_from_server = ntohl(num_from_server);
    printf("[Client] received integer from server: %d\n", num_from_server);


    // print out the server's response
    printf("[Client] sum of integers: %d\n", (ntohl(num_from_user) + num_from_server));


    // close socket after done receiving
    printf("[Client] closing client socket\n");
    close(network_socket);
    printf("[Client] client socket closed\n");
    printf("[Client] Exiting gracefully\n");

    return 0;
}