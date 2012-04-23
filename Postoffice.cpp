//  Postoffice.cpp
//  Network module for rx/tx via UDP
//
//  Created by Benjamin Krebs on 16/04/12.


#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "Postoffice.h"


int socketidtx;
int socketidrx;
struct addrinfo *server_info_rx;
struct addrinfo *server_info_tx;
int rxbuffersize;


int Postoffice::createRxSocket(bool broadcast, const char* port, int buffersize){
    rxbuffersize = buffersize;
    
    struct addrinfo readable_server_info;

    
    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct hints
    readable_server_info.ai_family = AF_INET; //Set to ipv4
    readable_server_info.ai_socktype = SOCK_DGRAM; //Set type to  datagram (Unreliable)
    readable_server_info.ai_flags = AI_PASSIVE;    //Set ip to local
    readable_server_info.ai_protocol = IPPROTO_UDP; //Ensure that udp is used
    
    getaddrinfo(NULL, port, &readable_server_info, &server_info_rx); //Converting the readable server info into network ordered bytes
    
    
    socketidrx = socket(server_info_rx->ai_family, server_info_rx->ai_socktype, server_info_rx->ai_protocol); //Create socket
    
    
    if (broadcast == true) {
        int YES = 1;
        setsockopt(socketidrx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
    }
    
    if (bind(socketidrx, server_info_rx->ai_addr, server_info_rx->ai_addrlen) == -1){ //Make a connection to the socket
        close(socketidrx);
        std::cout << "BINDING ERROR!!!\n\n";
    }
    
    freeaddrinfo(server_info_rx); //Free the server_info not being used anymore
    
    return 0;
}


int Postoffice::createTxSocket(bool broadcast, const char* ip, const char* port, int default_buffer_size){
    
    
    struct addrinfo readable_server_info;
    
    
    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct readable_server_info
    readable_server_info.ai_family = AF_INET;
    readable_server_info.ai_socktype = SOCK_DGRAM;
    readable_server_info.ai_protocol = IPPROTO_UDP;
    
    if (getaddrinfo(ip, port, &readable_server_info, &server_info_tx) == -1) {
        std::cout << "GETADDRINFO ERROR\n";
    }
    
    
    if ((socketidtx = socket(server_info_tx->ai_family, server_info_tx->ai_socktype, server_info_tx->ai_protocol)) == -1) {
        std::cout << "SOCKET CREATION ERROR\n";
    }
    
    
    if (broadcast == true) {
        int YES = 1;
        setsockopt(socketidtx, SOL_SOCKET, SO_BROADCAST, &YES, sizeof(YES));
    }
    
    return 0;
}

int Postoffice::closeConnection(){
    close(socketidtx);
    close(socketidrx);
    
    return 0;
}



int Postoffice::send(const void *message, int size_of_message){
    
    if ((sendto(socketidtx, message, size_of_message, 0, server_info_tx->ai_addr, server_info_tx->ai_addrlen)) == -1){
        std::cout << "SENDTO ERROR\n";
    }
    return 0;
}



const char *Postoffice::recieve(){
    char recieved_message[rxbuffersize];
    
    recvfrom(socketidrx, &recieved_message, rxbuffersize, 0, server_info_rx->ai_addr, &server_info_rx->ai_addrlen);
    
    const char *pointer = recieved_message;
    
    return pointer;
}