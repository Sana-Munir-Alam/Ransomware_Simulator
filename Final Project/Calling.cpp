/*
▗▄▄▖ ▗▄▄▄▖▗▖  ▗▖ ▗▄▖ ▗▄▄▖▗▖  ▗▖    ▗▖ ▗▖ ▗▄▖ ▗▄▄▖ ▗▖ ▗▖
▐▌ ▐▌  █  ▐▛▚▖▐▌▐▌ ▐▌▐▌ ▐▌▝▚▞▘     ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌▐▌▗▞▘
▐▛▀▚▖  █  ▐▌ ▝▜▌▐▛▀▜▌▐▛▀▚▖ ▐▌      ▐▌ ▐▌▐▌ ▐▌▐▛▀▚▖▐▛▚▖ 
▐▙▄▞▘▗▄█▄▖▐▌  ▐▌▐▌ ▐▌▐▌ ▐▌ ▐▌      ▐▙█▟▌▝▚▄▞▘▐▌ ▐▌▐▌ ▐▌
*/
#include "encryption.h"
#include "Calling.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>  // Needed for srand() and rand()
#include <ctime>    // Needed for time()

using namespace std;

bool isTextFile(const string& filePath) {
    size_t dotPos = filePath.find_last_of('.'); // Find the last dot to get the extension
    
    // If there's no dot or it's the last character, it's invalid
    if (dotPos == string::npos || dotPos == filePath.length() - 1) {    // npos is a built in variable of library string that is in simple words -1
        return false;
    }
    string extension = filePath.substr(dotPos);     // Extract extension
    return extension == ".txt";                     // Compare it to ".txt" to check if it's TRUE or FALSE
}

int ReadEncryptionType(const string& filename) {   // This function reads the keylog.txt file inorder to get the proper decryption class
    ifstream KeyLog("keylog.txt");  // The File is read which keeps the log of File_Path|Key|Encryption_Type

    vector<tuple<string, string, int>> KeyInfo;     // A variable to store the Files Info being read from the file in the syntax File|Key|Type
    int EncryptionType = 0;                         // A variable to store the Encryption Type being read from file
    bool found = false;

    string Line;

    while (getline(KeyLog, Line)) {
        istringstream iss(Line);
        string filePart, keyPart, typePart;
        size_t pos1 = Line.find('|');
        size_t pos2 = Line.rfind('|');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = Line.substr(0, pos1);
            keyPart = Line.substr(pos1 + 1, pos2 - pos1 - 1);
            typePart = Line.substr(pos2 + 1);

            int type = stoi(typePart);
            KeyInfo.emplace_back(filePart, keyPart, type);

            if (filePart == filename && type == 1) { // Check for Vigenère entries
                EncryptionType = type;
                found = true;
                break;
            } else if (filePart == filename && type == 2){ // Check for Ceaser entries
                EncryptionType = type;
                found = true;
                break;
            } else if (filePart == filename && type == 3){ // Check for Railfence entries
                EncryptionType = type;
                found = true;
                break;
            }
        }
    }
    KeyLog.close();
    return EncryptionType; // Returning Encryption Type
}

// THIS FUNCTION WILL CALL FOR  THE OOP BASED ENCRYPTION CLASSES TO ENCRYPT TXT AND NON TXT FILES PATHWAYS
void ProcessEncryption(const string& Path, int offset){
    // Will need to add random generator to choose encryption, use switch case through which i will handle object creation and encryption
    /*
    1- Vignere Encryption
    2- Ceaser Encryption
    3- RailFence Encryption
    */
    srand(time(0) + offset);
    int EncryptionTypeText = (rand() % 3) + 1;      // This is for Text File
    int EncryptionTypeNonText = (rand() % 2) + 2;   // This is for Non-Text Files

    if(isTextFile(Path)){   // Firstly check if the path we are getting is of text file or not
        switch (EncryptionTypeText) {
            case 1:{
                VigenereEncryption vigEnc; // CREATING VIGNERÉ ENCRYPTION OBJECT
                vigEnc.encryptTXT(Path);
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }case 2:{
                CeaserCipher caesarEnc; // CREATING CEASER ENCRYPTION OBJECT
                caesarEnc.encryptTXT(Path);
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }case 3:{
                RailFenceEncryption railEnc; // CREATING RAILFENCE ENCRYPTION OBJECT
                railEnc.encryptTXT(Path);
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }
        }
    }else{      // The current Path is not a Text File
        switch (EncryptionTypeNonText) {
            case 2:{
                CeaserCipher caesarEnc; // CREATING CEASER ENCRYPTION OBJECT
                caesarEnc.encryptNON(Path);
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeNonText << endl;
                break;
            }case 3:{
                RailFenceEncryption railEnc; // CREATING RAILFENCE ENCRYPTION OBJECT
                railEnc.encryptNON(Path);
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeNonText << endl;
                break;
            }
        }
    }
}

// THIS FUNCTION WILL CALL FOR THE OOP BASED DECRYPTION CLASSED TO DECRYPT TXT AND NON-TXT FILES PATHWAYS
void ProcessDecryption(const string& Path){
    /*
    1- Vignere Encryption
    2- Ceaser Encryption
    3- RailFence Encryption
    */
    // Using A function to get the Encryption type in order to call the proper decryption class.
    int DecryptionType = ReadEncryptionType(Path);      // Sending File name to get the Encryption Type
    int DecryptionTypeNON = ReadEncryptionType(Path);   // Sending File name to get the Encryption Type
    if(isTextFile(Path)){
        switch (DecryptionType) {
            case 1:{
                VigenereEncryption vigEnc;  //Creating Object for Decryption
                vigEnc.decryptTXT(Path);
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }case 2:{
                CeaserCipher caesarEnc; //Creating Object for Decryption
                caesarEnc.decryptTXT(Path);
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }case 3:{
                RailFenceEncryption railEnc; //Creating Object for Decryption
                railEnc.decryptTXT(Path);
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }
        }
    }else{
        // Apppend ".enc" to the original path
        string encryptedPath = Path + ".enc";
        switch (DecryptionTypeNON) {
            case 2:{
                CeaserCipher caesarEnc; //Creating Object for Decryption
                caesarEnc.decryptNON(encryptedPath);
                cout << "Decrypting " << encryptedPath << " using type " << DecryptionTypeNON << endl;
                break;
            }case 3:{
                RailFenceEncryption railEnc; //Creating Object for Decryption
                railEnc.decryptNON(encryptedPath);
                cout << "Decrypting " << encryptedPath << " using type " << DecryptionTypeNON << endl;
                break;
            }
        }
    }
}

// THIS FUNCTION IS TECHNICALLY THE MAIN OF THIS .cpp file
void RunEncryptionHandler(int Number) {
    ifstream FileList("Information.txt");
    if (!FileList.is_open()) {
        cerr << "Could not open Information.txt." << endl;
        return;
    }

    vector<string> filePaths;
    string line;
    while (getline(FileList, line)) {
        if (!line.empty()) filePaths.push_back(line);
    }
    FileList.close();

    if (filePaths.empty()) {
        cerr << "No file paths found in Information.txt." << endl;
        return;
    }

    int offset = 0;
    if (Number == 1) {
        for (const string& Path : filePaths) {
            ProcessEncryption(Path, offset++);
        }
    } else if (Number == 2) {
        for (const string& Path : filePaths) {
            ProcessDecryption(Path);
        }
    } else {
        cerr << "Invalid option passed to RunEncryptionHandler. Use 1 for encryption or 2 for decryption." << endl;
    }
}