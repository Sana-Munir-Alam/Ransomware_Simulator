#include "encryption.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

/*
▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ ▗▄▄▄▖     ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌  ▐▌  █  ▐▌   ▐▌   ▐▛▚▖▐▌▐▌   ▐▌ ▐▌▐▌       ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▌  ▐▌  █  ▐▌▝▜▌▐▛▀▀▘▐▌ ▝▜▌▐▛▀▀▘▐▛▀▚▖▐▛▀▀▘    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
 ▝▚▞▘ ▗▄█▄▖▝▚▄▞▘▐▙▄▄▖▐▌  ▐▌▐▙▄▄▖▐▌ ▐▌▐▙▄▄▖    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

// Function to generate a random 4-letter key
string VigenereEncryption::generateRandomKey() {
    string randomKey;                           // Variable to store the generated key
    for (int i = 0; i < 4; i++) {               // Loop to generate 4 random characters
        randomKey += 'A' + (rand() % 26);       // Generates a random uppercase letter (A-Z) and appends it to the key
    }
    return randomKey;                           // Returns the generated key
}

// Function to store key into a log-file
void VigenereEncryption::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {                                           // Check if file is successfully opened
        keylog << filename << "|" << key << "|1" << endl;   // Write filename, key, and encryption type (1 for Vigenere) to the file
    }
    keylog.close();
}

// Function to retrive the key from the log-file
string VigenereEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";                       // Name of the key log file
    vector<tuple<string, string, int>> entries;             // To store all parsed entries from the log in this format (filename, key, type)
    string lastKey = "";                                    // Will hold the last matching key found
    bool found = false;                                     // Flag to check if the key was found

    ifstream infile(keylogFile);                            // Open the key log file for reading
    if (!infile) {                                          // Check if the file could not be opened
        cerr << "Error: Key log file not found!" << endl;   // Show error if file is missing
        return "";                                          // Return empty string if log file is not found
    }

    string line;                                            // Variable to store each line read from the file
    while (getline(infile, line)) {                         // Read the file line by line
        string filePart, keyPart, typePart;                 // To store split parts of the line
        size_t pos1 = line.find('|');                       // Find position of first '|'
        size_t pos2 = line.rfind('|');                      // Find position of last '|'

        // If both '|' are found and are not the same position
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);                                // Extract the filename part
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);               // Extract the key part
            typePart = line.substr(pos2 + 1);                               // Extract the type part (as string)

            int type = stoi(typePart);                                      // Convert type from string to int
            entries.emplace_back(filePart, keyPart, type);                  // Add the parsed entry to the list

            if (filePart == filename && type == 1) {                        // If filename matches and type is Vigenère (1)
                lastKey = keyPart;                                          // Store the key
                found = true;                                               // Mark as found
            }
        }
    }
    infile.close();                                                         // Close the input file

    if (!found) {                                                           // If no matching key was found
        cerr << "Error: No Vigenère key found for " << filename << endl;    // Show error
        return "";                                                          // Return empty string
    }

    // Now remove the found key entry from the log and rewrite the rest
    ofstream outfile(keylogFile);                                           // Open the same file for writing (overwrites existing content)
    if (!outfile) {                                                         // Check if the file couldn't be opened
        cerr << "Error: Unable to update key log file!" << endl;            // Show error
        return "";                                                          // Return empty string
    }

    bool removed = false;                                                   // Flag to ensure only one matching entry is removed
    for (const auto& entry : entries) {                                     // Loop over all saved entries
        const string& file = get<0>(entry);                                 // Get filename from tuple
        const string& key = get<1>(entry);                                  // Get key from tuple
        int type = get<2>(entry);                                           // Get type from tuple

        // If this is the matching entry and we haven't removed one yet
        if (file == filename && key == lastKey && type == 1 && !removed) {
            removed = true;                                                 // Skip writing this specific key (remove it from log)
            continue;                                                       // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl;               // Write entry back to file
    }
    outfile.close();                                                        // Close the output file
    return lastKey;                                                         // Return the key that was last retrieved
}

// Generate full key for encryption/decryption based on text length [Wrapping the key around the file content]
string VigenereEncryption::generateKey(const string& text, const string& keyword) {
    string newKey(text.size(), ' ');                        // Create a string of same size as text with spaces
    int keyIndex = 0;                                       // Index to track keyword position
    for (size_t i = 0; i < text.size(); i++) {              // Loop through text
        if (!isalpha(text[i])) {                            // If character is not a letter
            newKey[i] = text[i];                            // Preserve spaces, numbers, and punctuation
        } else {
            newKey[i] = keyword[keyIndex % keyword.size()]; // Assign corresponding keyword character
            keyIndex++;                                     // Move to the next keyword character
        }
    }
    return newKey; // Return the generated key
}

// Encrypt function with random key generation for Text Files
void VigenereEncryption::encryptTXT(const string &filename) {
    string key = generateRandomKey();                                               // Generate a random key
    storeKey(filename, key);                                                        // Store filename-key pair

    ifstream file(filename);                                                        // Open input file
    if (!file) return;                                                              // Return if file opening fails
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());    // Read entire file into a string
    file.close();                                                                   // Close input file

    string Fullkey = generateKey(text, key);                                        // Generate a full-length key for text
    string cipherText;                                                              // Variable to store encrypted text

    for (size_t i = 0; i < text.size(); i++) {                                      // Loop through each character in text
        if (!isalpha(text[i])) {                                                    // If character is not a letter
            cipherText.push_back(text[i]);                                          // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(text[i]) ? 'A' : 'a';                               // Determine base ('A' or 'a') for case handling
            char x = (text[i] - base + (toupper(Fullkey[i]) - 'A')) % 26 + base;    // Perform Vigenere encryption
            cipherText.push_back(x);                                                // Append encrypted character
        }
    }

    ofstream outFile(filename);                                                     // Open file for writing (overwrite mode)
    outFile << cipherText;                                                          // Write encrypted text to file
    outFile.close();                                                                // Close output file
}

// Decrypt function that retrieves the saved key for Text Files
void VigenereEncryption::decryptTXT(const string &filename) {
    string key = retrieveKey(filename);                                                     // Retrieve key for the file
    if (key.empty()) {                                                                      // If key is not found
        cerr << "Error: No Vignere key found for " << filename << endl;                     // Print error message
        return;                                                                             // Exit function
    }

    ifstream file(filename);                                                                // Open encrypted file
    if (!file) return;                                                                      // Return if file opening fails
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());      // Read encrypted file content into a variable
    file.close();                                                                           // Close input file

    string fullKey = generateKey(cipherText, key);                                          // Generate a full-length key for decryption
    string originalText;                                                                    // Variable to store decrypted content

    for (size_t i = 0; i < cipherText.size(); i++) {                                        // Loop through each character in encrypted text
        if (!isalpha(cipherText[i])) {                                                      // If character is not a letter
            originalText.push_back(cipherText[i]);                                          // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(cipherText[i]) ? 'A' : 'a';                                 // Determine base ('A' or 'a') for case handling
            char x = (cipherText[i] - base - (toupper(fullKey[i]) - 'A') + 26) % 26 + base; // Perform Vigenere decryption
            originalText.push_back(x);                                                      // Append decrypted character
        }
    }

    ofstream outFile(filename);                                                             // Open file for writing (overwrite mode)
    outFile << originalText;                                                                // Write decrypted text to file
    outFile.close();                                                                        // Close output file
}

/*
 ▗▄▄▖▗▄▄▄▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▖      ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌   ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌    ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▌   ▐▛▀▀▘▐▛▀▜▌ ▝▀▚▖▐▛▀▀▘▐▛▀▚▖    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
▝▚▄▄▖▐▙▄▄▖▐▌ ▐▌▗▄▄▞▘▐▙▄▄▖▐▌ ▐▌    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

// Function to generate a random key
string CeaserCipher::generateRandomKey() {
    int key = rand() % 26 + 1;          // Generate a random key between 1 and 26
    return to_string(key);              // Convert int to string and return
}

// Function to store key into a log-file
void CeaserCipher::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {                                               // Check if file is successfully opened
        keylog << filename << "|" << key << "|2" << endl;       // Write filename, key, and encryption type (2 for Caesar) to the file
    }
    keylog.close();
}

// Function to retrive the key from the log-file
string CeaserCipher::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";                       // Name of the key log file
    vector<tuple<string, string, int>> entries;             // To store all parsed entries from the log in this format (filename, key, type)
    string lastKey = "";                                    // Will hold the last matching key found
    bool found = false;                                     // Flag to check if the key was found

    ifstream infile(keylogFile);                            // Open the key log file for reading
    if (!infile) {                                          // Check if the file could not be opened
        cerr << "Error: Key log file not found!" << endl;   // Show error if file is missing
        return "";                                          // Return empty string if log file is not found
    }

    string line;                                            // Variable to store each line read from the file
    while (getline(infile, line)) {                         // Read the file line by line
        string filePart, keyPart, typePart;                 // To store split parts of the line
        size_t pos1 = line.find('|');                       // Find position of first '|'
        size_t pos2 = line.rfind('|');                      // Find position of last '|'

        // If both '|' are found and are not the same position
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);                                // Extract the filename part
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);               // Extract the key part
            typePart = line.substr(pos2 + 1);                               // Extract the type part (as string)

            int type = stoi(typePart);                                      // Convert type from string to int
            entries.emplace_back(filePart, keyPart, type);                  // Add the parsed entry to the list

            if (filePart == filename && type == 2) {                        // If filename matches and type is Ceaser (2)
                lastKey = keyPart;                                          // Store the key
                found = true;                                               // Mark as found
            }
        }
    }
    infile.close();                                                         // Close the input file

    if (!found) {                                                           // If no matching key was found
        cerr << "Error: No Caesar key found for " << filename << endl;      // Show error
        return "";                                                          // Return an empty string
    }

    // Now remove the found key entry from the log and rewrite the rest
    ofstream outfile(keylogFile);                                           // Open the same file for writing (overwrites existing content)
    if (!outfile) {                                                         // Check if the file couldn't be opened
        cerr << "Error: Unable to update key log file!" << endl;            // Show error
        return "";                                                          // Return empty string
    }

    bool removed = false;                                                   // Flag to ensure only one matching entry is removed
    for (const auto& entry : entries) {                                     // Loop over all saved entries
        const string& file = get<0>(entry);                                 // Get filename from tuple
        const string& key = get<1>(entry);                                  // Get key from tuple
        int type = get<2>(entry);                                           // Get type from tuple

        // If this is the matching entry and we haven't removed one yet
        if (file == filename && key == lastKey && type == 2 && !removed) {
            removed = true;                                                 // Skip writing this specific key (remove it from log)
            continue;                                                       // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl;               // Write entry back to file
    }
    outfile.close();                                                        // Close the output file
    return lastKey;                                                         // Return the key that was last retrieved
}

// Encrypt function with random key generation for Text Files
void CeaserCipher::encryptTXT(const string &filename) {
	string key = generateRandomKey();                                               // Generate a random key
	storeKey(filename, key);                                                        // Store filename-key pair

    ifstream file(filename);                                                        // Open input file
    if (!file) return;                                                              // Return if file opening fails
	
	string result = "";                                                             // Variable to store in the final encrypted content
	char c;                                                                         // Reads each character from the file to convert
	while (file.get(c)) {
		if (isupper(c)) {                                                           // Encrypt Uppercase letters
			result += char(int(c + stoi(key) - 'A') % 26 + 'A');
		} else if (islower(c)) {                                                    // Encrypt Lowercase letters
			result += char(int(c + stoi(key) - 'a') % 26 + 'a');
		}else{                                                                      // Preserve spaces, numbers, and punctuation
            result +=c;
        }
	}
	file.close();                                                                   // Close input file

	ofstream outFile(filename);                                                     // Open file for writing (overwrite mode)
	outFile << result;                                                              // Write encrypted text to file
	outFile.close();                                                                // Close output file
}

// Decrypt function that retrieves the saved key for Text Files to decrypt
void CeaserCipher::decryptTXT(const string &filename) {
	string key = retrieveKey(filename);                                             // Retrieve key for the file
    if (key.empty()) {                                                              // If key is not found
        cerr << "Error: No key found for " << filename << endl;                     // Print error message
        return;                                                                     // Exit function
    }

    ifstream file(filename);                                                        // Open encrypted file
    if (!file) return;                                                              // Return if file opening fails
	
    string result = "";                                                             // Variable to store the decrypted content
	char c;                                                                         // Reads each character from the file to convert
	while (file.get(c)) {
		if (isupper(c)) {                                                           // Decrypt Uppercase letters
			result += char(int(c - stoi(key) - 'A' + 26) % 26 + 'A');
		} else if(islower(c)){                                                      // Decrypt Lowercase letters
			result += char(int(c - stoi(key) - 'a' + 26) % 26 + 'a');
		}else {                                                                     // Preserve spaces, numbers, and punctuation
            result += c;
        }
	}
	file.close();                                                                   // Close input file

	ofstream outFile(filename);                                                     // Open file for writing (overwrite mode)
	outFile << result;                                                              // Write decrypted text to file
	outFile.close();                                                                // Close output file
}

// NON TEXT FILE ENCRYPTION
void CeaserCipher::encryptNON(const string& filename) {
    string keyStr = generateRandomKey();                                                    // Step 1: Generate a random Ceaser key
    int key = stoi(keyStr);                                                                 // Convert the string key to an integer for encryption calculations

    ifstream inFile(filename, ios::binary);                                                 // Step 2: Open the input file in binary mode for reading
    if (!inFile) {                                                                          // Check if the file opened successfully
        cerr << "Error: Could not open file " << filename << " for reading." << endl;       // Print Error message
        return;                                                                             // Exit function if file failed to open
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());  // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                         // Close the input file after reading

    for (char& byte : buffer) {                                                             // Step 3: Encrypt the buffer content using Caesar Cipher logic
        // Convert byte to unsigned char for proper wrapping with modulus
        // Add key and wrap around within byte range (0–255) using % 256
        byte = static_cast<char>((static_cast<unsigned char>(byte) + key) % 256);
    }

    string encryptedFilename = filename + ".enc";                                           // Step 4: Construct a new filename for the encrypted output by appending ".enc"

    ofstream outFile(encryptedFilename, ios::binary);                                       // Step 5: Open the new encrypted file in binary mode for writing
    if (!outFile) {                                                                         // Check if the output file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for writing.\n";    // Print Error Message
        return;                                                                             // Exit function if file failed to open
    }

    outFile.write(buffer.data(), buffer.size());                                            // Write the encrypted buffer content to the new file
    outFile.close();                                                                        // Close the encrypted output file

    storeKey(filename, keyStr);                                                             // Step 6: Store key using original filename

    // Step 7: Attempt to delete the original file after successful encryption
    if (remove(filename.c_str()) != 0) {                                                    // If file deletion fails 
        cerr << "Warning: Could not delete original file: " << filename << endl;            // Print Error Message
    } else {
        cout << "Original file deleted: " << filename << endl;                              // Print File deleted Successfully Message
    }
    // Final status message confirming successful encryption
    cout << "Ceaser Encryption. Encrypted file saved as: " << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void CeaserCipher::decryptNON(const string& encryptedFilename) {
    /*
        Step 1: Validate that the input file has a valid `.enc` extension and is at least 5 characters long
        (.enc is 4 characters, and we need at least one character before it to form a valid filename)
    */
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be an encrypted .enc file." << endl;
        return;                                                                                     // Exit Function if file format is not correct
    }

    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);            // Step 2: Derive original filename by removing .enc which is 4 character long

    string keyStr = retrieveKey(originalFilename);                                                  // Step 3: Retrieve the Ceaser key associated with the original file
    if (keyStr.empty()) {                                                                           // If no key retrived
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;  // Print Error Message
        return;                                                                                     // Exit Function
    }
    int key = stoi(keyStr);                                                                         // Converting key into integer

    ifstream inFile(encryptedFilename, ios::binary);                                                // Step 4: Open and read the encrypted file into memory (binary mode)
    if (!inFile) {                                                                                  // Check if the file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;      // Print Error message
        return;                                                                                     // Exit function if file failed to open
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());          // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                                 // Close the input file after reading

    // Step 5: Decrypt the content by reversing the Caesar shift
    for (char& byte : buffer) {
        // Convert byte to unsigned char to avoid negative values during subtraction
        // Subtract key and wrap around within byte range (0–255) using modular arithmetic
        byte = static_cast<char>((static_cast<unsigned char>(byte) - key + 256) % 256);
    }

    ofstream outFile(originalFilename, ios::binary);                                                // Step 6: Write the decrypted content to the original file (overwrite or recreate it)
    if (!outFile) {                                                                                 // Check if the output file opened successfully
        cerr << "Error: Could not open file " << originalFilename << " for writing." << endl;       // Print Error Message
        return;                                                                                     // Exit function if file failed to open
    }

    outFile.write(buffer.data(), buffer.size());                                                    // Write the decrypted buffer content to the new file
    outFile.close();                                                                                // Close the decrypted output file

    // Step 7: Attempt to delete the encrypted file after successful decryption
    if (remove(encryptedFilename.c_str()) != 0) {                                                   // If file deletion fails 
        cerr << "Warning: Failed to delete encrypted file " << encryptedFilename << endl;           // Print Error Message
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;                            // Print File deleted Successfully Message
    }
    // Final status message confirming successful decryption
    cout << "Ceaser Decryption. File restored as: " << originalFilename << endl;
}

