#include <iostream>
#include "ToneMapper.hpp"

// g++ main.cpp -o main -std=c++17
int main(int argc, char *argv[])
{
    if (argv[1][1] == 'l')
    {
        ldr(argv[2]);
    }
    else if (argv[1][1] == 'c')
    {
        clamping(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'e')
    {
        equalization(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'b')
    {
        equalizationAndCampling(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'g')
    {
        gamma(argv[2], stof(argv[3]), stof(argv[4]));
    }
    else
    {
        cout << "Usage: main [-l | -c | -e | -b | -g] <filename.ppm>" << endl;
    }

    return 0;
}