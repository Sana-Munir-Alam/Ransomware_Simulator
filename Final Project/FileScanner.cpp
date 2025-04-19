#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>  // for getenv

using namespace std;
namespace fs = std::filesystem;

namespace FileScanner{
    // LOCAL FUCNTION ONLY VISIBLE IN THIS FILE

    // Function to check if a file should be skipped
    bool shouldSkipFile(const fs::path& filePath) {
        string fileName = filePath.filename().string();
        
        // Skip temporary lock files (e.g., ~$)
        if (fileName.rfind("~$", 0) == 0) {
            return true;
        }
        // Skip .DS_Store files (used by macOS)
        if (fileName == ".DS_Store") {
            return true;
        }
        // Skip everything inside .vscode folder
        for (const auto& part : filePath) {
            if (part == ".vscode") {
                return true;
            }
        }
        // Skip everything inside Project folder
        for (const auto& part : filePath) {
            if (part == "Project") {
                return true;
            }
        }
        // Skip other .json files if necessary
        if (fileName.rfind(".json", 0) == fileName.length() - 5) {
            return true;
        }
        // We can add more checks here if we have other types of files to skip
        return false;
    }

    void scanDesktopAndSave(const string& outputFilename){
        // Get HOME environment variable for macOS/Linux
        const char* homeDir = getenv("HOME");
        if (!homeDir) {
            cerr << "Could not find HOME environment variable.\n";
            return;
        }
    
        // Construct path to Desktop
        string desktopPath = string(homeDir) + "/Desktop";
    
        // Output file in current directory
        ofstream outFile("information2.txt");
        if (!outFile.is_open()) {
            cerr << "Failed to open information2.txt for writing.\n";
            return;
        }
    
        try {
            for (const auto& entry : fs::recursive_directory_iterator(desktopPath)) {
                if (fs::is_regular_file(entry.path())) {
                    // If file should be skipped, move to the next iteration
                    if (shouldSkipFile(entry.path())) {
                        continue;
                    }
    
                    // Get the path relative to the Desktop, e.g. "Folder1/Subfolder/File.txt"
                    fs::path relativePath = fs::relative(entry.path(), desktopPath);
                    outFile << relativePath.string() << "\n";
                }
            }
        } catch (const fs::filesystem_error& e) {
            cerr << "Filesystem error: " << e.what() << '\n';
        }
    
        outFile.close();
        cout << "Done! File paths saved to information2.txt\n";
    }
}