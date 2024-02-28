#include <unordered_map>
#include <cstdio>
#include <vector>
#include <cmath>
#include "generator.cpp"

class CopyModel{
private:
    long hits;
    long misses;
    long tries;

    std::string method;
    std::unordered_map<std::string, std::string> hashTable;
    std::ofstream outputFile;
    std::string probs;
    Generator generator;
    

public:
    
    CopyModel(std::string& filename, int chunkSize,std::string method="last"): generator(filename,chunkSize,method), method(method){
        probs = "";
        hits = 0;
        misses = 0;
        tries = 0;
        
    }

    long getHits(){
        return hits;
    }

    long getMisses(){
        return misses;
    }

    long getTries(){
        return tries;
    }

    void fillHash(){
        //printf("\nmethod:%s",method.c_str());
        int chunkSize = generator.getChunkSize();
        
        //printf("\nchunkSize:%d",chunkSize);

        if (method == "last"){
            while(generator.hasMoreData()){
                std::string chunk = generator.getNextChunk();
                //int position = generator.getPosition()-generator.getChunkSize();
                
                std::string key = chunk.substr(0,chunkSize-1);
                std::string value = chunk.substr(chunkSize - 1, 1);
                //printf("\n%s:%s",key.c_str(),value.c_str());
                std::string prediction = "";
                if(hashTable.count(key) > 0){
                    prediction = hashTable[key];
                }
                updateStats(prediction,value);
                hashTable[key] = value;
                
            }
        }
        
        for (auto it = hashTable.begin(); it != hashTable.end(); ++it) {
            printf("\n%s:%s",it->first.c_str(),it->second.c_str());
        }

        //printf("\ntries:%ld\nhits:%ld\nmisses:%ld",tries,hits,misses);
    }

    void updateStats(std::string prediction, std::string copied){
        if(prediction == copied){
            hits+=1;
        }else{
            misses+=1;
        }
        tries+=1;
        double prob = (prob_hit(hits,misses));
        probs += std::to_string(prob)+"   "+std::to_string(eai(prob))+"\n";
        printf("\n%f",(prob_hit(hits,misses)));

        if(tries%1000==0){
            writeResults();
            probs="";
        }
    }
    
    double prob_hit(long hits, long misses, int alpha = 1){
        printf("\nhits:%ld",hits);
        printf("\nmisses:%ld",misses);
        return static_cast<double>(hits+alpha)/(hits+misses+2*alpha);
    }

    //returns estimated amount of information for a symbol
    double eai(double probability){
        return -log2(probability);
    }

    void writeResults(){
        std::ofstream outputFile("results.txt", std::ios_base::app);
        //outputFile.open("results.txt");

        if (!outputFile.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            exit(1);
        }

        outputFile << probs;

        outputFile.close();
    }

};