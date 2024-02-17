#include <iostream>
#include <fstream>
#include <string>

class Generator {
private:
    std::ifstream file;
    std::string buffer;
    long position;
    int chunkSize;

public:
    Generator(const std::string& filename, int chunkSize = 4) : file(filename), chunkSize(chunkSize) {
        position = 0;
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
        while (count < chunkSize && file.get(c)) {
            buffer.push_back(c);
            count++;
        }

        file.seekg(position + 1);
        position = file.tellg();

        return buffer;
    }

    bool hasMoreData() const {
        return position != -1;
    }
};


int main() {

    const std::string filename = "../../example/tiny_example.txt";
    Generator generator(filename);

    while(generator.hasMoreData()){
        std::string dnaChunk = generator.getNextChunk();
        std::cout << dnaChunk << std::endl;
    }
    
    return 0;
}
