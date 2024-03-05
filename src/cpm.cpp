#include <unordered_map>
#include <cstdio>
#include <unordered_set>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>

class CopyModel{
private:
    std::unordered_map<std::string, long> hits;
    std::unordered_map<std::string, long> tries;
    std::unordered_map<std::string, long> hashTable;
    std::unordered_set<std::string> uniqueSymbols;

    int chunkSize;
    double treshold;
    std::string method;
    std::string data;
    std::ifstream inputFile;
    

public:
    
    CopyModel(std::string& filename, int chunkSize,std::string method="last"): inputFile(filename), chunkSize(chunkSize), method(method){
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
        loadData();
        getUniqueSymbols(data);
        resetHashTables();
        cleanFile("results.txt");
        treshold = 0.5;
    }

    void loadData(){
        std::string line;
        while (std::getline(inputFile, line)) {
            data += line;
        }
        inputFile.close();
    }

    void resetHashTables(){
        for(std::string symbol: uniqueSymbols){
            hits[symbol] = 0;
            tries[symbol] = 0;
        }
    }

    void getUniqueSymbols(std::string data) {
        for (char symbol : data){
            uniqueSymbols.insert(std::string(1, symbol));
        }   
    }

    void run(){
        long length = data.length();
        long pointer = 0;
        std::string chunk;
        while(getChunk(pointer,chunk)){
            if ((hashTable.find(chunk)) == hashTable.end()){
                hashTable[chunk] = pointer;    
            }else{
                if(pointer+chunkSize<length){
                    copy(hashTable[chunk]+chunkSize,pointer+chunkSize,length);
                    writeIterationData(pointer);
                    resetHashTables();
                }
            }
            pointer+=1;
        }

        for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
            //printf("\n%s:%ld",it->first.c_str(),it->second);
        }

    }

    void cleanFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        file.close();
    }

    void writeIterationData(long iteration){
        std::ofstream outputFile("results.txt", std::ios_base::app);

        if (!outputFile.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            exit(1);
        }
        
        std::string outputData =std::to_string(iteration)+"\t\t\tHits\t\tMisses\t\tTries\t\tProb(H)\t\tEAI\n";

        for (std::string symbol : uniqueSymbols){
            double pHit = prob_hit(hits[symbol],tries[symbol]-hits[symbol]);
            double bits = eai(pHit);
            outputData+= "\t\t\t" + std::to_string(hits[symbol]) + "\t\t\t" + std::to_string(tries[symbol]-hits[symbol]) + "\t\t\t"+std::to_string(tries[symbol]) +
                         "\t\t\t" + std::to_string(pHit) + "\t" + std::to_string(bits) + "\n";
        }

        outputFile << outputData;
        outputFile.close();
    }

    void copy(long copyPointer, long predictionPointer, long length){
        double currTreshold = 0;
        long localMisses = 0;
        long localTries = 0;
        int i=0;
        while( (currTreshold<treshold || localTries <=5) && (predictionPointer+i < length) && (copyPointer+i < predictionPointer)){
            std::string copyChar = std::string(1,data[copyPointer+i]);
            std::string predictionChar = std::string(1,data[predictionPointer+i]);
            tries[predictionChar] += 1;
            localTries+=1;
            if(copyChar == predictionChar){
                hits[predictionChar] += 1;
            }else{
                localMisses+=1;
            }
        
            currTreshold = static_cast<double>(localMisses)/(localTries);
            i+=1;
        }
    }

    bool getChunk(long pointer, std::string& chunk){
        chunk = data.substr(pointer,chunkSize);
        return chunk.length() == chunkSize;
    }
    
    double prob_hit(long hits, long misses, int alpha = 1){
        return static_cast<double>(hits+alpha)/(hits+misses+2*alpha);
    }

    //returns estimated amount of information for a symbol
    double eai(double probability){
        return -log2(probability);
    }

};