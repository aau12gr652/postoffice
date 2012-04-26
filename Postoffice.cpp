//
//  postoffice.cpp
//  
//
//  Created by Benjamin Krebs on 25/04/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Postoffice.h"

postoffice::postoffice(const char* port, const char* ip){
    std::cout << "Så lavede man lige et objekt!";
    
    socketidtx=0;
    socketidrx=0;
    
    
    
    if (ip == NULL) {
        createSocket(true, ip, port, true);
    }
    else {
        createSocket(true, ip, port, false);
        
    }
    
    
}


int postoffice::closeConnection(){
    if (socketidtx!=0) {
        close(socketidtx);
    }
    else if (socketidrx!=0) {
        close(socketidrx);
    }
    else {
        return -1;
    }
    
    return 0;
}

int postoffice::send(const void *message, int size_of_message){
    
    if (socketidtx==0) {
        return -1;
    }
    
    if ((sendto(socketidtx, message, size_of_message, 0, server_info_tx->ai_addr, server_info_tx->ai_addrlen)) == -1){
        std::cout << "SENDTO ERROR\n";
    }
    return 0;
}

int postoffice::recieve(void* bufferptr, int size){
    if (socketidrx==0) {
        return -1;
    }
    
    int recieved_message_size;
    
    recieved_message_size = recvfrom(socketidrx, bufferptr, size, 0, server_info_rx->ai_addr, &server_info_rx->ai_addrlen);
    
    return recieved_message_size;
}




void postoffice::createSocket(bool broadcast, const char* ip, const char* port, bool rx_true_tx_false){
    struct addrinfo readable_server_info;
    
    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct hints
    readable_server_info.ai_family = AF_INET; //Set to ipv4
    readable_server_info.ai_socktype = SOCK_DGRAM; //Set type to  datagram (Unreliable)
    readable_server_info.ai_protocol = IPPROTO_UDP; //Ensure that udp is used
    
    
    if (rx_true_tx_false) {
        readable_server_info.ai_flags = AI_PASSIVE;    //Set ip to local
        
        getaddrinfo(NULL, port, &readable_server_info, &server_info_rx); //Converting the readable server info into network ordered bytes
        
        if ((socketidrx = socket(server_info_rx->ai_family, server_info_rx->ai_socktype, server_info_rx->ai_protocol)) == -1) {
            std::cout << "SOCKET CREATION ERROR\n";
        }    
        
        
        if (broadcast) {
            int YES = 1;
            setsockopt(socketidrx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
        }
        
        
        if (bind(socketidrx, server_info_rx->ai_addr, server_info_rx->ai_addrlen) == -1){ //Make a connection to the socket
            close(socketidrx);
            std::cout << "BINDING ERROR!!!\n\n";
        }
        
        freeaddrinfo(server_info_rx); //Free the server_info not being used anymore
        
        std::cout << "\n RX socket created" << std::endl;
    }
    else {
        if (getaddrinfo(ip, port, &readable_server_info, &server_info_tx) == -1) {
            std::cout << "GETADDRINFO ERROR\n";
        }
        
        
        if ((socketidtx = socket(server_info_tx->ai_family, server_info_tx->ai_socktype, server_info_tx->ai_protocol)) == -1) {
            std::cout << "SOCKET CREATION ERROR\n";
        }    
        
        
        int YES = 1;
        setsockopt(socketidtx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
        if (broadcast) {
            
            setsockopt(socketidtx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
        }
        
        freeaddrinfo(server_info_tx); //Free the server_info not being used anymore
        
        std::cout << "\n TX socket created" << std::endl;
    }

    
    
    
}





//int postoffice::createTxSocket(bool broadcast, const char* ip, const char* port){
//    struct addrinfo readable_server_info;
//    
//    
//    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct readable_server_info
//    readable_server_info.ai_family = AF_INET;
//    readable_server_info.ai_socktype = SOCK_DGRAM;
//    readable_server_info.ai_protocol = IPPROTO_UDP;
//    
//    if (getaddrinfo(ip, port, &readable_server_info, &server_info_tx) == -1) {
//        std::cout << "GETADDRINFO ERROR\n";
//    }
//    
//    
//    if ((socketidtx = socket(server_info_tx->ai_family, server_info_tx->ai_socktype, server_info_tx->ai_protocol)) == -1) {
//        std::cout << "SOCKET CREATION ERROR\n";
//    }
//    
//    
//    if (broadcast == true) {
//        int YES = 1;
//        setsockopt(socketidtx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
//    }
//    
//    return 0;
//    
//}
//
//int postoffice::createRxSocket(bool broadcast, const char* port){
//    
//    struct addrinfo readable_server_info;
//    
//    
//    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct hints
//    readable_server_info.ai_family = AF_INET; //Set to ipv4
//    readable_server_info.ai_socktype = SOCK_DGRAM; //Set type to  datagram (Unreliable)
//    readable_server_info.ai_flags = AI_PASSIVE;    //Set ip to local
//    readable_server_info.ai_protocol = IPPROTO_UDP; //Ensure that udp is used
//    
//    getaddrinfo(NULL, port, &readable_server_info, &server_info_rx); //Converting the readable server info into network ordered bytes
//    
//    
//    socketidrx = socket(server_info_rx->ai_family, server_info_rx->ai_socktype, server_info_rx->ai_protocol); //Create socket
//    
//    
//    if (broadcast == true) {
//        int YES = 1;
//        setsockopt(socketidrx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
//    }
//    
//    if (bind(socketidrx, server_info_rx->ai_addr, server_info_rx->ai_addrlen) == -1){ //Make a connection to the socket
//        close(socketidrx);
//        std::cout << "BINDING ERROR!!!\n\n";
//    }
//    
//    freeaddrinfo(server_info_rx); //Free the server_info not being used anymore
//    
//    return 0;
//    
//}

