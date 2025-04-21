#include <stdlib.h>
#include <stdio.h>

int main() {

    int result;
    result = system("gcc tcp_client.c -o client");
    if (result != 0) {
        printf("Error compiling CLIENT\n");
        return 1;
    }

    result = system("gcc tcp_server.c -o server");
    if (result != 0) {
        printf("Error compiling SERVER\n");
        return 1;
    }

    return 0;
}