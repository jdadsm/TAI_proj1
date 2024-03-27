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
        std::vector<char> previous_symbols; // Buffer to store previous symbols
        srand((unsigned) time(NULL));
        for (char symbol : data){
            // Exclude previous symbols from uniqueSymbols set
            std::set<char> eligible_symbols = uniqueSymbols;
            for (char prev_symbol : previous_symbols) {
                eligible_symbols.erase(prev_symbol);
            }

            int mutator = 1 + rand() % 100;
            int mutate_ch = int(mutate_chance * 100);
            if (mutator > mutate_ch){
                file << symbol; // Keep the original symbol
            } else {
                if (eligible_symbols.empty()) {
                    // If there are no eligible symbols, keep the original symbol
                    file << symbol;
                } else {
                    // Select a random symbol from the eligible set for mutation
                    int mutate_key = rand() % eligible_symbols.size();
                    auto l_front = eligible_symbols.begin();
                    std::advance(l_front, mutate_key);
                    file << *l_front;
                }
            }

            // Update previous_symbols buffer
            if (previous_symbols.size() == 2) {
                // Keep the buffer size limited to 2 to store only the previous two symbols
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
