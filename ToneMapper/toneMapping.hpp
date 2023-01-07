#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

// return max emission position in the output vector
int processHeader(ifstream &file,
                  float &m, float &c, int &width,
                  int &height, bool ldr, bool write, vector<string> &output)
{
    int initial_vars = 0, max_pos;
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
                output.push_back(line + "\n");
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
                    output.push_back(line + "\n");

            } // Color resolution
            else if (initial_vars == 2)
            {
                c = stof(line);
                int hdrC = ldr ? 255 : c;
                if (write)
                    output.push_back(to_string(hdrC));

                max_pos = output.size();
            }
            else if (write)
                output.push_back(line + "\n");

            initial_vars++;
        }
    }

    output.push_back("\n");

    if (!hasMax)
        m = c;

    return max_pos - 1;
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

void dumpToFile(ofstream &outFile, vector<string> header, vector<string> output)
{
    for (string s : header)
        outFile << s;

    for (string s : output)
        outFile << s;
}

string ldr(string fileName)
{
    vector<string> header;
    vector<string> output;

    float m, c;
    int width, height, max_pos, max = numeric_limits<int>::lowest();

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
        max_pos = processHeader(file, m, c, width, height, true, true, header);

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
            if (reduced > max)
                max = reduced;

            output.push_back(to_string(reduced));
            count++;
            if (count == 3)
            {
                count = 0;
                output.push_back("    ");
                column++;
            }
            if (column == width)
            {
                output.push_back("\n");
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                output.push_back(" ");
            }
            file >> num;
        }
    }
    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "LDR export completed! Check the new file "
         << "ldr_" + fileName << "." << endl;

    header[max_pos] = to_string(max);

    dumpToFile(outFile, header, output);

    file.close();
    outFile.close();

    return "ldr\\ldr_" + fileName;
}

string clamping(string fileName, float value = 0.0)
{
    vector<string> header;
    vector<string> output;
    float m, c;
    int width, height, max_pos, max = numeric_limits<int>::lowest();
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
        max_pos = processHeader(file, m, c, width, height, false, true, header);

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
            }
            else
            {
                v = v * m / value;
                s = round(v * c / m);
            }
            if (s > max)
                max = s;
            output.push_back(to_string(s));
            count++;
            if (count == 3)
            {
                count = 0;
                output.push_back("    ");
                column++;
            }
            if (column == width)
            {
                output.push_back("\n");
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                output.push_back(" ");
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Clamping completed! Check the new file "
         << "c_" + fileName << "." << endl;

    header[max_pos] = to_string(max);

    dumpToFile(outFile, header, output);

    file.close();
    outFile.close();

    return "c_" + fileName;
}

string equalization(string fileName, float value = 0.0)
{

    vector<string> header;
    vector<string> output;
    float m, c;
    int width, height, max_pos, max = numeric_limits<int>::lowest();
    ;

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

        max_pos = processHeader(file, m, c, width, height, false, true, header);

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
            if (s > max)
                max = s;

            output.push_back(to_string(s));
            count++;
            if (count == 3)
            {
                count = 0;
                output.push_back("    ");
                column++;
            }
            if (column == width)
            {

                output.push_back("\n");
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                output.push_back(" ");
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Equalization completed! Check the new file "
         << "e_" + fileName << "." << endl;

    header[max_pos] = to_string(max);

    dumpToFile(outFile, header, output);

    file.close();
    outFile.close();

    return "e_" + fileName;
}

string equalizationAndCampling(string fileName, float value = 0.0)
{
    string oldFileName = fileName;
    // First the picture is equalized
    equalization(fileName, value);
    fileName = "e_" + oldFileName;

    // Then the picture is clamped
    string outFile = clamping(fileName, value);

    if (remove(("e_" + oldFileName).c_str()))
    {
        throw std::invalid_argument("Error deleting e_" + oldFileName);
    }

    return outFile;
}

// Gamma values between 0-2 (>1 darker, <1 brighter)
string gamma(string fileName, float g, float value)
{
    string oldFileName = fileName;
    // First the picture is equalized

    equalization(fileName, value);
    fileName = "e_" + fileName;

    vector<string> header;
    vector<string> output;
    float m, c;
    int width, height, max_pos, max = numeric_limits<int>::lowest();

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
        max_pos = processHeader(file, m, c, width, height, false, true, header);

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

            v = m * pow(v / m, 1 / g);

            s = round(v * c / m);

            if (s > max)
                max = s;

            // Write number to the outfile
            output.push_back(to_string(s));
            count++;
            // Write spaces every three numbers
            if (count == 3)
            {
                count = 0;
                output.push_back("    ");
                column++;
            }
            // No more columns in this row
            if (column == width)
            {
                output.push_back("\n");
                column = 0;
                row++;
                showProgress(row, height);
            }
            else
            {
                output.push_back(" ");
            }
            file >> num;
        }
    }

    cout << "[++++++++++++++++++++++++++++++++++++++++]\t100%" << endl
         << endl;
    cout << "Gamma curve completed! Check the new file "
         << "g_" + oldFileName << "." << endl;

    header[max_pos] = to_string(max);

    dumpToFile(outFile, header, output);

    file.close();
    outFile.close();

    // e_filename is removed
    if (remove(fileName.c_str()))
    {
        throw std::invalid_argument("Error deleting " + fileName);
    }

    return "g_" + oldFileName;
}

string clampingAndGamma(string fileName, float g, float value = 0.0)
{
    string oldFileName = fileName;
    gamma(fileName, g, value);
    fileName = "g_" + oldFileName;

    // Then the picture is clamped
    string outFile = clamping(fileName, value);

    if (remove(("g_" + oldFileName).c_str()))
    {
        throw std::invalid_argument("Error deleting g_" + oldFileName);
    }

    return outFile;
}