#ifndef RECEIVER_H
#define RECEIVER_H
#include "jackclient.h"
#include "netclient.h"

class Receiver : public JackClient, public NetClient{

public:
    Receiver();
    void receiver_socket_test();
};

#endif // RECEIVER_H

