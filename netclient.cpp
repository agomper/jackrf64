#include "netclient.h"

NetClient::NetClient() {
    portNumber = 57160;
}

void NetClient::create_socket_connection() {
    //int socket(int domain, int type, int protocol)
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0) {
        perror("Socket creation failed \n");
        exit(1);
    }
}

void NetClient::init_isAddress(int mode) {
  isAddress.sin_family = AF_INET;
  isAddress.sin_port = htons(portNumber);

  if(mode == 1) { //Sender
      struct hostent *hostinfo = gethostbyname(hostname);
      if (hostinfo == NULL) {
          fprintf(stderr, "init_sockaddress: unknown host: %s.\n", hostname);
          exit(1);
      }
      else {
          isAddress.sin_addr = *(struct in_addr *) hostinfo->h_addr_list[0];
      }
  }
  else if (mode == 2) { //Receiver
      isAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  else {
      cout<<"Error! init_sockaddress() Mode no valid. \n";
      exit (1);
  }
}

void NetClient::bind_isAddress() {
    //int bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
    int err = bind(socketfd, (struct sockaddr *)&isAddress, sizeof(isAddress));
    if(err < 0) {
        perror("bind_inet: bind() failed");
        exit(1);
    }
}
