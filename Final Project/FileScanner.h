#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <string>
using namespace std;

namespace FileScanner {
    void scanDesktopAndSave(const string& outputFilename);  // This function scans all files on the user's Desktop and writes their absolute paths to a file.
}

#endif // FILESCANNER_H
