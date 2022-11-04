#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void processHeader(ifstream &file, ofstream &outFile,
                   float &m, float &c, int &width,
                   int &height, bool ldr, bool write)
{

    int initial_vars = 0;
    string line;
    bool hasMax = false;

    while (initial_vars < 3)
    {
        getline(file, line);
        if (line[0] == '#')
        {
            if (line[1] == 'M' && line[2] == 'A' && line[3] == 'X' && line[4] == '=')
            {
                m = stof(line.substr(5, line.length()));
                hasMax = true;
            }

            if (write)
                outFile << line << endl;
        }
        else
        {

            // First line is skipped
            // Image resolution
            if (initial_vars == 1)
            {

                width = stoi(line.substr(0, line.find(" ")));
                height = stoi(line.substr(line.find(" "), line.length()));

                if (write)
                    outFile << line << endl;

            } // Color resolution
            else if (initial_vars == 2)
            {
                c = stof(line);
                int hdrC = ldr ? 255 : c;
                if (write)
                    outFile << hdrC << endl;
            }

            else if (write)
                outFile << line << endl;
            initial_vars++;
        }
    }

    if (!hasMax)
        m = c;
}

void showProgress(int row, int height)
{
    if (row % 20 == 0)
    {
        int bar = row;
        int percentage = 0;
        cout << "[";
        percentage = row * 100 / height;
        for (int i = 0; i < 40; i++)
        {
            if (bar >= height / 40)
            {
                cout << "+";
            }
            else
            {
                cout << " ";
            }
            bar -= height / 40;
        }
        cout << "]\t" << percentage << "%\r";
        cout.flush();
    }
}

void ldr(string fileName)
{

    float m, c;
    int width, height;

    ifstream file;
    ofstream outFile;
    file.open(fileName);
    outFile.open("ldr/ldr_" + fileName);

    if (!file)
    {
        throw std::invalid_argument("File not found!");
    }
    else
    {
        processHeader(file, outFile, m, c, width, height, true, true);

        string num;
        int numI;
        int count = 0;
        int column = 0;
        int row = 0;

        file >> num;
        while (!file.eof())
        {
            numI = stoi(num);
            float v = numI * m / c;
            int reduced = round(v * (255 / m));
            outFile << reduced;
            count++;
            if (count == 3)
            {
                count = 0;
                outFile << "    ";
                column++;
            }
            if (column == width)
            {
                outFile << endl;
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                outFile << " ";
            }
            file >> num;
        }
    }
    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "LDR export completed! Check the new file "
         << "ldr_" + fileName << "." << endl;

    file.close();
    outFile.close();
}

void clamping(string fileName, float value = 0.0)
{

    float m, c;
    int width, height;

    ifstream file;
    ofstream outFile;
    file.open(fileName);
    outFile.open("c_" + fileName);

    if (!file)
    {
        throw std::invalid_argument("File not found!");
    }
    else
    {

        processHeader(file, outFile, m, c, width, height, false, true);

        if (value == 0.0)
        {
            value = m;
        }

        string num;
        int s;
        int count = 0;
        int column = 0;
        int row = 0;

        file >> num;
        while (!file.eof())
        {
            s = stoi(num);
            float v = s * m / c;
            if (v > value)
            {
                v = value;
                v = v * m / value;
                s = round(v * c / m);
                outFile << s;
            }
            else
            {
                v = v * m / value;
                s = round(v * c / m);
                outFile << s;
            }
            count++;
            if (count == 3)
            {
                count = 0;
                outFile << "    ";
                column++;
            }
            if (column == width)
            {
                outFile << endl;
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                outFile << " ";
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Clamping completed! Check the new file "
         << "c_" + fileName << "." << endl;

    file.close();
    outFile.close();
}

void equalization(string fileName, float value = 0.0)
{

    float m, c;
    int width, height;

    ifstream file;
    ofstream outFile;
    file.open(fileName);
    outFile.open("e_" + fileName);

    if (!file)
    {
        throw std::invalid_argument("File not found!");
    }
    else
    {

        processHeader(file, outFile, m, c, width, height, false, true);

        string num;
        int s;

        int count = 0;
        int column = 0;
        int row = 0;

        if (value == 0.0)
        {
            value = m;
        }

        file >> num;
        while (!file.eof())
        {
            s = stoi(num);
            float v = s * m / c;
            v = v * m / value;
            s = v * c / m;
            outFile << s;
            count++;
            if (count == 3)
            {
                count = 0;
                outFile << "    ";
                column++;
            }
            if (column == width)
            {
                outFile << endl;
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                outFile << " ";
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Equalization completed! Check the new file "
         << "e_" + fileName << "." << endl;

    file.close();
    outFile.close();
}

void equalizationAndCampling(string fileName, float value = 0.0)
{
    string oldFileName = fileName;
    // First the picture is equalized
    equalization(fileName, value);
    fileName = "e_" + oldFileName;

    // Then the picture is clamped
    clamping(fileName, value);

    if (remove(("e_" + oldFileName).c_str()))
    {
        throw std::invalid_argument("Error deleting e_" + oldFileName);
    }
}

// Gamma values between 0-2 (>1 darker, <1 brighter)
void gamma(string fileName, float g, float value)
{
    string oldFileName = fileName;
    // First the picture is equalized
    equalization(fileName, value);
    fileName = "e_" + fileName;

    float m, c;
    int width, height;

    ifstream file;
    ofstream outFile;
    file.open(fileName);
    outFile.open("g_" + oldFileName);

    if (!file)
    {
        throw std::invalid_argument("File not found!");
    }
    else
    {
        processHeader(file, outFile, m, c, width, height, false, true);

        string num;

        long s;
        int count = 0;
        int column = 0;
        int row = 0;

        file >> num;
        while (!file.eof())
        {
            // Read the number and save it
            s = stoi(num);
            long v = s * m / c;
            // Apply gamma curve
            v = m * pow(v / m, g);
            s = round(v * c / m);
            // Write number to the outfile
            outFile << s;
            count++;
            // Write spaces every three numbers
            if (count == 3)
            {
                count = 0;
                outFile << "    ";
                column++;
            }
            // No more columns in this row
            if (column == width)
            {
                outFile << endl;
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                outFile << " ";
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Gamma curve completed! Check the new file "
         << "g_" + oldFileName << "." << endl;

    file.close();
    outFile.close();

    // e_filename is removed
    if (remove(fileName.c_str()))
    {
        throw std::invalid_argument("Error deleting " + fileName);
    }
}

void clampingAndGamma(string fileName, float g, float value = 0.0)
{
    string oldFileName = fileName;
    gamma(fileName, g, value);
    fileName = "g_" + oldFileName;

    // Then the picture is clamped
    clamping(fileName, value);

    if (remove(("g_" + oldFileName).c_str()))
    {
        throw std::invalid_argument("Error deleting g_" + oldFileName);
    }
}