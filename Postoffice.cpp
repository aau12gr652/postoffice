#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "Postoffice.h"
#include <string.h>

postoffice::postoffice(const char* port, const char* ip)
{
    direction=TX;
    int return_value = createSocket(ip, port);
    if (return_value)
    {
        std::cout << "\nERROR: " << return_value << std::endl;
        closeConnection();
        socketid=0;
    }
}

postoffice::postoffice(const char* port)
{
    direction=RX;
    int return_value = createSocket(NULL, port);
    if (return_value)
    {
        std::cout << "\nERROR: " << return_value << std::endl;
        closeConnection();
        socketid=0;
    }
}

int postoffice::closeConnection()
{
    if (socketid)
        if (close(socketid))
            return CONNECTION_CLOSE_ERROR;
        else
            return NO_ERROR;
        else
            return SOCKET_ID_NOT_VALID;
}

int postoffice::isValid()
{
    if (socketid)
        return true;
    return false;
}

int postoffice::send(const void *message, int size_of_message)
{    
    if (socketid)
        if (sendto(socketid, message, size_of_message, 0, server_info->ai_addr, server_info->ai_addrlen))
            return SEND_ERROR;
        else
            return NO_ERROR;
    return SOCKET_ID_NOT_VALID;
}

int postoffice::receive(void* bufferptr, int size)
{
    if (socketid)
    {
        ssize_t received_message_size;
        received_message_size = recvfrom(socketid, bufferptr, size, 0, server_info->ai_addr, &server_info->ai_addrlen);
        if (received_message_size > 0)
            if (received_message_size > 2147483647)
                return SIZE_ERROR;
            else
                return received_message_size;
        std::cout << "\n" << strerror(errno) << std::endl; // Mulighed for at udskrive fejl tekst fra underlæggende system!
        return RECEIVE_ERROR;
    }
    return SOCKET_ID_NOT_VALID;
}

int postoffice::createSocket(const char* ip, const char* port)
{
    struct addrinfo readable_server_info;
    int return_value;
    
    memset(&readable_server_info, 0, sizeof(readable_server_info)); //Reset the struct hints
    readable_server_info.ai_family = AF_INET; //Set to ipv4
    readable_server_info.ai_socktype = SOCK_DGRAM; //Set type to  datagram (Unreliable)
    readable_server_info.ai_protocol = IPPROTO_UDP; //Ensure that udp is used
    
    if (direction)
    {
        readable_server_info.ai_flags = AI_PASSIVE;    //Set ip to local
        return_value = createSocketCommon(ip, port, &readable_server_info);
        if (return_value)
            return return_value;
        
        if (bind(socketid, server_info->ai_addr, server_info->ai_addrlen))
        {
            std::cout << "\n" << strerror(errno) << std::endl;
            return BINDING_ERROR;
        }
    }
    else
        return_value = createSocketCommon(ip, port, &readable_server_info);
    
    freeaddrinfo(server_info); //Free the server_info not being used anymore
    return return_value;
}

int postoffice::createSocketCommon(const char* ip, const char* port, addrinfo* readable_server_info)
{
    if (getaddrinfo(ip, port, readable_server_info, &server_info))
        return GET_ADDR_INFO_ERROR;
    
    socketid = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if (socketid < 0)
    {
        std::cout << "\n" << strerror(errno) << std::endl;
        return SOCKET_CREATION_ERROR;
    }
    
    int TRUE = 1;
    if (setsockopt(socketid, SOL_SOCKET, SO_BROADCAST, &TRUE, sizeof(TRUE)))
    {
        std::cout << "\n" << strerror(errno) << std::endl;
        return SET_SOCKET_OPTION_ERROR;
    }
    return NO_ERROR;
}