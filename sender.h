#ifndef SENDER_H
#define SENDER_H
#include "jackclient.h"
#include "netclient.h"

class Sender : public JackClient, public NetClient{

public:
    Sender();
    void sender_socket_test();
};

#endif // SENDER_H
