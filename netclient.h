#ifndef NETCLIENT_H
#define NETCLIENT_H
//Network
#include <netdb.h>
#include <sys/socket.h>
extern "C" {
#include "c-common/byte-order.h"
}
//Comunes
#include "jackclient.h"
#include <stdint.h>

typedef struct {
  uint32_t index;                //Identificador del paquete
  uint16_t channels;             //Num channels
  uint16_t frames;               //Num frames
  uint8_t data[1024];            //Payload
} networkPacket;

class NetClient {
protected:
    sockaddr_in srISAddress;            //Internet socket address
    sockaddr_in fpgaISAddress;            //Internet socket address
    int srSocketFD;                     //Socket File descriptor
    int fpgaSocketFD;
    int payloadSamples = 256;
    int payloadBytes =  payloadSamples*sizeof(float);
public:
    NetClient();
    int getPayloadBytes() const;
    int getPayloadSamples() const;
    int getSrSocketFD() const;
    int getFpgaSocketFD() const;
    void packet_sendto(networkPacket *p, int socket, sockaddr_in address);
    void packet_recvfrom(networkPacket *p, int socket);
    void packet_hton(networkPacket *p);
    void packet_ntoh(networkPacket *p);
    void sendto_exactly(int fd, const u8 *data, int n, struct sockaddr_in address);
    void recv_exactly(int fd, void *buf, size_t n, int flags);
    int xsendto(int fd, const void *data, size_t n, int flags, struct sockaddr *addr, socklen_t length);
    int xrecv(int fd, void *buf, size_t n, int flags);
    sockaddr_in getSrISAddress();
};

#endif // NETCLIENT_H
