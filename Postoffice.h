#ifndef _postoffice_h
#define _postoffice_h

class postoffice {
    int socketid;
    bool direction;
    struct addrinfo *server_info;

    bool decision;

    int createTxSocket(bool broadcast, const char* ip, const char* port);
    int createRxSocket(bool broadcast, const char* port);
    int createSocket(const char* ip, const char* port);
    int createSocketCommon(const char* ip, const char* port, addrinfo* readable_server_info);

public:

    postoffice(const char* port, const char* ip);
    postoffice(const char* port);

    int closeConnection();
    int send(const void *message, int size_of_message);
    int receive(void* bufferptr, int size);
    int isValid();

};

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

serial_data frank(stamp*, serial_data);
serial_data unfrank(serial_data, stamp*);

#endif
