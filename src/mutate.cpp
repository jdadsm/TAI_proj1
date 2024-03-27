#include <cstdio>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <set>
#include <vector>

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
        cleanFile("../example/mutated.txt");
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
        std::ofstream file("../example/mutated.txt");
        std::vector<char> previous_symbols;
        srand((unsigned) time(NULL));
        for (char symbol : data){
            std::set<char> eligible_symbols = uniqueSymbols;
            for (char prev_symbol : previous_symbols) {
                eligible_symbols.erase(prev_symbol);
            }

            int mutator = 1 + rand() % 100;
            int mutate_ch = int(mutate_chance * 100);
            if (mutator > mutate_ch){
                file << symbol;
            } else {
                if (eligible_symbols.empty()) {
                    file << symbol;
                } else {
                    int mutate_key = rand() % eligible_symbols.size();
                    auto l_front = eligible_symbols.begin();
                    std::advance(l_front, mutate_key);
                    file << *l_front;
                }
            }

            if (previous_symbols.size() == 2) {
                previous_symbols.erase(previous_symbols.begin());
            }
            previous_symbols.push_back(symbol);
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
