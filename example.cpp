#include <iostream>
#include "Postoffice.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
        return -1;
    if (!strncmp(argv[1],"TX",2))
    {
        std::cout << "TX\n";
        postoffice po("4000", "255.255.255.255");
        const char *msg="Hello, world!\n";
        int msgSize = 16;
        po.send(msg, msgSize);
        po.closeConnection();
    }
    else if (!strncmp(argv[1],"RX",2))
    {
        std::cout << "RX\n";
        postoffice po("s");
        char data[100];
        po.receive(data,100);
        std::cout << data;
        po.closeConnection();
    }
    return 0;
}
