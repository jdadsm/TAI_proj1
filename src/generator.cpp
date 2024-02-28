
#include <fstream>
#include <string>
#include <iostream>

// class to prevent loading the whole file into memory
class Generator {
private:
    std::ifstream file;
    std::string buffer;
    std::string method;
    long position;
    int chunkSize;
    bool reachedEOF;

public:
    Generator(std::string& filename, int chunkSize, std::string method) : file(filename), chunkSize(chunkSize), method(method) {
        if(method == "last"){
            setChunkSize(chunkSize+1);
        }
        position = this->chunkSize-1;
        reachedEOF = false;
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }

    ~Generator() {
        file.close();
    }
    void setChunkSize(int size){
        this->chunkSize = size;
    }
    int getChunkSize() {
        return this->chunkSize;
    }
    std::string getNextChunk() {
        if (!file.is_open()) {
            std::cerr << "File is not open!" << std::endl;
            return "";
        }
    

        buffer.clear();
        char c;
        int count = 0;
        file.seekg(position - chunkSize + 1);
        while (count < chunkSize && file.get(c)) {
            buffer.push_back(c);
            count++;
        }
        
        // this line is needed to update the failbit that is triggered when EOF is reached
        file.get(c); 

        if(file.tellg() == -1){
            reachedEOF = true;
        }
        position+=1;        
        
        //printf("\nbuffer:%s\tposition:%ld",buffer.c_str(),position);

        return buffer;
    }

    bool hasMoreData() {
        return !reachedEOF;
    }
    
    long getPosition() {
        return position;
    }

    // returns '\0' if the position does not contain a character
    char getChar(long pos){
        char c;
        file.seekg(pos);
        file.get(c);
        if(c == std::ifstream::traits_type::eof()){
            c = '\0';
        }
        return c;
    }
};