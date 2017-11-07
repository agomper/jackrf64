#include "receiver.h"

Receiver::Receiver() {
    srPortNumber = 57160;

    srISAddress.sin_family = AF_INET;
    srISAddress.sin_port = htons(srPortNumber);
    srISAddress.sin_addr.s_addr = htonl(INADDR_ANY);
}

void Receiver::create_sr_socket_connection() {
    //int socket(int domain, int type, int protocol)
    srSocketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(srSocketFD < 0) {
        cout<<"Receiver socket creation failed. \n";
        exit(1);
    }
}

void Receiver::bind_sr_ISAddress() {
    //int bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    int code = bind(srSocketFD, (struct sockaddr *)&srISAddress, sizeof(srISAddress));
    if(code < 0) {
        cout<<("Receiver could not bind the ISAddress. \n");
        exit(1);
    }
    cout<<"Waiting on port: "<<srPortNumber<<endl;
}


void Receiver::receiver_socket_test()
{
    unsigned char buf[2048];
    socklen_t addrlen = sizeof(srISAddress);
    int recvlen;            /* # bytes received */

    /* now loop, receiving data and printing what we received */
    while(1) {
        printf("waiting on port %d\n", srPortNumber);
        recvlen = recvfrom(srSocketFD, buf, 2048,
                           0, (struct sockaddr *)&srISAddress, &addrlen);
        printf("received %d bytes\n", recvlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            printf("received message: \"%s\"\n", buf);
        }
    }
    /* never exits */
}

void Receiver::finish()
{
    cout<<"Cerrando y liberando. \n";
    close(srSocketFD);
    jack_ringbuffer_free(ringBuffer);
    jack_client_close(clientfd);
    close(comPipe[0]);
    close(comPipe[1]);
    free(jackBuffer);
}



