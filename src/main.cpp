#include "cpm.cpp"
#include <vector>

#include <chrono>
using namespace std;


std::vector<double> linspace(double start, double end, int numPoints) {
    std::vector<double> result;
    if (numPoints <= 1) {
        result.push_back(start);
        return result;
    }

    double step = (end - start) / (numPoints - 1);
    for (int i = 0; i < numPoints; ++i) {
        result.push_back(start + i * step);
    }
    return result;
}

std::vector<long> linspace_int(long start, long numPoints) {
    std::vector<long> result;
    long i = start;
    long k=0;
    while(i<numPoints){
        result.push_back(i);
        i++;
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::string filename = "../example/tiny_example.txt";
    std::string method = "last ten";
    int chunkSize = 11;
    double threshold = 0.1;
    bool logs = false;
    bool grid_search = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "-m" && i + 1 < argc) {
            method = argv[++i];
        } else if (arg == "-c" && i + 1 < argc) {
            chunkSize = std::stoi(argv[++i]);
        } else if (arg == "-t" && i + 1 < argc) {
            threshold = std::stod(argv[++i]);
        } else if (arg == "-l" && i + 1 < argc) {
            logs = (std::stoi(argv[++i]) == 1);
        } else if (arg == "-g" && i + 1 < argc) {
            grid_search = (std::stoi(argv[++i]) == 1);
        } 
    }

    ofstream outFile("grid_search_iterations.txt");
    if (!outFile.is_open()) {
        cout << "Error opening file for writing." << endl;
        exit(1);
    }

    if(!grid_search){
        
        CopyModel copymodel = CopyModel(filename,chunkSize,threshold,method);
        double size = copymodel.run(logs);
        printf("%f",size);

    }else{

        vector<long> chunkSizes = linspace_int(9,12);
        vector<std::string> methods = {"last ten"};
        vector<double> thresholds = linspace(0.05,1,20);

        for (std::string Imethod: methods){
            for (long Ichunksize : chunkSizes) {
                for (double Ithreshold : thresholds) {
                    auto start = std::chrono::high_resolution_clock::now();
                    CopyModel copymodel = CopyModel(filename,Ichunksize,Ithreshold,Imethod);
                    std::cout << logs << endl;
                    double size = copymodel.run(logs);
                    auto end = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                    cout << "Method: " << Imethod << ", Chunksize: " << Ichunksize << ", Threshold: " << Ithreshold << ", Size: " << size << ", Time: " << duration.count() << endl;
                    outFile << "Method: " << Imethod << ", Chunksize: " << Ichunksize << ", Threshold: " << Ithreshold << ", Size: " << size << ", Time: " << duration.count() << endl;
                }
            }
        }
    }

    outFile.close();
    printf("\n");

    return 0;
}
 