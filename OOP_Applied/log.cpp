#include "log.h"
#include <fstream>
using namespace std;

void LogAction(const string &action) {
    ofstream logFile("log.txt", ios::app);
    logFile << action << endl;
    logFile.close();
}