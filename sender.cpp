#include "sender.h"


Sender::Sender() {
    strcpy(srHostName, "127.0.0.1");
    srPortNumber = 57160;
    soundFileName = "Suspiros.wav";

    srISAddress.sin_family = AF_INET;
    srISAddress.sin_port = htons(srPortNumber);

    struct hostent *hostinfo = gethostbyname(srHostName);
    if (hostinfo == NULL) {
        cout<<"Unknown SR Host. \n";
        exit(1);
    }
    else {
        srISAddress.sin_addr = *(struct in_addr *) hostinfo->h_addr_list[0];
    }
    //******FPGA********//
    fpgaPortNumber = 57150;

    fpgaISAddress.sin_family = AF_INET;
    fpgaISAddress.sin_port = htons(fpgaPortNumber);
    fpgaISAddress.sin_addr.s_addr = htonl(INADDR_ANY);
}

void Sender::create_sr_socket_connection() {
    //int socket(int domain, int type, int protocol)
    srSocketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(srSocketFD < 0) {
        cout<<"Sender/Receiver socket creation failed. \n";
        exit(1);
    }
}

void Sender::create_fpga_socket_connection() {
    //int socket(int domain, int type, int protocol)
    fpgaSocketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if(fpgaSocketFD < 0) {
        cout<<"FPGA/Sender socket creation failed. \n";
        exit(1);
    }
}

void Sender::bind_fpga_ISAddress()
{
    //int bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    int code = bind(fpgaSocketFD,
                    (struct sockaddr *)&fpgaISAddress, sizeof(fpgaISAddress));
    if(code < 0) {
        cout<<("Receiver could not bind the fpgaAddress. \n");
        exit(1);
    }
    cout<<"Waiting on port: "<<fpgaPortNumber<<endl;
}

void Sender::sender_socket_test() {
    char my_message[30] = "This is a test message.";
    /* send a message to the server */
    if (sendto(srSocketFD, my_message, strlen(my_message),
               0, (struct sockaddr *)&srISAddress, sizeof(srISAddress)) < 0) {
        perror("Sender socket test failed. \n");
        exit(1);
    }
}

void Sender::finish() {
    close(srSocketFD);
    jack_ringbuffer_free(ringBuffer);
    jack_client_close(clientfd);
    close(comPipe[0]);
    close(comPipe[1]);
    free(jackBuffer);
}

