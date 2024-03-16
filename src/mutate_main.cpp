#include "mutate.cpp"

int main(int argc, char* argv[]) {
    std::string filename = "";
    double mutate_chance;
    if(argc == 2){
        filename = argv[1];
        mutate_chance = 0.5;
    } else if(argc == 3){
        filename = argv[1];
        mutate_chance = std::stof(argv[2]);
    } else {
        printf("Usage: %s <filename> [mutate chance]\n", argv[0]);
        exit(1);
    }
    //replace with mutate later
    Mutate mut = Mutate(filename,mutate_chance);
    mut.run();
    
    printf("\n");
    return 0;
}