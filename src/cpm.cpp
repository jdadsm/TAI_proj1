#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstdio>
#include <vector>


class Generator {
private:
    std::ifstream file;
    std::string buffer;
    long position;
    int chunkSize;

public:
    Generator(std::string& filename, int chunkSize) : file(filename), chunkSize(chunkSize) {
        position = chunkSize-1;
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

        if(position == -1){
            return NULL;
        }

        buffer.clear();
        char c;
        int count = 0;
        file.seekg(position - chunkSize + 1);
        while (count < chunkSize && file.get(c)) {
            buffer.push_back(c);
            count++;
        }

        position=file.tellg();
        //printf("\nbuffer:%s\tposition:%ld",buffer.c_str(),position);

        return buffer;
    }

    bool hasMoreData() {
        return position!=-1;
    }
    
    long getPosition() {
        return position;
    }
};

class CopyModel{
private:
    std::string method;
    std::unordered_map<std::string, int> hashTable;
    Generator generator;

public:
    
    CopyModel(std::string& filename, int chunkSize,std::string method="last"): generator(filename,chunkSize), method(method){
        if(method == "last"){
            //printf("\nlast method");
        }
    }

    void fillHash(){
        //printf("\nfillHash");
        //printf("\nmethod:%s",method.c_str());
        if (method == "last"){
            while(true){
                std::string chunk = generator.getNextChunk();
                int position = generator.getPosition();
                if(!generator.hasMoreData()){
                    break;
                }
                hashTable[chunk] = position;
            }
        }
        
        for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
            printf("\n%s:%d",it->first.c_str(),it->second);
        }
    }
    
    double prob_hit(int hits, int misses, int alpha = 2){
        return (hits+alpha)/(hits+misses+2*alpha);
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
