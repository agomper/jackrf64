#ifndef NETCLIENT_H
#define NETCLIENT_H
//Network
#include <netdb.h>
#include <sys/socket.h>
//Comunes
#include "jackclient.h"
#include <stdint.h>

typedef struct {
  int index;                //Identificador del paquete
  uint16_t channels;             //Num channels
  uint16_t frames;               //Num frames
  uint8_t data[1];   //Payload
} networkPacket;

class NetClient {
protected:
    sockaddr_in isAddress;            //Internet socket address
    int socketfd;                     //Socket File descriptor
    int portNumber;                   //Puerto comunicacion.
    char hostname[20];
    int payloadSamples = 256;
    int payloadBytes =  payloadSamples*sizeof(float);
public:
    NetClient();
    void create_socket_connection();
    void init_isAddress(int mode);
    void bind_isAddress();
    networkPacket getP() const;
    void setP(const networkPacket &value);
};

#endif // NETCLIENT_H
