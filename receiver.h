#ifndef RECEIVER_H
#define RECEIVER_H
#include "jackclient.h"
#include "netclient.h"
#include "soundfile.h"

class Receiver : public JackClient, public NetClient, public SoundFile{
    int srPortNumber;                   //Puerto comunicacion.
public:
    Receiver();
    void create_sr_socket_connection();
    void bind_sr_ISAddress();
    void receiver_socket_test();
    void finish();
};

#endif // RECEIVER_H

