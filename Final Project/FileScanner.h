// FileScanner.h

#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <string>
using namespace std;

namespace FileScanner {
    // Techincally this function is being called as in the seperate file it was the main function and as we are combining it woth our project therefore the file could not have a main function hence we changed into a void function
    void scanDesktopAndSave(const string& outputFilename);
}

#endif // FILESCANNER_H