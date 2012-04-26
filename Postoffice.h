//
//  postoffice.h
//  
//
//  Created by Benjamin Krebs on 25/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _postoffice_h
#define _postoffice_h

class postoffice {
    int socketidtx;
    int socketidrx;
    struct addrinfo *server_info_rx;
    struct addrinfo *server_info_tx;
    
    bool decision;
    
    int createTxSocket(bool broadcast, const char* ip, const char* port);
    
    int createRxSocket(bool broadcast, const char* port);
    
    void createSocket(bool broadcast, const char* ip, const char* port, bool rx_true_tx_false);
    
public:
    
    postoffice(const char* port, const char* ip);
    
    int closeConnection();
    
    int send(const void *message, int size_of_message);
    
    int recieve(void* bufferptr, int size);
    
};



#endif
