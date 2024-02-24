#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdio>
#include <vector>
#include <cmath>

// class to prevent loading the whole file into memory
class Generator {
private:
    std::ifstream file;
    std::string buffer;
    long position;
    int chunkSize;
    bool reachedEOF;

public:
    Generator(std::string& filename, int chunkSize) : file(filename), chunkSize(chunkSize) {
        position = chunkSize-1;
        reachedEOF = false;
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
    }

    ~Generator() {
        file.close();
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

class CopyModel{
private:
    long hits;
    long misses;
    long tries;
    std::string method;
    std::unordered_map<std::string, int> hashTable;
    Generator generator;

public:
    
    CopyModel(std::string& filename, int chunkSize,std::string method="last"): generator(filename,chunkSize), method(method){
        hits = 0;
        misses = 0;
        tries = 0;
        if(method == "last"){
            //printf("\nlast method");
        }
    }

    void fillHash(){
        //printf("\nmethod:%s",method.c_str());
        if (method == "last"){
            while(generator.hasMoreData()){
                std::string chunk = generator.getNextChunk();
                int position = generator.getPosition();
                
                printf("\n%s-%c:%d",chunk.c_str(),generator.getChar(position),position);

                if(hashTable.count(chunk) != 0){ // if it is not the first time the chunk has appeared
                    if(predictionIsCorrect(hashTable[chunk],position)){
                        hits+=1;
                    }else{
                        misses+=1;
                    }
                }
                tries+=1;

                hashTable[chunk] = position;
            }
        }
        
        for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
            //printf("\n%s:%d",it->first.c_str(),it->second);
        }

        printf("\ntries:%ld\nhits:%ld\nmisses:%ld",tries,hits,misses);
    }

    bool predictionIsCorrect(long lastPosition, long predictionPosition){
        char lastChar = generator.getChar(lastPosition);
        char predictionChar = generator.getChar(predictionPosition);
        return lastChar == predictionChar;
    }
    
    double prob_hit(int hits, int misses, int alpha = 2){
        return (hits+alpha)/(hits+misses+2*alpha);
    }

    //returns estimated amount of information for a symbol
    double eai(double probability){
        return -log2(probability);
    }

};


int main(int argc, char* argv[]) {
    std::string filename = "";
    int chunkSize = 0;
    if(argc == 2){
        filename = argv[1];
        chunkSize = 4;
    } else if(argc == 3){
        filename = argv[1];
        chunkSize = std::stoi(argv[2]);
    } else {
        printf("Usage: %s <filename> [chunkSize]\n", argv[0]);
        exit(1);
    }
    //printf("\nfilename:%s",filename.c_str());
    //printf("\nchunkSize:%d",chunkSize);
    CopyModel copymodel = CopyModel(filename,chunkSize);
    copymodel.fillHash();
    printf("\n");
    return 0;
}
