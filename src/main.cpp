#include "cpm.cpp"

int main(int argc, char* argv[]) {
    std::string filename = "";
    int chunkSize = 0;
    double threshold = 0.5;
    if(argc == 2){
        filename = argv[1];
        chunkSize = 4;
    } else if(argc == 3){
        filename = argv[1];
        chunkSize = std::stoi(argv[2]);
    }else if (argc == 4){
        filename = argv[1];
        chunkSize = std::stoi(argv[2]);
        threshold = std::stof(argv[3]);
    } else {
        printf("Usage: %s <filename> [chunkSize] [threshold]\n", argv[0]);
        exit(1);
    }
    
    CopyModel copymodel = CopyModel(filename,chunkSize,threshold,"last");
    copymodel.run();
    
    printf("\n");
    return 0;
}