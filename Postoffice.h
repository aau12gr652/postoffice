//  Postoffice.h
//  Network module for rx/tx via UDP
//
//  Created by Benjamin Krebs on 16/04/12.

//

namespace Postoffice {
    int createTxSocket(bool broadcast, const char* ip, const char* port, int default_buffer_size);
    int createRxSocket(bool broadcast, const char* port, int default_buffer_size);
    int closeConnection();
    int send(const void *message, int size_of_message);
    const char* recieve();
}

