#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void processHeader(ifstream &file, float m, float c, int width, int height) {

    int initial_vars = 0;
    string line;

    ifstream file;

    if (!file) {
        throw std::invalid_argument("File not found!");
    } else {
        
        while (initial_vars < 3) { 
            getline(file, line);
            if (line[0] == '#'){
                if (line[1] == 'M' && line[2] == 'A'
                 && line[3] == 'X' && line[4] == '=')
                    m = stof(line.substr(5,line.length()));
            } else{

                //First line is skipped
                //Image resolution
                if (initial_vars == 1) {
                    
                    width = stoi(line.substr(0, line.find(" ")));
                    height = stoi(line.substr(line.find(" "), line.length()));
                        
                }//Color resolution 
                else if (initial_vars == 2)
                    c = stof(line);

                initial_vars++;
            } 
            
        }

}

void clamping(string fileName){

    float m, c;
    int width, height;
    int initial_vars = 0;
    string line;

    ifstream file;
    ofstream outFile;
    file.open(fileName);
    outFile.open("clamping_" + fileName);

    if (!file) {
        throw std::invalid_argument("File not found!");
    } else {
        
        while (initial_vars < 3) { 
            getline(file, line);
            if (line[0] == '#'){
                if (line[1] == 'M' && line[2] == 'A'
                 && line[3] == 'X' && line[4] == '=')
                    m = stof(line.substr(5,line.length()));
            } else{

                //First line is skipped
                //Image resolution
                if (initial_vars == 1) {
                    
                    width = stoi(line.substr(0, line.find(" ")));
                    height = stoi(line.substr(line.find(" "), line.length()));
                        
                }//Color resolution 
                else if (initial_vars == 2)
                    c = stof(line);

                initial_vars++;
            } 
            
            outFile << line << endl;
        }

        string num;
        int numI;
        int count = 0;
        int column = 0;
        
        while(!file.eof()){
            file >> num;
            numI = stoi(num);
            if(((numI * m) / c) > m){
                outFile << c;
            } else {
                outFile << numI;
            }
            count++;
            if (count == 3){
                count = 0;
                outFile << "    ";
                column++;
            }
            if (column == width) {
                outFile << endl;
                column = 0;
            } else {
                outFile << " ";
            }   
        }
    }

    file.close();
    outFile.close();
}