/*
▗▄▄▖  ▗▄▖ ▗▄▄▄▖▗▖   ▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖     ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌ ▐▌▐▌ ▐▌  █  ▐▌   ▐▌   ▐▌   ▐▛▚▖▐▌▐▌   ▐▌       ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▛▀▚▖▐▛▀▜▌  █  ▐▌   ▐▛▀▀▘▐▛▀▀▘▐▌ ▝▜▌▐▌   ▐▛▀▀▘    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
▐▌ ▐▌▐▌ ▐▌▗▄█▄▖▐▙▄▄▖▐▌   ▐▙▄▄▖▐▌  ▐▌▝▚▄▄▖▐▙▄▄▖    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

// Function to generate a random key
string RailFenceEncryption::generateRandomKey() {
    return to_string((rand() % 8) + 2);         // Generate a random key between 2 and 9
}

// Function to store key into a log-file
void RailFenceEncryption::storeKey(const string &filename, const string& key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {                                               // Check if file is successfully opened
        keylog << filename << "|" << key << "|3" << endl;       // Write filename, key, and encryption type (3 for Railfence) to the file
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string RailFenceEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";                       // Name of the key log file
    vector<tuple<string, string, int>> entries;             // To store all parsed entries from the log in this format (filename, key, type)
    string lastKey = "";                                    // Will hold the last matching key found
    bool found = false;                                     // Flag to check if the key was found

    ifstream infile(keylogFile);                            // Open the key log file for reading
    if (!infile) {                                          // Check if the file could not be opened
        cerr << "Error: Key log file not found!" << endl;   // Show error if file is missing
        return "";                                          // Return empty string if log file is not found
    }

    string line;                                            // Variable to store each line read from the file
    while (getline(infile, line)) {                         // Read the file line by line
        string filePart, keyPart, typePart;                 // To store split parts of the line
        size_t pos1 = line.find('|');                       // Find position of first '|'
        size_t pos2 = line.rfind('|');                      // Find position of last '|'

        // If both '|' are found and are not the same position
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);                                // Extract the filename part
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);               // Extract the key part
            typePart = line.substr(pos2 + 1);                               // Extract the type part (as string)

            int type = stoi(typePart);                                      // Convert type from string to int
            entries.emplace_back(filePart, keyPart, type);                  // Add the parsed entry to the list

            if (filePart == filename && type == 3) {                        // If filename matches and type is Railfence (3)
                lastKey = keyPart;                                          // Store the key
                found = true;                                               // Mark as found
            }
        }
    }
    infile.close();                                                         // Close the input file

    if (!found) {                                                           // If no matching key was found
        cerr << "Error: No Railfence key found for " << filename << endl;   // Show error
        return "";                                                          // Return an empty string
    }

    // Now remove the found key entry from the log and rewrite the rest
    ofstream outfile(keylogFile);                                           // Open the same file for writing (overwrites existing content)
    if (!outfile) {                                                         // Check if the file couldn't be opened
        cerr << "Error: Unable to update key log file!" << endl;            // Show error
        return "";                                                          // Return empty string
    }

    bool removed = false;                                                   // Flag to ensure only one matching entry is removed
    for (const auto& entry : entries) {                                     // Loop over all saved entries
        const string& file = get<0>(entry);                                 // Get filename from tuple
        const string& key = get<1>(entry);                                  // Get key from tuple
        int type = get<2>(entry);                                           // Get type from tuple

        // If this is the matching entry and we haven't removed one yet
        if (file == filename && key == lastKey && type == 3 && !removed) {
            removed = true;                                                 // Skip writing this specific key (remove it from log)
            continue;                                                       // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl;               // Write entry back to file
    }
    outfile.close();                                                        // Close the output file
    return lastKey;                                                         // Return the key that was last retrieved
}

// Encrypt function with random key generation for Text Files
void RailFenceEncryption::encryptTXT(const string &filename) {
    string key = generateRandomKey();                                               // Generate a random key
    storeKey(filename, key);                                                        // Store filename-key pair

    ifstream file(filename);                                                        // Open input file
    if (!file) return;                                                              // Return if file opening fails
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());    // Read entire file into a string
    file.close();                                                                   // Close input file

    // Rail Fence Encoding
    vector<string> rail(stoi(key));
    int row = 0, dir = 1;                                                           // Current Rail index = 0 AND Direction flag: 1 for down, -1 for up

    for (char c : text) {                                                           // Loop through each character in the text and place it in the appropriate rail
        rail[row] += c;                                                             // Append character to the current rail
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                      // If at top rail, start moving down
        if (row == stoi(key) - 1) dir = -1;                                         // If at bottom rail, start moving up
        row += dir;                                                                 // Move to the next rail in the current direction
    }

    string cipherText;                                                              // A variable to store the whole encrypted content
    for (string line : rail) {
        cipherText += line;                                                         // Concatenate all rails to produce the final encrypted text
    }

    ofstream outFile(filename);                                                     // Open file for writing (overwrite mode)
    outFile << cipherText;                                                          // Write encrypted text to file
    outFile.close();                                                                // Close output file
}

// Decrypt function that retrieves the saved key for Text Files
void RailFenceEncryption::decryptTXT(const string &filename) {
    string key = retrieveKey(filename);                                                     // Retrieve key for the file
    if (stoi(key) == -1) {                                                                  // If key is not found
        cerr << "Error: No key found for " << filename << endl;                             // Print error message
        return;                                                                             // Exit function
    }

    ifstream file(filename);                                                                // Open encrypted file
    if (!file) return;                                                                      // Return if file opening fails
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());      // Read encrypted file content into a variable
    file.close();                                                                           // Close input file

    int n = cipherText.size();                                                              // Total number of characters in the encrypted text
    vector<string> rail(stoi(key), string(n, '\n'));                                        // Number of rails used in encryption also fill each rail with placeholder characters (e.g., '\n') for proper sizing

    // First: Mark the zigzag pattern [where characters are going to be placed]
    int row = 0, dir = 1;                                                                   // Current Rail index = 0 AND Direction flag: 1 for down, -1 for up
    for (int i = 0; i < n; i++) {
        rail[row][i] = '*';                                                                 // Mark this position with "*" to indicate it will be filled later
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                              // If at top rail, start moving down
        if (row == stoi(key) - 1) dir = -1;                                                 // If at bottom rail, start moving up
        row += dir;                                                                         // Move to the next rail in the current direction
    }

    // Second: Replace marked positions ('*') with characters from the cipherText
    int index = 0;
    for (int i = 0; i < stoi(key); i++) {
        for (int j = 0; j < n; j++) {
            if (rail[i][j] == '*' && index < n) {                                           // If the position has "*" then...
                rail[i][j] = cipherText[index++];                                           // Fill rail pattern in row-wise order
            }
        }
    }

    // Third pass: Read the characters in zigzag fashion to reconstruct the original text
    string decryptedText;                                                                   // Variable to store in the decrypted content.
    row = 0, dir = 1;
    for (int i = 0; i < n; i++) {
        decryptedText += rail[row][i];                                                      // Append character from the appropriate rail
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                              // If at top rail, start moving down
        if (row == stoi(key) - 1) dir = -1;                                                 // If at bottom rail, start moving up
        row += dir; 
    }
    ofstream outFile(filename);                                                             // Open file for writing (overwrite mode)
    outFile << decryptedText;                                                               // Write decrypted text to file
    outFile.close();                                                                        // Close output file
}

// NON TEXT FILE ENCRYPTION
void RailFenceEncryption::encryptNON(const string& filename) {
    string keyStr = generateRandomKey();                                                    // Step 1: Generate a random Rail Fence key
    int rails = stoi(keyStr);                                                               // Convert the string key to an integer for encryption calculations
    storeKey(filename, keyStr);                                                             // Store key using original filename

    ifstream inFile(filename, ios::binary);                                                 // Step 2: Open the input file in binary mode for reading
    if (!inFile) {                                                                          // Check if the file opened successfully
        cerr << "Error: Could not open file " << filename << " for reading." << endl;       // Print Error message
        return;                                                                             // Exit function if file failed to open
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());  // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                         // Close the input file after reading

    // Step 3: Rail Fence encryption
    vector<vector<char>> rail(rails);                                                       // Initialize a 2D vector of chars, with each vector representing one rail
    int row = 0, dir = 1;                                                                   // Current Rail index = 0 AND Direction flag: 1 for down, -1 for up

    for (char ch : buffer) {                                                                // Loop through each character in the buffer and place it in the appropriate rail
        rail[row].push_back(ch);                                                            // Append character to the current rail
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                              // If at top rail, start moving down
        else if (row == rails - 1) dir = -1;                                                // If at bottom rail, start moving up
        row += dir;                                                                         // Move to the next rail in the current direction
    }
    
    vector<char> encrypted;                                                                 // A variable to store the whole encrypted content
    for (const auto& r : rail) {
        encrypted.insert(encrypted.end(), r.begin(), r.end());                              // Append each rail's contents to the final encrypted output
    }

    string encryptedFilename = filename + ".enc";                                           // Step 4: Construct a new filename for the encrypted output by appending ".enc"

    ofstream outFile(encryptedFilename, ios::binary);                                       // Step 5: Open the new encrypted file in binary mode for writing
    if (!outFile) {                                                                         // Check if the output file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for writing.\n";    // Print Error Message
        return;                                                                             // Exit function if file failed to open
    }

    outFile.write(encrypted.data(), encrypted.size());                                      // Write the encrypted buffer content to the new file
    outFile.close();                                                                        // Close the encrypted output file

    // Step 6: Attempt to delete the original file after successful encryption
    if (remove(filename.c_str()) != 0) {                                                    // If file deletion fails 
        cerr << "Warning: Could not delete original file: " << filename << endl;            // Print Error Message
    } else {
        cout << "Original file deleted: " << filename << endl;                              // Print File deleted Successfully Message
    }
    // Final status message confirming successful encryption
    cout << "Rail Encryption. Encrypted file saved as:" << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void RailFenceEncryption::decryptNON(const string& encryptedFilename) {
    /*
        Step 1: Validate that the input file has a valid `.enc` extension and is at least 5 characters long
        (.enc is 4 characters, and we need at least one character before it to form a valid filename)
    */
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be a .enc file." << endl;
        return;                                                                                     // Exit Function if file format is not correct
    }

    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);            // Step 2: Derive original filename by removing .enc which is 4 character long

    string keyStr = retrieveKey(originalFilename);                                                  // Step 3: Retrieve the Rail Fence key associated with the original file
    if (keyStr.empty()) {                                                                           // If no key retrived
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;  // Print Error Message
        return;                                                                                     // Exit Function
    }
    int rails = stoi(keyStr);                                                                       // Converting key into integer

    ifstream inFile(encryptedFilename, ios::binary);                                                // Step 4: Open and read the encrypted file into memory (binary mode)
    if (!inFile) {                                                                                  // Check if the file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;      // Print Error message
        return;                                                                                     // Exit function if file failed to open
    }

    vector<char> encrypted((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());       // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                                 // Close the input file after reading

    // Step 5: Rail Fence decryption
    vector<vector<char>> rail(rails, vector<char>(encrypted.size(), '\n'));                         // Number of rails used in encryption, fill each rail with placeholder characters (e.g., '\n') for proper sizing
    int row = 0, dir = 1;                                                                           // Current Rail index = 0 AND Direction flag: 1 for down, -1 for up

    // First: Mark the zigzag pattern [where characters are going to be placed]
    for (size_t i = 0; i < encrypted.size(); ++i) {
        rail[row][i] = '*';                                                                         // Mark this position with "*" to indicate it will be filled later
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                                      // If at top rail, start moving down
        else if (row == rails - 1) dir = -1;                                                        // If at bottom rail, start moving up
        row += dir;                                                                                 // Move to the next rail in the current direction
    }

    // Second: Replace marked positions ('*') with characters from the encrypted text
    size_t index = 0;
    for (int r = 0; r < rails; ++r) {
        for (size_t c = 0; c < encrypted.size(); ++c) {
            if (rail[r][c] == '*' && index < encrypted.size()) {                                    // If the position has "*" then...
                rail[r][c] = encrypted[index++];                                                    // Fill rail pattern in row-wise order
            }
        }
    }

    // Third pass: Read the characters in zigzag fashion to reconstruct the original text
    vector<char> decrypted;                                                                         // Variable to store in the decrypted content.
    row = 0; dir = 1;
    for (size_t i = 0; i < encrypted.size(); ++i) {
        decrypted.push_back(rail[row][i]);                                                          // Append character from the appropriate rail
        // Change direction if the top or bottom rail is reached
        if (row == 0) dir = 1;                                                                      // If at top rail, start moving down
        else if (row == rails - 1) dir = -1;                                                        // If at bottom rail, start moving up
        row += dir;                                                                                 // Move to the next rail in the current direction
    }

    ofstream outFile(originalFilename, ios::binary);                                                // Step 6: Write the decrypted content to the original file (overwrite or recreate it)
    if (!outFile) {                                                                                 // Check if the output file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for writing.\n";            // Print Error Message
        return;                                                                                     // Exit function if file failed to open
    }

    outFile.write(decrypted.data(), decrypted.size());                                              // Write the decrypted buffer content to the new file
    outFile.close();                                                                                // Close the decrypted output file

    // Step 7: Attempt to delete the encrypted file after successful decryption
    if (remove(encryptedFilename.c_str()) != 0) {                                                   // If file deletion fails 
        cerr << "Warning: Failed to delete encrypted file " << encryptedFilename << endl;           // Print Error Message
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;                            // Print File deleted Successfully Message
    }
    // Final status message confirming successful decryption
    cout << "Rail Decryption. File restored as: " << originalFilename << endl;
}

