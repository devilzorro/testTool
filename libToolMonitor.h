#include <vector>

using namespace std;

extern "C" {
    int initial(char *filePath,char *configPath,vector<vector<double>> &limit,vector<vector<double>> &countnum,vector<vector<double>> &load_ago);
    char *calculate(int loopMark,int sequence, int load, double timedata,int toolnum,vector<vector<double>> &limit,vector<vector<double>> &countnum,vector<vector<double>> &load_ago);
}