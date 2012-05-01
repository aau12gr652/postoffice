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
        stamp A = {1, 2, 3, 4, 5, 6, 7};
        const char* msg="Hello, world!";
        serial_data B = {strlen(msg), (void*)msg};
        serial_data letter = frank(&A, B);
        po.send(letter.data, letter.size);
        po.closeConnection();
    }
    else if (!strncmp(argv[1],"RX",2))
    {
        std::cout << "RX\n";
        postoffice po("4000");
        char data[100];
        serial_data received_letter = {po.receive(data,100), data};
        stamp* header = (stamp*)malloc(sizeof(stamp));
        serial_data letter = unfrank(received_letter, header);
        std::cout << (char*)letter.data << std::endl;
        po.closeConnection();

        std::cout << header->Generation_ID*1 << std::endl;
	    std::cout << header->Number_Of_Layers*1 << std::endl;
    	std::cout << header->Layer_ID*1 << std::endl;
	    std::cout << header->Field_Size*1 << std::endl;
    	std::cout << header->Symbol_Size*1 << std::endl;
	    std::cout << header->Generation_Size*1 << std::endl;
    	std::cout << header->Layer_Size*1 << std::endl;
    }
    return 0;
}
