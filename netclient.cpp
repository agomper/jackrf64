#include "netclient.h"

int NetClient::getSrSocketFD() const
{
    return srSocketFD;
}

sockaddr_in NetClient::getSrISAddress()
{
    return srISAddress;
}

int NetClient::getFpgaSocketFD() const
{
    return fpgaSocketFD;
}

NetClient::NetClient() {

}

int NetClient::getPayloadBytes() const
{
    return payloadBytes;
}

int NetClient::getPayloadSamples() const
{
    return payloadSamples;
}

void NetClient::packet_sendto(networkPacket *p, int socket, sockaddr_in address) {
  packet_hton(p); //Network byte order
  sendto_exactly(socket, (unsigned char *)p, sizeof(networkPacket), address);
}

void NetClient::packet_recvfrom(networkPacket *p, int socket) {
  recv_exactly(socket, (char *)p, sizeof(networkPacket), 0);
  packet_ntoh(p); //Network byte order
}

//Network byte order
void NetClient::packet_hton(networkPacket *p)
{
  u_int8_t *d = p->data;
  int i = p->channels * p->frames;
  while(i--){
    hton32_buf(d, d);
    d += 4;
  }
  p->index = hton_i32(p->index);
  p->channels = hton_i16(p->channels);
  p->frames = hton_i16(p->frames);
}

//Network byte order
void NetClient::packet_ntoh(networkPacket *p)
{
  p->index = ntoh_i32(p->index);
  p->channels = ntoh_i16(p->channels);
  p->frames = ntoh_i16(p->frames);
  u8 *d = p->data;
  u32 i = p->channels * p->frames;
  while(i--) {
    ntoh32_buf(d, d);
    d += 4;
  }
}

void NetClient::sendto_exactly(int fd, const u8 *data, int n, struct sockaddr_in address)
{
  int err = xsendto(fd, data, n, 0, (struct sockaddr *)&address, sizeof(address));
  if(err != n) {
    fprintf(stderr, "sendto_exactly: partial write\n");
    exit(1);
  }
}

void NetClient::recv_exactly(int fd, void *buf, size_t n, int flags)
{
  int err = (int) xrecv(fd, buf, n, flags);
  if(err != n) {
    fprintf(stderr, "recv_exactly: partial recv (%d != %ul)\n", err, (int) n);
    exit(1);
  }
}

int NetClient::xsendto(int fd, const void *data, size_t n, int flags, struct sockaddr *addr, socklen_t length)
{
  int err = sendto(fd, data, n, flags, addr, length);
  if(err < 0) {
    perror("sendto() failed");
    exit(1);
  }
  return err;
}

int NetClient::xrecv(int fd, void *buf, size_t n, int flags)
{
  int err = recv(fd, buf, n, flags);
  if(err < 0) {
    perror("recv() failed");
    exit(1);
  }
  return err;
}
