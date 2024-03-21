#include <unordered_map>
#include <cstdio>
#include <unordered_set>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <queue>
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

    double run(bool logs=true){
        long length = data.length();
        long pointer = 0;
        long last=0;
        std::string chunk;
        while(getChunk(pointer,chunk)){  
                pointer = copy(pointer,length,chunk,last,logs);
                //writeIterationData(pointer);
                //resetHashTables();
                resetHashTables();
        }
        return bits_total/(1024*1024*8);
    }

    void cleanFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }
        file.close();
    }

    void writeIterationData(long iteration, double currTreshold, long totalMisses, 
    long totalTries, std::string symbol_in, std::string copy_char,bool miss, bool logs){
        
        double pHit;
        double bits;
        std::ofstream outputFile;
        std::string outputData;
        if (logs){
            
            std::ofstream outputFile("results.txt", std::ios_base::app);
            if (!outputFile.is_open()) {
                std::cerr << "Error opening file!" << std::endl;
                exit(1);
            }

            outputData =std::to_string(iteration)+"\t\t\tHits\t\tMisses\t\tTries\t\tProb(H)\t\tEAI"+"\t\t Predict:"+symbol_in+"\t\tSymbol : "+copy_char+"\n";
        
        }
        
        
         
        double pHit_s = prob_hit(totalTries-totalMisses,totalMisses);
        double bits_it ;
        if(copy_char==symbol_in){
            bits_it =  eai(pHit_s);
        }else{
            bits_it = eai((1-pHit_s)/(uniqueSymbols.size()-1));
        }
        bits_total += bits_it;
        
        if (logs){
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
            
        }
    
        

        double totalProb = prob_hit(totalTries-totalMisses, totalMisses);
        totalTries++;
        if(miss)totalMisses++;
        if (logs){
             outputData += "Total\t\t" + std::to_string(totalTries-totalMisses) + "\t\t\t" + std::to_string(totalMisses)+ "\t\t\t" + std::to_string(totalTries)+ "\t\t\t" + std::to_string(totalProb) + "\t" + std::to_string(eai(totalProb)) + "\n";
            outputData += "current treshold:" + std::to_string(currTreshold) + "\n";
            outputData += "Accumulated bits:" + std::to_string(bits_it) + "\n\n";
            outputFile << outputData;
            outputFile.close();
        }
              
    }

    long copy(long pointer_in, long length,std::string& chunk_in,long& last,bool logs,std::string th_method="lat ten"){
        long pointer = pointer_in;
        long copyPointer = -1;
        long localMisses = 0;
        long localTries = 0;
        double bits_it;
        std::queue<long> last_ten;


        double currTreshold = 0;
        std::string chunk = chunk_in;
        std::string copyChar = "";
        while( (currTreshold<treshold || localTries <=10) && (pointer+1 < length)){
            std::string predictionChar = std::string(1,data[pointer]);
            tries[predictionChar] += 1;
            
            // before a the first sequence repetition
            if(copyPointer == -1){
                if(beforeRepetition(copyPointer,chunk,pointer,localMisses, currTreshold, localTries,
                 predictionChar, copyChar,th_method,last_ten,logs)) continue;
            }
            //after
            //printf("b-%ld",pointer);
            
            afterRepetition(copyPointer,chunk,pointer,localMisses, currTreshold, localTries,
            predictionChar, copyChar,th_method,last_ten,logs);
            progress(pointer,length,last);
        }
        return pointer;
    }

    bool beforeRepetition (long& copyPointer,std::string& chunk,long& pointer,long& localMisses,
    double& currTreshold, long& localTries, std::string& predictionChar,std::string& copyChar,
    std::string th_method,std::queue<long>& last_ten,bool logs){

        if (hashTable.find(chunk) == hashTable.end()) {
            hashTable[chunk] = pointer;
            writeIterationData(pointer,currTreshold,localMisses,localTries,predictionChar,copyChar,true,logs);
            localMisses+=1;
            localTries+=1;
            //printf("a-%ld",pointer);
            pointer+=1;
            getChunk(pointer,chunk);
            if (th_method == "last ten"){
                if(last_ten.size()==10)last_ten.pop();
                last_ten.push(0);
                currTreshold = static_cast<double>(last_ten.size())/(sumElements(last_ten)); 
            }else{
                currTreshold = static_cast<double>(localMisses)/(localTries); 
            }
            
            return true;
        }
        copyPointer = hashTable[chunk];
        hashTable[chunk] = pointer;
        return false;      
    }
    long sumElements(std::queue<long>q){
        int sum = 0;
        while(!q.empty())
        {
            sum+=q.front();
            q.pop();
        }
        return sum;
    }

    void afterRepetition(long& copyPointer,std::string& chunk,long& pointer,long& localMisses,double& currTreshold, long& localTries,
     std::string& predictionChar,std::string& copyChar,std::string th_method,std::queue<long>& last_ten,bool logs){
        copyChar = std::string(1,data[copyPointer]);
        
        if(copyChar == predictionChar){
            hits[predictionChar] += 1;
            writeIterationData(pointer,currTreshold,localMisses,localTries,predictionChar,copyChar,false,logs);
            if (th_method == "last ten"){
                if(last_ten.size()==10)last_ten.pop();
                last_ten.push(1);
                currTreshold = static_cast<double>(last_ten.size())/(sumElements(last_ten)); 
            }else{
                currTreshold = static_cast<double>(localMisses)/(localTries); 
            }
        }else{
            writeIterationData(pointer,currTreshold,localMisses,localTries,predictionChar,copyChar,true,logs);
            localMisses+=1;
            if (th_method == "last ten"){
                if(last_ten.size()==10)last_ten.pop();
                last_ten.push(0);
                currTreshold = static_cast<double>(last_ten.size())/(sumElements(last_ten)); 
            }else{
                currTreshold = static_cast<double>(localMisses)/(localTries); 
            }
        }
        localTries+=1;
        pointer +=1;
        copyPointer+=1;   
        getChunk(pointer,chunk);
    }
    
    void progress(long pointer,long length, long& last){
        long percentage = (pointer*100) / length;
            if(std::fmod(percentage, 5.0)== 0.0 &&  last!=percentage){
                std::cout << "\n"+std::to_string(percentage)<< std::endl;
            }
            last = percentage;
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