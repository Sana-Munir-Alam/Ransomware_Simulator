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

// Function to check if the file is a .txt file
bool isTextFile(const string& filePath) {
    size_t dotPos = filePath.find_last_of('.');                         // Find the last dot to get the extension
    
    // If there's no dot or it's the last character, it's invalid
    if (dotPos == string::npos || dotPos == filePath.length() - 1) {    // npos is a built in variable of library string that is in simple words -1
        return false;
    }
    string extension = filePath.substr(dotPos);                         // Extract extension
    return extension == ".txt";                                         // Compare it to ".txt" to check if it's TRUE or FALSE
}

// Function to read the keylog.txt file inorder to call the proper decryption class methods
int ReadEncryptionType(const string& filename) {
    ifstream KeyLog("keylog.txt");                  // The File is read which keeps the log of File_Path|Key|Encryption_Type

    vector<tuple<string, string, int>> KeyInfo;     // A variable to store the Files Info being read from the file in the syntax File|Key|Type
    int EncryptionType = 0;                         // A variable to store the Encryption Type being read from file
    bool found = false;                             // Flag to track whether a matching file was found

    string Line;                                    // Temporary string to hold each line from the keylog file

    while (getline(KeyLog, Line)) {                 // Read the file line-by-line
        string filePart, keyPart, typePart;         // Strings to hold each part split by the '|' delimiter
        size_t pos1 = Line.find('|');               // Position of the first '|' found through command find which searches from Left/Beginning
        size_t pos2 = Line.rfind('|');              // Position of the last '|' found through command rfind which searches from Right/End

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {     // Check if the line is valid (has at least two '|'s) and than extract the substrings based on positions of '|'
            filePart = Line.substr(0, pos1);                                    // File path
            keyPart = Line.substr(pos1 + 1, pos2 - pos1 - 1);                   // Encryption key
            typePart = Line.substr(pos2 + 1);                                   // Encryption type as string

            int type = stoi(typePart);                                          // Convert typePart from string to integer
            KeyInfo.emplace_back(filePart, keyPart, type);                      // Store the parsed data into the KeyInfo vector

            if (filePart == filename && type == 1) {                            // Check for Vigenère entries
                EncryptionType = type;
                found = true;
                break;
            } else if (filePart == filename && type == 2){                      // Check for Ceaser entries
                EncryptionType = type;
                found = true;
                break;
            } else if (filePart == filename && type == 3){                      // Check for Railfence entries
                EncryptionType = type;
                found = true;
                break;
            } else if (filePart == filename && type == 4){                      // Check for Affine entries
                EncryptionType = type;
                found = true;
                break;
            }
        }
    }
    KeyLog.close();         // Close the key log file after reading
    return EncryptionType;  // Return the encryption type found (0 if no match was found)
}

// THIS FUNCTION WILL CALL FOR THE OOP BASED ENCRYPTION CLASSES TO ENCRYPT TXT AND NON TXT FILES PATHWAYS
void ProcessEncryption(const string& Path, int offset){
    /*
    1- Vignere Encryption
    2- Ceaser Encryption
    3- RailFence Encryption
    4- Affine Encryption
    */
    srand(time(0) + offset);                        // This ensures different files may get different encryption methods even when executed quickly
    int EncryptionTypeText = (rand() % 4) + 1;      // Randomly choose encryption type for text files [1-4]
    int EncryptionTypeNonText = (rand() % 3) + 2;   // Randomly choose encryption type for non-text files [2-4]
    if(isTextFile(Path)){                           // First, determine if the file at the provided path is a text file or not
        switch (EncryptionTypeText) {               // If it is a text file, choose an encryption method based on the randomly selected type
            case 1:{                                // CASE 1: Vigenère Cipher selected
                VigenereEncryption vigEnc;          // Create a Vigenère encryption object
                vigEnc.encryptTXT(Path);            // Call the encryption method for text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }case 2:{                               // CASE 2: Caesar Cipher selected
                CeaserCipher caesarEnc;             // Create a Caesar encryption object
                caesarEnc.encryptTXT(Path);         // Call the encryption method for text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }case 3:{                               // CASE 3: Rail Fence Cipher selected
                RailFenceEncryption railEnc;        // Create a Rail Fence encryption object
                railEnc.encryptTXT(Path);           // Call the encryption method for text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }case 4:{                               // CASE 4: Affine Cipher selected
                AffineEncryption affineEnc;         // Create a Affine encryption object
                affineEnc.encryptTXT(Path);         // Call the encryption method for text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeText << endl;
                break;
            }
        }
    }else{                                          // If it's not a text file (i.e., it's binary or other type), use the EncryptionTypeNonText
        switch (EncryptionTypeNonText) {
            case 2:{                                // CASE 2: Caesar Cipher selected
                CeaserCipher caesarEnc;             // Create a Caesar encryption object
                caesarEnc.encryptNON(Path);         // Call the encryption method for non-text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeNonText << endl;
                break;
            }case 3:{                               // CASE 3: Rail Fence Cipher selected
                RailFenceEncryption railEnc;        // Create a Rail Fence encryption object
                railEnc.encryptNON(Path);           // Call the encryption method for non-text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeNonText << endl;
                break;
            }case 4:{                               // CASE 4: Affine Cipher selected
                AffineEncryption affineEnc;         // Create a Affine encryption object
                affineEnc.encryptNON(Path);         // Call the encryption method for text files
                cout << "Encrypting " << Path << " with type: " << EncryptionTypeNonText << endl;
                break;
            }
        }
    }
}

