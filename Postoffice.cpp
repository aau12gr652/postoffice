#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include "Postoffice.h"
#include <string.h>
#include <fstream>

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
    freeaddrinfo(server_info); //Free the server_info not being used anymore
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

int postoffice::send(void* message, int size_of_message, stamp header)
{
    serial_data Letter = {size_of_message, message};
    return sendLetter(postoffice::frank(header, Letter));
}

int postoffice::sendLetter(serial_data Letter)
{    
    if (socketid)
    {
        ssize_t result = sendto(socketid, Letter.data, Letter.size, 0, server_info->ai_addr, server_info->ai_addrlen);
        int fejl = errno;
        if (result < 0)
        {
            int fejl = errno;
            std::cout << std::endl << strerror(fejl) << std::endl;
            return SEND_ERROR;
        }
        else
            return NO_ERROR;
    }
    return SOCKET_ID_NOT_VALID;
}

int postoffice::receive(void* bufferptr, int size, stamp* header)
{
    serial_data Letter = {size, bufferptr};
    int msgSize = postoffice::receiveLetter(Letter);
    Letter.size = msgSize;
    return unfrank(Letter, header);
}

int postoffice::receiveLetter(serial_data Letter)
{
    if (socketid)
    {
        ssize_t received_message_size = recvfrom(socketid, Letter.data, Letter.size, 0, server_info->ai_addr, &server_info->ai_addrlen);
        if (received_message_size > 0)
            if (received_message_size > 2147483647)
                return SIZE_ERROR;
            else
                return received_message_size;
        std::cout << "\n" << strerror(errno) << std::endl;
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

serial_data postoffice::frank(stamp header, serial_data packet)
{
    int total_size = packet.size + sizeof(stamp);
    void* p = malloc(total_size);
    serial_data return_value = {total_size, p};
    memcpy(p, &header, sizeof(stamp));
    memcpy((char*)p + sizeof(stamp), packet.data, packet.size);
    free(packet.data);
    return return_value;
}

int postoffice::unfrank(serial_data letter, stamp* header) // header is a output variable, memory has to be allocated before function call
{
    memcpy(header, letter.data, sizeof(stamp));
    int total_size = letter.size-sizeof(stamp);
    memcpy(letter.data, (char*)letter.data+sizeof(stamp), total_size);
    return total_size;
}

uint8_t* devRandom(int count)
{
    std::ifstream randomFile;
    randomFile.open("/dev/random");
    uint8_t* tal = (uint8_t*)malloc(sizeof(uint8_t)*count);
    for (int n = 0; n < count; n++)
        tal[n] = randomFile.get();
    randomFile.close();
    return tal;
}