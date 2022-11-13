#include <iostream>
#include "toneMapping.hpp"

int main(int argc, char *argv[])
{
    if (argv[1][1] == 'l') // toneMapper -l <filename.ppm>
    {
        ldr(argv[2]);
    }
    else if (argv[1][1] == 'c') // toneMapper -c <filename.ppm> <value>
    {
        clamping(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'e') // toneMapper -e <filename.ppm> <value>
    {
        equalization(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'b') // toneMapper -b <filename.ppm> <value>
    {
        equalizationAndCampling(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'g') // toneMapper -g <filename.ppm> <gamma> <value>
    {
        gamma(argv[2], stof(argv[3]), stof(argv[4]));
    }
    else
    {
        cout << "Usage: toneMapper [ -l | -c | -e | -b | -g ] <filename.ppm>" << endl;
    }

    return 0;
}