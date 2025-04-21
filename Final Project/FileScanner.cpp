#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>  // for getenv [it means get environment]

using namespace std;
namespace fs = std::filesystem;

namespace FileScanner{  // Using Namespace as a safeside so that no function name can be same as we have multiple cpp files, not doing it can result in logic collison.

    // FUNCTION TO CHECK WHETHER A FILE IS TO BE SKIPPED
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
        // Skip everything inside Project folder [CAUTION: THIS FOLDER CONTAINS ARE MAIN CODE SO IT HAS TO REMAIN SAFE]
        for (const auto& part : filePath) {
            if (part == "Project") {
                return true;
            }
        }
        // Skip other .json files if necessary
        if (fileName.rfind(".json", 0) == fileName.length() - 5) {
            return true;
        }
        // Skip .extinfo files
        if (fileName == ".extinfo") {
            return true;
        }
        // We can add more checks here if we have other types of files to skip
        return false;
    }

    // FUNCTION TO SCAN THE FILES ON DESKTOP AND SAVE IT IN A FILE.
    void scanDesktopAndSave(const string& outputFilename){
        // Get HOME environment variable for macOS/Linux
        const char* homeDir = getenv("HOME");                        // Get the user's home directory path from environment variables (e.g., /Users/yourname)
        if (!homeDir) {                                              // If the HOME variable is not found (unlikely but possible), show an error
            cerr << "Could not find HOME environment variable.\n";   // Print an error.
            return;                                                  // Exit the function early.
        }
    
        string desktopPath = string(homeDir) + "/Desktop";           // Construct the full path to the Desktop by appending "/Desktop" to the home path Eg: User/yourname/Desktop
    
        ofstream outFile("Information2.txt");                         // Create a file named Information2.txt which will contain the File pathways on Desktop
        if (!outFile.is_open()) {                                     // Check if the file couldn't be opened
            cerr << "Failed to open information2.txt for writing.\n"; // Print an Error
            return;                                                   // Exit the function.
        }
    
        try {
            for (const auto& entry : fs::recursive_directory_iterator(desktopPath)) {   // Recursively iterate through all files and folders on Desktop.
                if (fs::is_regular_file(entry.path())) {                                // Check if the current entry is a regular file (not a directory or symlink).
                    if (shouldSkipFile(entry.path())) {                                 // Use our custom logic to skip unnecessary/hidden/system files.
                        continue;                                                       // Skip to the next file.
                    }
                    outFile << entry.path().string() << "\n";   // Write the Absolute Path of the valid file to Information2.txt E.g User/yourname/Desktop/folder_names
                }
            }
        } catch (const fs::filesystem_error& e) {               // Catch any filesystem-related exceptions (e.g., permission denied).
            cerr << "Filesystem error: " << e.what() << '\n';   // Print error message describing what went wrong.
        }
        outFile.close();                                        // Close the output file safely.
        cout << "Done! File paths saved to information2.txt\n"; // Notify the programmer that the process is complete.
    }
}
