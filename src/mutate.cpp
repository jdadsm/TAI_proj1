#include <cstdio>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <set>

class Mutate{
private:
    std::set<char> uniqueSymbols;
    double mutate_chance;
    std::string data;
    std::ifstream inputFile;
    
public:
    
    Mutate(std::string& filename, double mutate_chance): inputFile(filename), mutate_chance(mutate_chance){
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
        }
        loadData();
        getUniqueSymbols(data);
        cleanFile("mutated.txt");
    }
    
    void loadData(){
        std::string line;
        while (std::getline(inputFile, line)) {
            data += line;
        }
        inputFile.close();
    }
    
    void getUniqueSymbols(std::string data) {
        for (char symbol : data){
            uniqueSymbols.insert(symbol);
        }   
    }
    
    void run(){
        std::ofstream file("mutated.txt");
        char latest_symbol;
        srand((unsigned) time(NULL));
        for (char symbol : data){
            if (latest_symbol){
                uniqueSymbols.insert(latest_symbol);
            }
            latest_symbol = symbol;
            uniqueSymbols.erase(latest_symbol);
            int mutator = 1 + rand() % 100;
            int mutate_ch = int(mutate_chance * 100);
            if (mutator > mutate_ch){
                file << symbol;
                continue;
            }
            else{
                int mutate_key = rand() % uniqueSymbols.size();
                auto l_front = uniqueSymbols.begin();
                std::advance(l_front, mutate_key);
                file << *l_front;

            }

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
    
};
