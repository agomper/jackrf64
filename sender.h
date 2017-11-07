#ifndef SENDER_H
#define SENDER_H
#include "jackclient.h"
#include "netclient.h"

class Sender : public JackClient, public NetClient, public SoundFile{
    int srPortNumber;                   //Puerto comunicacion.
    int fpgaPortNumber;
    char srHostName[20];                //Host IP address
public:
    Sender();
    void create_sr_socket_connection();
    void create_fpga_socket_connection();
    void bind_fpga_ISAddress();
    void sender_socket_test();
    void finish();
};

#endif // SENDER_H
