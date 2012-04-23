//
//  Postoffice.h
//  broadcastServer
//
//  Created by Benjamin Krebs on 18/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

namespace Postoffice {
    int createTxSocket(bool broadcast, const char* ip, const char* port, int default_buffer_size);
    int createRxSocket(bool broadcast, const char* port, int default_buffer_size);
    int closeConnection();
    int send(const void *message, int size_of_message);
    const char* recieve();
}