// THIS FUNCTION WILL CALL FOR THE OOP BASED DECRYPTION CLASSES TO DECRYPT TXT AND NON-TXT FILES PATHWAYS
void ProcessDecryption(const string& Path){
    /*
    1- Vignere Encryption
    2- Ceaser Encryption
    3- RailFence Encryption
    4- Affine Encryption
    */
    // Retrieve the encryption type stored with or associated with this file path using ReadEncryptionType
    int DecryptionType = ReadEncryptionType(Path);      // For text files
    int DecryptionTypeNON = ReadEncryptionType(Path);   // For non-text files

    if(isTextFile(Path)){                               // First, determine if the file at the provided path is a text file or not
        switch (DecryptionType) {                       // If it is a text file, choose an decryption method based on the DecryptionType
            case 1:{                                    // CASE 1: Vigenère Cipher selected
                VigenereEncryption vigEnc;              // Create Vigenère decryption object
                vigEnc.decryptTXT(Path);                // Decrypt the text file
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }case 2:{                                   // CASE 2: Caesar Cipher selected
                CeaserCipher caesarEnc;                 // Create Caesar decryption object
                caesarEnc.decryptTXT(Path);             // Decrypt the text file
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }case 3:{                                   // CASE 3: Rail Fence Cipher selected
                RailFenceEncryption railEnc;            // Create Rail Fence decryption object
                railEnc.decryptTXT(Path);               // Decrypt the text file
                cout << "Decrypting " << Path << " using type " << DecryptionType << endl;
                break;
            }case 4:{                                   // CASE 4: Affine Cipher selected
                AffineEncryption affineEnc;             // Create a Affine decryption object
                affineEnc.decryptTXT(Path);             // Decrypt the text file
                cout << "Decrypting " << Path << " using type: " << DecryptionType << endl;
                break;
            }
        }
    }else{
        string encryptedPath = Path + ".enc";           // Apppend ".enc" to the original path
        switch (DecryptionTypeNON) {
            case 2:{                                    // CASE 2: Caesar Cipher selected
                CeaserCipher caesarEnc;                 // Create Caesar decryption object
                caesarEnc.decryptNON(encryptedPath);    // Decrypt the non-text file
                cout << "Decrypting " << encryptedPath << " using type " << DecryptionTypeNON << endl;
                break;
            }case 3:{                                   // CASE 3: Rail Fence Cipher selected
                RailFenceEncryption railEnc;            // Create Rail Fence decryption object
                railEnc.decryptNON(encryptedPath);      // Decrypt the non-text file
                cout << "Decrypting " << encryptedPath << " using type " << DecryptionTypeNON << endl;
                break;
            }case 4:{                                   // CASE 4: Affine Cipher selected
                AffineEncryption affineEnc;             // Create a Affine decryption object
                affineEnc.decryptNON(encryptedPath);    // Decrypt the text file
                cout << "Decrypting " << encryptedPath << " using type: " << DecryptionTypeNON << endl;
                break;
            }
        }
    }
}

// THIS FUNCTION IS TECHNICALLY THE MAIN OF THIS .cpp file
void RunEncryptionHandler(int Number) {
    ifstream FileList("Information.txt");                           // Open the "Information2.txt" file which contains file paths to encrypt or decrypt
    if (!FileList.is_open()) {                                      // Check if the file was successfully opened
        cerr << "Could not open Information.txt." << endl;          // Print the error message if unsuccessful.
        return;                                                     // Exit function.
    }

    vector<string> filePaths;                                       // Vector to store each file path read from the text file
    string line;                                                    // Variable to store each line (file path) as it's read
    while (getline(FileList, line)) {                               // Read the file line-by-line
        if (!line.empty()){
            filePaths.push_back(line);                              // Ignore empty lines and store valid paths into the vector
        }    
    }
    FileList.close();                                               // Close the file after reading all lines

    if (filePaths.empty()) {                                        // If the vector is empty, no valid file paths were found.
        cerr << "No file paths found in Information.txt." << endl;  // Print Error Message.
        return;                                                     // Exit Function.
    }

    int offset = 0;                                                 // Offset used to randomize encryption type per file (to get different results even with same seed)
    if (Number == 1) {                                              // If Number is 1, we perform encryption
        for (const string& Path : filePaths) {                      // Loop through each file path and call ProcessEncryption on it, passing current offset
            ProcessEncryption(Path, offset++);
        }
    } else if (Number == 2) {                                       // If Number is 2, we perform decryption
        for (const string& Path : filePaths) {                      // Loop through each file path and call ProcessDecryption on it
            ProcessDecryption(Path);
        }
    } else {                                                        // If the input was neither 1 nor 2, it's an invalid operation
        cerr << "Invalid option passed to RunEncryptionHandler. Use 1 for encryption or 2 for decryption." << endl;
    }
}
