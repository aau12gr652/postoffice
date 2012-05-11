#ifndef _postoffice_h
#define _postoffice_h

#include <cstdlib>
#include <stdint.h>
#include <boost/thread.hpp>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <fstream>

struct stamp {
    uint8_t Generation_ID;
    uint8_t Number_Of_Layers;
    uint8_t Layer_ID;
    uint8_t Field_Size; // 2 to the power of N, e.g. binary field identified by 1.
    uint16_t Symbol_Size;
    uint16_t Generation_Size;
    uint16_t Layer_Size;
};

struct serial_data {
    int size;
    void* data;
};

class postoffice {
    int socketid;
    bool direction;
    struct addrinfo *server_info;
    bool decision;
    int timeout;

    int runThread;
    boost::mutex dataLock;
    boost::thread* receivingThread;
    void receiveThread(void);
    std::list<serial_data> receivedData;

    int createTxSocket(bool broadcast, const char* ip, const char* port);
    int createRxSocket(bool broadcast, const char* port);
    int createSocket(const char* ip, const char* port);
    int createSocketCommon(const char* ip, const char* port, addrinfo* readable_server_info);
    int sendLetter(serial_data);
    int receiveLetter(serial_data);
    serial_data frank(stamp*, serial_data);
    int unfrank(serial_data letter, stamp* header, void* bufferptr);

public:

    postoffice(const char* port, const char* ip); // Transmistter
    postoffice(const char* port, int timeOut); // Receiver

    int closeConnection();
    int isValid();

    int send(void*, int, stamp*);
    int send(serial_data, stamp*);

    int receive(void*, stamp*);
    void startThread(void);
    void stopThread(void);
};

#define TX_DELAY 750

#define TX 0
#define RX 1

#define NO_ERROR 0
#define CONNECTION_CLOSE_ERROR -100
#define SOCKET_ID_NOT_VALID -101
#define SEND_ERROR -102
#define RECEIVE_ERROR -103
#define SIZE_ERROR -104
#define GET_ADDR_INFO_ERROR -105
#define SET_SOCKET_OPTION_ERROR -106
#define SOCKET_CREATION_ERROR -107
#define BINDING_ERROR -108

uint8_t* devRandom(int count = 1);
void print_stamp(stamp*);

#endif
