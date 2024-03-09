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
    double bits_total=0;
    int chunkSize;
    double treshold;
    std::string method;
    std::string data;
    std::ifstream inputFile;
    

public:
    
    CopyModel(std::string& filename, int chunkSize, double treshold, std::string method="last"): inputFile(filename), chunkSize(chunkSize),treshold(treshold), method(method){
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
        loadData();
        getUniqueSymbols(data);
        resetHashTables();
        cleanFile("results.txt");
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
        long last=0;
        std::string chunk;
        while(getChunk(pointer,chunk)){  
                pointer = copy(pointer,length,chunk,last);
                //writeIterationData(pointer);
                //resetHashTables();
                resetHashTables();
            
            pointer+=1;
        }
        printf("\n%f",bits_total);
    }

    void cleanFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        file.close();
    }

    void writeIterationData(long iteration, double currTreshold, long totalMisses, long totalTries, std::string symbol_in, std::string copy_char){
        //std::ofstream outputFile("results.txt", std::ios_base::app);
        double pHit;
        double bits;
       /*
        if (!outputFile.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            exit(1);
        }
        
        std::string outputData =std::to_string(iteration)+"\t\t\tHits\t\tMisses\t\tTries\t\tProb(H)\t\tEAI"+"\t\t Predict:"+symbol_in+"\t\tSymbol : "+copy_char+"\n";
        */ 
        double pHit_s = prob_hit(totalTries-totalMisses,totalMisses);
        double bits_it ;
        if(copy_char==symbol_in){
            bits_it =  eai(pHit_s);
        }else{
            bits_it = eai((1-pHit_s)/(uniqueSymbols.size()-1));
        }
        bits_total += bits_it;
        /*
        for (std::string symbol : uniqueSymbols){
            if(symbol_in==symbol){
                pHit = pHit_s;
                bits = eai(pHit);
            }else{
                pHit = (1-pHit_s)/(uniqueSymbols.size()-1);
                bits = eai(pHit);
            }
            outputData += symbol + "\t\t\t" + std::to_string(hits[symbol]) + "\t\t\t" + std::to_string(tries[symbol]-hits[symbol]) + "\t\t\t"+std::to_string(tries[symbol]) +
                         "\t\t\t" + std::to_string(pHit) + "\t" + std::to_string(bits) +"\n";
        }
        */
        double totalProb = prob_hit(totalTries-totalMisses, totalMisses);
        /*
        outputData += "Total\t\t" + std::to_string(totalTries-totalMisses) + "\t\t\t" + std::to_string(totalMisses)+ "\t\t\t" + std::to_string(totalTries)+ "\t\t\t" + std::to_string(totalProb) + "\t" + std::to_string(eai(totalProb)) + "\n";
        outputData += "current treshold:" + std::to_string(currTreshold) + "\n";
        outputData += "Accumulated bits:" + std::to_string(bits_it) + "\n\n";
        outputFile << outputData;
        outputFile.close();
        */
        
    }

    long copy(long pointer_in, long length,std::string& chunk_in,long& last){
        long pointer = pointer_in;
        std::string chunk = chunk_in;
        double currTreshold = 0;
        long localMisses = 0;
        long localTries = 0;
        double bits_it ;
        long copyPointer = -1;
        std::string copyChar = "";
        while( (currTreshold<treshold || localTries <=5) && (pointer+1 < length)){
            std::string predictionChar = std::string(1,data[pointer]);
            tries[predictionChar] += 1;
            localTries+=1;
            // before a the first sequence repetition
            if(copyPointer == -1){
                if (hashTable.find(chunk) == hashTable.end()) {
                hashTable[chunk] = pointer;
                localMisses+=1;
                //printf("a-%ld",pointer);
                writeIterationData(pointer,currTreshold,localMisses,localTries,predictionChar,copyChar);
                pointer+=1;
                getChunk(pointer,chunk);
                continue;
                }else{
                copyPointer = hashTable[chunk];
                hashTable[chunk] = pointer;
                }   
            }
            //after
            //printf("b-%ld",pointer);
            std::string copyChar = std::string(1,data[copyPointer]);
            if(copyChar == predictionChar){
                hits[predictionChar] += 1;
            }else{
                localMisses+=1;
            }
            currTreshold = static_cast<double>(localMisses)/(localTries); 
            pointer +=1;
            copyPointer+=1;
            writeIterationData(pointer,currTreshold,localMisses,localTries,predictionChar,copyChar);
            getChunk(pointer,chunk);
            //progress
            long percentage = (pointer*100) / length;
            if(std::fmod(percentage, 5.0)== 0.0 &&  last!=percentage){
                std::cout << "\n"+std::to_string(percentage)<< std::endl;
            }
            last = percentage;
            
        }
        return pointer;
    }

    double bits_fail(std::string failed_symbol){
        double total = 0;
        for (std::string symbol : uniqueSymbols){
            if(symbol != failed_symbol){
                total += eai(prob_hit(hits[symbol],tries[symbol]-hits[symbol]));
            }
        }
        return total;
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