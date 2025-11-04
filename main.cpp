#include <fstream>
#include <iostream>


#include "JsonParser.hpp"

int main(int argc, char** argv)
{
    std::ifstream file(argv[1]);

    if (file.is_open() == false)
    {
        return 1;
    }

    JsonParser parser(file);


    file.close();
    return 0;
}