/*
 ▗▄▖ ▗▄▄▄▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖     ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌ ▐▌▐▌   ▐▌     █  ▐▛▚▖▐▌▐▌       ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▛▀▜▌▐▛▀▀▘▐▛▀▀▘  █  ▐▌ ▝▜▌▐▛▀▀▘    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
▐▌ ▐▌▐▌   ▐▌   ▗▄█▄▖▐▌  ▐▌▐▙▄▄▖    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

// Function to check if two numbers 'a' and 'm' are coprime (i.e., GCD(a, m) == 1)
bool AffineEncryption::isCoprime(int a, int m) {
    // Step 1: Use the Euclidean algorithm to find the greatest common divisor (GCD) of 'a' and 'm'
    while (m) {
        int temp = m;       // Store the value of 'm' in a temporary variable
        m = a % m;          // Update 'm' to the remainder of 'a' divided by 'm'
        a = temp;           // Update 'a' to the previous value of 'm'
    }
    // Step 2: If the GCD is 1, 'a' and 'm' are coprime; otherwise, they are not
    return a == 1;          // If GCD is 1, return true (they are coprime), else return false
}

// Function to compute the modular inverse of 'a' modulo 'm' using the Extended Euclidean Algorithm
int AffineEncryption::modInverse(int a, int m) {
    // Step 1: Initialize variables for the Euclidean algorithm
    int m0 = m, t, q;           // Store original modulus 'm' in 'm0' and initialize temporary variables 't' and 'q'
    int x0 = 0, x1 = 1;         // Initialize the coefficients for the extended Euclidean algorithm

    if (m == 1) return 0;       // The modular inverse of any number mod 1 is always 0, so return 0
    // Step 2: Use the Extended Euclidean Algorithm to find the modular inverse
    while (a > 1) {             // Continue while 'a' is greater than 1 [that is the remainder is not zero]
        q = a / m;              // Compute the quotient 'q' (integer division of 'a' by 'm')
        t = m;                  // Store the value of 'm' in 't'
        m = a % m;              // Update 'm' to the remainder of 'a' divided by 'm'
        a = t;                  // Set 'a' to the previous value of 'm'

        t = x0;                 // Update the temporary variable 't' with the value of 'x0'
        x0 = x1 - q * x0;       // Update 'x0' with the new value, based on the quotient 'q'
        x1 = t;                 // Update 'x1' with the previous value of 'x0'
    }

    // Step 3: Ensure the result is positive by adding 'm0' (the original modulus) if necessary!!
    if (x1 < 0) x1 += m0;       // If 'x1' is negative, add 'm0' to make it positive
    return x1;                  // Return the modular inverse of 'a' modulo 'm'
}

// Function to generate a valid key pair (a, b) for Affine cipher
string AffineEncryption::generateTextKey() {
    int a = rand() % 25 + 1;                    // Generate a random number between 1 and 25 (inclusive) for 'a'
    while (!isCoprime(a, 26)) {                 // Ensure that 'a' is coprime with 26 (required for decryption)
        a = rand() % 26;                        // Re-roll 'a' until it satisfies the coprime condition
    }
    int b = rand() % 26;                        // Generate a random number between 0 and 25 for 'b'
    return to_string(a) + "," + to_string(b);   // Return the key as a comma-separated string: "a,b"
}

// Function to generate a random binary key for encryption, consisting of two parts: 'a' and 'b'
string AffineEncryption::generateBinaryKey() {
    int a = rand() % 255 + 1;                   // Generate a random integer 'a' between 1 and 255
    while (!isCoprime(a, 256)) {                // Ensure 'a' is coprime with 256 (i.e., GCD(a, 256) = 1)
        a = rand() % 256;                       // Generate a new random 'a' between 0 and 255
    }
    int b = rand() % 256;                       // Generate a random integer 'b' between 0 and 255
    return to_string(a) + "," + to_string(b);   // Return the generated key as a comma-separated string: "a,b"
}

// Function to store key into a log-file
void AffineEncryption::storeKey(const string& filename, const string& key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {                                               // Check if file is successfully opened
        keylog << filename << "|" << key << "|4" << endl;       // Write filename, key, and encryption type (4 for Affine) to the file 
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string AffineEncryption::retrieveKey(const string& filename) {
    string keylogFile = "keylog.txt";                       // Name of the key log file
    vector<tuple<string, string, int>> entries;             // To store all parsed entries from the log in this format (filename, key, type)
    string lastKey = "";                                    // Will hold the last matching key found
    bool found = false;                                     // Flag to check if the key was found

    ifstream infile(keylogFile);                            // Open the key log file for reading
    if (!infile) {                                          // Check if the file could not be opened
        cerr << "Error: Key log file not found!" << endl;   // Show error if file is missing
        return "";                                          // Return empty string if log file is not found
    }

    string line;                                            // Variable to store each line read from the file
    while (getline(infile, line)) {                         // Read the file line by line
        string filePart, keyPart, typePart;                 // To store split parts of the line
        size_t pos1 = line.find('|');                       // Find position of first '|'
        size_t pos2 = line.rfind('|');                      // Find position of last '|'

        // If both '|' are found and are not the same position
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);                                // Extract the filename part
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);               // Extract the key part
            typePart = line.substr(pos2 + 1);                               // Extract the type part (as string)

            int type = stoi(typePart);                                      // Convert type from string to int
            entries.emplace_back(filePart, keyPart, type);                  // Add the parsed entry to the list

            if (filePart == filename && type == 4) {                        // If filename matches and type is Railfence (3)
                lastKey = keyPart;                                          // Store the key
                found = true;                                               // Mark as found
            }
        }
    }
    infile.close();                                                         // Close the input file

    if (!found) {                                                           // If no matching key was found
        cerr << "Error: No Affine key found for " << filename << endl;      // Show error
        return "";                                                          // Return an empty string
    }

    // Now remove the found key entry from the log and rewrite the rest
    ofstream outfile(keylogFile);                                           // Open the same file for writing (overwrites existing content)
    if (!outfile) {                                                         // Check if the file couldn't be opened
        cerr << "Error: Unable to update key log file!" << endl;            // Show error
        return "";                                                          // Return empty string
    }

    bool removed = false;                                                   // Flag to ensure only one matching entry is removed
    for (const auto& entry : entries) {                                     // Loop over all saved entries
        const string& file = get<0>(entry);                                 // Get filename from tuple
        const string& key = get<1>(entry);                                  // Get key from tuple
        int type = get<2>(entry);                                           // Get type from tuple

        // If this is the matching entry and we haven't removed one yet
        if (file == filename && key == lastKey && type == 4 && !removed) {
            removed = true;                                                 // Skip writing this specific key (remove it from log)
            continue;                                                       // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl;               // Write entry back to file
    }
    outfile.close();                                                        // Close the output file
    return lastKey;                                                         // Return the key that was last retrieved
}

// Encrypt function with random key generation for Text Files
void AffineEncryption::encryptTXT(const string& filename) {
    string key = generateTextKey();                                                 // Generate a random key
    storeKey(filename, key);                                                        // Store filename-key pair

    int a = stoi(key.substr(0, key.find(',')));                                     // Store extracted key 1
    int b = stoi(key.substr(key.find(',') + 1));                                    // Store extracted key 2

    ifstream file(filename);                                                        // Open input file
    if (!file) return;                                                              // Return if file opening fails
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());    // Read entire file into a string
    file.close();                                                                   // Close input file

    string cipher = "";                                                             // Variable to store encrypted content
    for (char c : text) {                                                           // Encrypt each character using the Affine Cipher formula: E(x) = (a * x + b) mod 26
        if (isalpha(c)) {                                                           // Only encrypt alphabetic characters
            char base = isupper(c) ? 'A' : 'a';                                     // Determine ASCII base ('A' for uppercase, 'a' for lowercase)
            char enc = ((a * (c - base) + b) % 26) + base;                          // Apply Affine encryption
            cipher += enc;                                                          // Append encrypted character
        } else {
            cipher += c;                                                            // Preserve spaces, numbers, and punctuation
        }
    }

    ofstream outFile(filename);                                                     // Open file for writing (overwrite mode)
    outFile << cipher;                                                              // Write encrypted text to file
    outFile.close();                                                                // Close output file
}

// Decrypt function that retrieves the saved key for Text Files
void AffineEncryption::decryptTXT(const string& filename) {
    string key = retrieveKey(filename);                                                             // Retrieve key for the file
    if (key.empty()) return;                                                                        // If key is not found exit function

    int a = stoi(key.substr(0, key.find(',')));                                                     // Store extracted key 1
    int b = stoi(key.substr(key.find(',') + 1));                                                    // Store extracted key 2

    // First: Compute the modular inverse of 'a' for decryption (modular inverse of 'a' mod 26)
    int a_inv = modInverse(a, 26);  // Add this line
    if (a_inv == 0) {                                                                               // If no modular inverse is found (a_inv = 0), decryption is not possible
        cerr << "Error: Modular inverse not found for a = " << a << ". Cannot decrypt." << endl;    // Print Error message
        return;                                                                                     // Exit Function
    }
    
    ifstream file(filename);                                                                        // Open encrypted file
    if (!file) return;                                                                              // Return if file opening fails
    string cipher((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());                  // Read encrypted file content into a variable
    file.close();                                                                                   // Close input file

    string plain = "";                                                                              // Variable to store the decrypted text
    for (char c : cipher) {                                                                         // Decrypt each character using the Affine decryption formula: D(x) = a_inv * (x - b) mod 26
        if (isalpha(c)) {                                                                           // Only decrypt alphabetic characters
            char base = isupper(c) ? 'A' : 'a';                                                     // Determine the base (either 'A' or 'a') based on character case
            char dec = (a_inv * ((c - base - b + 26)) % 26) + base;                                 // Apply the Affine decryption formula
            plain += dec;                                                                           // Append decrypted character
        } else {
            plain += c;                                                                             // Preserve spaces, numbers, and punctuation
        }
    }
    ofstream outFile(filename);                                                                     // Open file for writing (overwrite mode)
    outFile << plain;                                                                               // Write decrypted text to file
    outFile.close();                                                                                // Close output file
}

// NON TEXT FILE ENCRYPTION
void AffineEncryption::encryptNON(const string& filename) {
    string key = generateBinaryKey();                                                                   // Step 1: Use generateBinaryKey() to get a random key
    storeKey(filename, key);                                                                            // Store filename-key pair

    int a = stoi(key.substr(0, key.find(',')));                                                         // Store extracted key 1
    int b = stoi(key.substr(key.find(',') + 1));                                                        // Store extracted key 2

    ifstream inFile(filename, ios::binary);                                                             // Step 2: Open the input file in binary mode for reading
    if (!inFile) {                                                                                      // Check if the file opened successfully
        cerr << "Error: Could not open file " << filename << " for reading." << endl;                   // Print Error message
        return;                                                                                         // Exit function if file failed to open
    }

    vector<unsigned char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());     // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                                     // Close the input file after reading

    for (auto& byte : buffer) {                                                                         // Step 3: Encrypt each byte in the buffer using the affine cipher formula
        byte = static_cast<unsigned char>((a * byte + b) % 256);                                        // Apply the affine encryption formula
    }

    string encryptedFilename = filename + ".enc";                                                       // Step 4: Construct a new filename for the encrypted output by appending ".enc"

    ofstream outFile(encryptedFilename, ios::binary);                                                   // Step 5: Open the new encrypted file in binary mode for writing
    if (!outFile) {                                                                                     // Check if the output file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for writing.\n";                // Print Error Message
        return;                                                                                         // Exit function if file failed to open
    }

    outFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());                               // Write the encrypted buffer content to the new file
    outFile.close();                                                                                    // Close the encrypted output file

    // Step 6: Attempt to delete the original file after successful encryption
    if (remove(filename.c_str()) != 0) {                                                                // If file deletion fails 
        cerr << "Warning: Could not delete original file: " << filename << endl;                        // Print Error Message
    } else {
        cout << "Original file deleted: " << filename << endl;                                          // Print File deleted Successfully Message
    }
    // Final status message confirming successful encryption
    cout << "Affine Encryption. Encrypted File saved as: " << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void AffineEncryption::decryptNON(const string& encryptedFilename) {
    /*
        Step 1: Validate that the input file has a valid `.enc` extension and is at least 5 characters long
        (.enc is 4 characters, and we need at least one character before it to form a valid filename)
    */
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be a .enc file." << endl;
        return;                                                                                     // Exit Function if file format is not correct
    }

    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);            // Step 2: Derive original filename by removing .enc which is 4 character long

    string keyStr = retrieveKey(originalFilename);                                                  // Step 3: Retrieve the Rail Fence key associated with the original file
    if (keyStr.empty()) {                                                                           // If no key retrived
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;  // Print Error Message
        return;                                                                                     // Exit Function
    }

    int a, b;                                                                                       // Variable to store the two part key
    sscanf(keyStr.c_str(), "%d,%d", &a, &b);                                                        // Storing the extracted key

    // Step 4: Compute the modular inverse of 'a' for decryption (mod 256)
    int a_inv = modInverse(a, 256);
    if (a_inv == -1) {                                                                              // If no modular inverse is found, decryption is not possible
        cerr << "Error: No modular inverse found for a = " << a << " under mod 256." << endl;       // Print Error message
        return;                                                                                     // Exit Function
    }
    
    ifstream inFile(encryptedFilename, ios::binary);                                                // Step 5: Open and read the encrypted file into memory (binary mode)
    if (!inFile) {                                                                                  // Check if the file opened successfully
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;      // Print Error message
        return;                                                                                     // Exit function if file failed to open
    }

    vector<unsigned char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>()); // Read the entire file content into a buffer using stream iterators
    inFile.close();                                                                                 // Close the input file after reading

    // Step 6: Decrypt each byte
    for (auto& byte : buffer) {
        byte = static_cast<unsigned char>((a_inv * (byte - b + 256)) % 256); // +256 to prevent negative
    }

    ofstream outFile(originalFilename, ios::binary);                                                // Step 7: Write the decrypted content to the original file (overwrite or recreate it)
    if (!outFile) {                                                                                 // Check if the output file opened successfully
        cerr << "Error: Could not write decrypted file: " << originalFilename << endl;              // Print Error Message
        return;                                                                                     // Exit function if file failed to open
    }

    outFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());                           // Write the decrypted buffer content to the new file
    outFile.close();                                                                                // Close the decrypted output file

    // Step 8: Attempt to delete the encrypted file after successful decryption
    if (remove(encryptedFilename.c_str()) != 0) {                                                   // If file deletion fails 
        cerr << "Warning: Failed to delete encrypted file " << encryptedFilename << endl;           // Print Error Message
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;                            // Print File deleted Successfully Message
    }
    // Final status message confirming successful decryption
    cout << "Affine Decryption. File restored as: " << originalFilename << endl;
}
