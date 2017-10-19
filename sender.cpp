#include "sender.h"


Sender::Sender() {
    strcpy(hostname, "127.0.0.1");
}

void Sender::sender_socket_test() {
    char my_message[30] = "This is a test message.";
    /* send a message to the server */
    if (sendto(socketfd, my_message, strlen(my_message),
               0, (struct sockaddr *)&isAddress, sizeof(isAddress)) < 0) {
        perror("Sender socket test failed. \n");
        exit(1);
    }
}



