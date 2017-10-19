#include "jackclient.h"

jack_client_t *JackClient::getClientfd() const
{
    return clientfd;
}

void JackClient::setClientfd(jack_client_t *value)
{
    clientfd = value;
}

int JackClient::getJackBufferSize() const
{
    return jackBufferSize;
}

int JackClient::getChannels() const
{
    return channels;
}

jack_ringbuffer_t *JackClient::getRingBuffer() const
{
    return ringBuffer;
}

jack_port_t *JackClient::getJackPort(int index)
{
    return jackPort[index];
}

float *JackClient::getJackBuffer() const
{
    return jackBuffer;
}

JackClient::JackClient() {
    options = JackNoStartServer; //No arranca JACK si no lo estaba.
    //JackNullOption; //Ninguna condicion a Jack Server, solo arranca.
    channels = 2;
    //Total = Num frames * Num channels * 32 bits
    jackBufferSize *= channels * sizeof(float);
    //xmalloc(). The motto is succeed or die. If it fails to allocate memory,
    //it will terminate your program and print an error message
    jackBuffer = (float *) malloc(jackBufferSize);
    //Allocates a ringbuffer data structure of a specified size.
    ringBuffer = jack_ringbuffer_create(jackBufferSize);
    //The pipe is then used for communication either between the parent or child
    //processes, or between two sibling processes.
    int err = pipe(pipeAux);
    if(err) {
      perror("pipe() failed");
      exit(1);
    }
}


jack_client_t *JackClient::open_jack_client(char *name) {
    this->clientName = name;

    /* open a client connection to the JACK server */
    clientfd = jack_client_open (clientName, options, &status);

    if (clientfd == NULL){
        //If connection failed, say why
        printf ("open_jack_client() failed, status = 0x%2.0x\n", status);
        if (status & JackServerFailed) {
            printf ("Unable to connect to JACK server.\n");
        }
        exit (1);
    }

    /* if connection was successful, check if the name we proposed is not in use */
    if (status & JackNameNotUnique){
        clientName = jack_get_client_name(clientfd);
        printf ("Warning: other agent with our name is "
                "running, `%s' has been assigned to us.\n", clientName);
    }

    /* display the current sample rate. */
    sampleRate = (double)jack_get_sample_rate(clientfd);
    printf ("Engine sample rate: %0.0f\n", sampleRate);

    return clientfd;
}

void JackClient::jack_ringbuffer_read_exactly(int nBytesToRead)
{
  int BytesRead = jack_ringbuffer_read(ringBuffer,(char *) jackBuffer, nBytesToRead);
  if(BytesRead != nBytesToRead) {
    cout<<"Error reading RingBuffer. Required = "<<nBytesToRead<<
          " Required= "<<BytesRead<<" \n.";
    exit(1);
  }
}
