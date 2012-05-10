#include <iostream>
#include "Postoffice.h"

int main(int argc, char *argv[])
{
    uint8_t* n = devRandom();
    std::cout << n[0]*1 << std::endl;
    free(n);
    int TOTAL = 100;

    if (argc != 2)
        return -1;
    if (!strncmp(argv[1],"TX",2))
    {
        std::cout << "TX " << std::endl;
        postoffice po("4000", "255.255.255.255");

        stamp A = {1, 2, 3, 4, 5, 6, 7};
        const int msgSize = 1400;
        void* msg = (void*)devRandom(msgSize);
		serial_data Letter = {msgSize, msg};

        for (int u = 0; u < TOTAL; u++)
        {
            po.send(msg, msgSize, &A);
            //po.send(Letter, &A);
            std::cout << u*1 << std::endl;
            usleep(500);
        }
        po.closeConnection();
        free(msg);
        //sleep(1000);
    }
    else if (!strncmp(argv[1],"RX",2))
    {
        std::cout << "RX " << std::endl;
        postoffice po("4000");
        char payload[1600];
        stamp* header = (stamp*)malloc(sizeof(stamp));

        int t;
        for (int u = 0; u < TOTAL; u++)
        {
            t = po.receive(payload, 1600, header, 1);
            std::cout << u*1 << std::endl;
        }
        free(header);
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
