#include <iostream>
#include "Postoffice.h"

int main(int argc, char *argv[])
{
    uint8_t* n = devRandom(2);
    //std::cout << n[0]*1 << " " << n[1]*1 << std::endl;

    if (argc != 2)
        return -1;
    if (!strncmp(argv[1],"TX",2))
    {
        std::cout << "TX " << std::endl;
        postoffice po("4000", "255.255.255.255");
        
        stamp A = {1, 2, 3, 4, 5, 6, 7};
        const char* msg="Hello, world!";
        
        for (int u = 0; u < 2000; u++)
        {
            po.send((void*)msg, strlen(msg), A);
            std::cout << u*1 << std::endl;
        }
        po.closeConnection();
        usleep(100);
    }
    else if (!strncmp(argv[1],"RX",2))
    {
        std::cout << "RX " << std::endl;
        postoffice po("4000");
        char payload[100];
        stamp* header = (stamp*)malloc(sizeof(stamp));

        for (int u = 0; u < 2000; u++)
        {
            po.receive(payload, 100, header);
            std::cout << u*1 << std::endl;
        }

        //std::cout << (char*)payload << std::endl;
        po.closeConnection();

        /*
        std::cout << header->Generation_ID*1 << std::endl;
        std::cout << header->Number_Of_Layers*1 << std::endl;
        std::cout << header->Layer_ID*1 << std::endl;
        std::cout << header->Field_Size*1 << std::endl;
        std::cout << header->Symbol_Size*1 << std::endl;
        std::cout << header->Generation_Size*1 << std::endl;
        std::cout << header->Layer_Size*1 << std::endl;
        */
    }
    return 0;
    
/*  // Print the stamps :-)
    std::cout << A.Generation_ID*1 << std::endl;
    std::cout << A.Number_Of_Layers*1 << std::endl;
    std::cout << A.Layer_ID*1 << std::endl;
    std::cout << A.Field_Size*1 << std::endl;
    std::cout << A.Symbol_Size*1 << std::endl;
    std::cout << A.Generation_Size*1 << std::endl;
    std::cout << A.Layer_Size*1 << std::endl;

    std::cout << header->Generation_ID*1 << std::endl;
    std::cout << header->Number_Of_Layers*1 << std::endl;
    std::cout << header->Layer_ID*1 << std::endl;
    std::cout << header->Field_Size*1 << std::endl;
    std::cout << header->Symbol_Size*1 << std::endl;
    std::cout << header->Generation_Size*1 << std::endl;
    std::cout << header->Layer_Size*1 << std::endl;
 */   
 
}
