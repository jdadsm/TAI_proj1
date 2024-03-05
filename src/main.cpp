#include "cpm.cpp"

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
    
    CopyModel copymodel = CopyModel(filename,chunkSize,"last");
    copymodel.run();
    
    printf("\n");
    return 0;
}