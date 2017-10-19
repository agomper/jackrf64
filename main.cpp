#include <unistd.h> /* POSIX */
#include <pthread.h>

#include "receiver.h"
#include "sender.h"

Sender senderObj;
Receiver recvObj;



void jack_client_shutdown(void *arg) {
  printf("Jack shutdown \n");
}

void jack_client_error_handler(const char *desc) {
  printf("Jack error: %s\n", desc);
}

int jack_callback_sender (jack_nframes_t nframes, void *arg){

}

int jack_callback_receiver (jack_nframes_t nframes, void *arg){
    if(nframes >= recvObj.getJackBufferSize()) {
        printf("jack-udp recv: JACK buffer size exceeds limit\n");
        return -1;
    }

    //Conversion del RingBuffer a Jack Ports
    //Le dice los punteros out que apunten al mismo sitio que los Jack ports.
    int i, j;
    float *out[2];
    for(i = 0; i < recvObj.getChannels(); i++) {
        out[i] = (float *) jack_port_get_buffer(recvObj.getJackPort(i), nframes);
    }

    //Comprueba si tiene informacion para el Jack
    int nsamples = nframes * recvObj.getChannels();
    int nbytes = nsamples * sizeof(float);
    int bytes_available = (int) jack_ringbuffer_read_space(recvObj.getRingBuffer());
    //Si no tiene suficiente informacion, reseta los punteros out.
    if(nbytes > bytes_available) {
        printf("jack-udp recv: buffer underflow (%d > %d)\n",
                nbytes, bytes_available);
        for(i = 0; i < nframes; i++) {
            for(j = 0; j < recvObj.getChannels(); j++) {
                out[j][i] = (float) 0.0;
            }
        }

    }
    //Pero si tiene suficiente informacion se la da a Jack ports, mediante
    //los punteros out.
    else {
        recvObj.jack_ringbuffer_read_exactly(nbytes);
        for(i = 0; i < nframes; i++) {
            for(j = 0; j < recvObj.getChannels(); j++) {
                out[j][i] = (float)
                        recvObj.getJackBuffer()[(i * recvObj.getChannels()) + j];
            }
        }
    }

    return 0;
}

//------------------------------MAIN------------------------------------//
int main () {
    int modeSelected;

    cout<<"Please select mode: 1. Sender - 2. Receiver \n";
    cin>>modeSelected;

    if (modeSelected==1) { //Sender
        cout<<"Sender mode selected. \n";
        senderObj.create_socket_connection();
        senderObj.init_isAddress(1);
        //senderObj.sender_socket_test();

        recvObj.open_jack_client("sender_client");
        //Sensibilidad de los mensajes de error a mostrar de Jack. Minimo.
        jack_set_error_function(jack_client_error_handler);
        //Register a function (and argument) to be called if and when the
        //JACK server shuts down the client thread
        jack_on_shutdown(senderObj.getClientfd(),
                         jack_client_shutdown, 0);
        //Tell the Jack server to call @a process_callback whenever there is
        //work be done, passing @a arg as the second argument.
        jack_set_process_callback(senderObj.getClientfd(), jack_callback_sender, 0);
    }
    else { //Receiver
        cout<<"Receiver mode selected. \n";
        recvObj.create_socket_connection();
        recvObj.init_isAddress(2);
        recvObj.bind_isAddress();
        //recvObj.receiver_socket_test();

        recvObj.open_jack_client("receiver_client");
        //Sensibilidad de los mensajes de error a mostrar de Jack. Minimo.
        jack_set_error_function(jack_client_error_handler);
        //Register a function (and argument) to be called if and when the
        //JACK server shuts down the client thread
        jack_on_shutdown(recvObj.getClientfd(),
                         jack_client_shutdown, 0);
        //Tell the Jack server to call @a process_callback whenever there is
        //work be done, passing @a arg as the second argument.
        jack_set_process_callback(recvObj.getClientfd(), jack_callback_receiver, 0);
    }


    //jack_set_process_callback (recvObj.getClientfd(), recv_get_local_mic, 0);;


}
