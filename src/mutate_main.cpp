#include "mutate.cpp"

using namespace std;
int main(int argc, char* argv[]) {
    std::string filename = "../example/tiny_example.txt";
    double mutate_chance = 0.5;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-m" && i + 1 < argc) {
            mutate_chance = std::stof(argv[++i]);
        }
    }

    Mutate mut = Mutate(filename,mutate_chance);
    mut.run();
    
    printf("\n");
    return 0;
}