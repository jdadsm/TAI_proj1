#include "cpm.cpp"
#include <vector>

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
    std::string filename = "";
    int chunkSize = 0;
    double threshold = 0.2;
    bool logs = true;
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
    }else if (argc == 5){
        filename = argv[1];
        chunkSize = std::stoi(argv[2]);
        threshold = std::stof(argv[3]);
        if(std::stoi(argv[4]) == 1){
            logs = true;
        }else if(std::stoi(argv[4]) == 0){
            logs = false;
        }else{
            exit(2);
        }
    } else {
        printf("Usage: %s <filename> [chunkSize] [threshold] [logs]\n", argv[0]);
        exit(1);
    }
    
    ofstream outFile("grid_search_iterations.txt");
    if (!outFile.is_open()) {
        cout << "Error opening file for writing." << endl;
        exit(1);
    }
    //CopyModel copymodel = CopyModel(filename,chunkSize,threshold,"last");

    //double size = copymodel.run(logs);
    //printf("%f",size);
    
    vector<long> chunkSizes = linspace_int(2,15);
    vector<std::string> methods = {"last ten"};
    vector<double> thresholds = linspace(0.05,1,20);

    for (std::string Imethod: methods){
        for (long Ichunksize : chunkSizes) {
            for (double Ithreshold : thresholds) {
                CopyModel copymodel = CopyModel(filename,Ichunksize,Ithreshold,Imethod);
                double size = copymodel.run(logs);
                cout << "Method: " << Imethod << ", Chunksize: " << Ichunksize << ", Threshold: " << Ithreshold << ", Size: " << size << endl;
                outFile << "Method: " << Imethod << ", Chunksize: " << Ichunksize << ", Threshold: " << Ithreshold << ", Size: " << size << endl;
            }
        }
    }
 
    outFile.close();
    
    printf("\n");
    return 0;
}
 