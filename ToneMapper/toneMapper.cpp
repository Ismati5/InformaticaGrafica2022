#include <iostream>
#include "toneMapping.hpp"

int main(int argc, char *argv[])
{
    string outputFile;
    if (argv[1][1] == 'l') // toneMapper -l <filename.ppm>
    {
        outputFile = ldr(argv[2]);
    }
    else if (argv[1][1] == 'c') // toneMapper -c <filename.ppm> <value>
    {
        outputFile = clamping(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'e') // toneMapper -e <filename.ppm> <value>
    {
        outputFile = equalization(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'b') // toneMapper -b <filename.ppm> <value>
    {
        outputFile = equalizationAndCampling(argv[2], stof(argv[3]));
    }
    else if (argv[1][1] == 'g') // toneMapper -g <filename.ppm> <gamma> <value>
    {
        outputFile = gamma(argv[2], stof(argv[3]), stof(argv[4]));
    }
    else
    {
        cout << "Usage: toneMapper [ -l | -c | -e | -b | -g ] <filename.ppm>" << endl;
    }

    ShellExecuteA(GetDesktopWindow(), NULL, outputFile.c_str(), NULL, NULL, SW_SHOW); // Open output file created
    
    return 0;
}