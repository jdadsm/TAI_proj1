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
    //printf("\nfilename:%s",filename.c_str());
    //printf("\nchunkSize:%d",chunkSize);
    CopyModel copymodel = CopyModel(filename,chunkSize,"last");
    copymodel.fillHash();

    printf("\nhits:%ld\nmisses:%ld\ntries:%ld",copymodel.getHits(),copymodel.getMisses(),copymodel.getTries());

    copymodel.writeResults();
    printf("\n");
    return 0;
}