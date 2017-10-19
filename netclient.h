#ifndef NETCLIENT_H
#define NETCLIENT_H
//Network
#include <netdb.h>
#include <sys/socket.h>
//Comunes
#include "jackclient.h"

class NetClient {
protected:
    sockaddr_in isAddress;            //Internet socket address
    int socketfd;                     //Socket File descriptor
    int portNumber;                   //Puerto comunicacion.
    char hostname[20];
public:
    NetClient();
    void create_socket_connection();
    void init_isAddress(int mode);
    void bind_isAddress();
};

#endif // NETCLIENT_H
