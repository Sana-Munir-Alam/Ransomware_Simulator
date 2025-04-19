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

void VigenereEncryption::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {
        keylog << filename << "|" << key << "|1" << endl;   // Write filename, key, and encryption type (1 for Vigenere) to the file
    }
    keylog.close();
}

string VigenereEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";  // Name of the key log file
    vector<tuple<string, string, int>> entries;  // To store all parsed entries from the log (filename, key, type)
    string lastKey = "";              // Will hold the last matching key found
    bool found = false;              // Flag to check if the key was found

    ifstream infile(keylogFile);     // Open the key log file for reading
    if (!infile) {                   // Check if the file could not be opened
        cerr << "Error: Key log file not found!" << endl; // Show error if file is missing
        return "";                   // Return empty string if log file is not found
    }

    string line;                        // Variable to store each line read from the file
    while (getline(infile, line)) {     // Read the file line by line
        istringstream iss(line);        // Create a string stream for the current line
        string filePart, keyPart, typePart;     // To store split parts of the line
        size_t pos1 = line.find('|');           // Find position of first '|'
        size_t pos2 = line.rfind('|');          // Find position of last '|'

        // If both '|' are found and are not the same position
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);                         // Extract the filename part
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);        // Extract the key part
            typePart = line.substr(pos2 + 1);                        // Extract the type part (as string)

            int type = stoi(typePart);                               // Convert type from string to int
            entries.emplace_back(filePart, keyPart, type);           // Add the parsed entry to the list

            if (filePart == filename && type == 1) {                 // If filename matches and type is Vigenère (1)
                lastKey = keyPart;                                   // Store the key
                found = true;                                        // Mark as found
            }
        }
    }
    infile.close();                             // Close the input file

    if (!found) {                               // If no matching key was found
        cerr << "Error: No Vigenère key found for " << filename << endl; // Show error
        return "";                              // Return empty string
    }

    // Now remove the found key entry from the log and rewrite the rest
    ofstream outfile(keylogFile);               // Open the same file for writing (overwrites existing)
    if (!outfile) {                             // Check if the file couldn't be opened
        cerr << "Error: Unable to update key log file!" << endl; // Show error
        return "";                              // Return empty string
    }

    bool removed = false;                       // Flag to ensure only one matching entry is removed
    for (const auto& entry : entries) {         // Loop over all saved entries
        const string& file = get<0>(entry);     // Get filename from tuple
        const string& key = get<1>(entry);      // Get key from tuple
        int type = get<2>(entry);               // Get type from tuple

        // If this is the matching entry and we haven't removed one yet
        if (file == filename && key == lastKey && type == 1 && !removed) {
            removed = true;                     // Mark that we've removed the matching entry
            continue;                           // Skip writing this entry (i.e., remove it)
        }
        outfile << file << "|" << key << "|" << type << endl; // Write entry back to file
    }
    outfile.close();                            // Close the output file
    return lastKey;                             // Return the key that was retrieved
}

// Generate full key for encryption/decryption based on text length
string VigenereEncryption::generateKey(const string& text, const string& keyword) {
    string newKey(text.size(), ' ');                // Create a string of same size as text with spaces
    int keyIndex = 0;                               // Index to track keyword position
    for (size_t i = 0; i < text.size(); i++) {      // Loop through text
        if (!isalpha(text[i])) {                    // If character is not a letter
            newKey[i] = text[i];                    // Preserve spaces, numbers, and punctuation
        } else {
            newKey[i] = keyword[keyIndex % keyword.size()]; // Assign corresponding keyword character
            keyIndex++;                             // Move to the next keyword character
        }
    }
    return newKey; // Return the generated key
}

// Encrypt function with random key generation for Text Files
void VigenereEncryption::encryptTXT(const string &filename) {
    string key = generateRandomKey();               // Generate a random key
    storeKey(filename, key);                        // Store filename-key pair

    ifstream file(filename);                        // Open input file
    if (!file) return;                              // Return if file opening fails
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // Read entire file into a string
    file.close();                                   // Close input file

    string Fullkey = generateKey(text, key);        // Generate a full-length key for text
    string cipherText;                              // Variable to store encrypted text

    for (size_t i = 0; i < text.size(); i++) {      // Loop through each character in text
        if (!isalpha(text[i])) {                    // If character is not a letter
            cipherText.push_back(text[i]);          // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(text[i]) ? 'A' : 'a'; // Determine base ('A' or 'a') for case handling
            char x = (text[i] - base + (toupper(Fullkey[i]) - 'A')) % 26 + base; // Perform Vigenere encryption
            cipherText.push_back(x);                // Append encrypted character
        }
    }

    ofstream outFile(filename);                     // Open file for writing (overwrite mode)
    outFile << cipherText;                          // Write encrypted text to file
    outFile.close();                                // Close output file
}

// Decrypt function that retrieves the saved key for Text Files
void VigenereEncryption::decryptTXT(const string &filename) {
    string key = retrieveKey(filename);                                 // Retrieve key for the file
    if (key.empty()) {                                                  // If key is not found
        cerr << "Error: No key found for " << filename << endl;         // Print error message
        return;                                                         // Exit function
    }

    ifstream file(filename);                        // Open encrypted file
    if (!file) return;                              // Return if file opening fails
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());      // Read encrypted file
    file.close();                                   // Close input file

    string fullKey = generateKey(cipherText, key);  // Generate a full-length key for decryption
    string originalText;                            // Variable to store decrypted text

    for (size_t i = 0; i < cipherText.size(); i++) {            // Loop through each character in encrypted text
        if (!isalpha(cipherText[i])) {                          // If character is not a letter
            originalText.push_back(cipherText[i]);              // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(cipherText[i]) ? 'A' : 'a';     // Determine base ('A' or 'a') for case handling
            char x = (cipherText[i] - base - (toupper(fullKey[i]) - 'A') + 26) % 26 + base; // Perform Vigenere decryption
            originalText.push_back(x);                          // Append decrypted character
        }
    }

    ofstream outFile(filename);     // Open file for writing (overwrite mode)
    outFile << originalText;        // Write decrypted text to file
    outFile.close();                // Close output file
}

/*
 ▗▄▄▖▗▄▄▄▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▖      ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌   ▐▌   ▐▌ ▐▌▐▌   ▐▌   ▐▌ ▐▌    ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▌   ▐▛▀▀▘▐▛▀▜▌ ▝▀▚▖▐▛▀▀▘▐▛▀▚▖    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
▝▚▄▄▖▐▙▄▄▖▐▌ ▐▌▗▄▄▞▘▐▙▄▄▖▐▌ ▐▌    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

//Function to store the encryption key in ceaserkeylog.txt
void CeaserCipher::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {                                               // Check if file is successfully opened
        keylog << filename << "|" << key << "|2" << endl;       // Write filename, key, and encryption type (2 for Caesar) to the file
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string CeaserCipher::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";
    vector<tuple<string, string, int>> entries;         // Stores file-key-type tuples
    string lastKey = "";                                // Variable to hold the last found key
    bool found = false;                                 // Flag to check if the key was found

    // Read all entries from the key log file
    ifstream infile(keylogFile);                            // Open the key log file for reading
    if (!infile) {                                          // Check if the file exists and can be opened
        cerr << "Error: Key log file not found!" << endl;   // Print out the Error
        return "";                                          // Return empty string if file is not found
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string filePart, keyPart, typePart;
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);
            typePart = line.substr(pos2 + 1);

            int type = stoi(typePart);
            entries.emplace_back(filePart, keyPart, type);  // Store the entry as a tuple

            if (filePart == filename && type == 2) {        // Check for Caesar cipher entries (type 2)
                lastKey = keyPart;
                found = true;
            }
        }
    }
    infile.close();     // Close the input file

    if (!found) {       // If no key was found for the given filename
        cerr << "Error: No Caesar key found for " << filename << endl;
        return "";      // Return an empty string
    }

    // Remove only the last key entry for this file
    ofstream outfile(keylogFile);                                   // Open the file for writing (this clears its contents)
    if (!outfile) {                                                 // Check if the file was successfully opened
        cerr << "Error: Unable to update key log file!" << endl;
        return "";                                                  // Return empty string if file cannot be written to
    }

    bool removed = false;                       // Flag to track if the last key entry has been removed
    for (const auto& entry : entries) {         // Iterate through all stored entries
        const string& file = get<0>(entry);
        const string& key = get<1>(entry);
        int type = get<2>(entry);

        if (file == filename && key == lastKey && type == 2 && !removed) {
            removed = true;                     // Skip writing this specific key (remove it from log)
            continue;                           // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl; // Write entry back to file
    }
    outfile.close();                            // Close the output file after updating it
    return lastKey;                             // Return the last found key
}

// Function to generate a random key
string CeaserCipher::generateRandomKey() {
    int key = rand() % 26 + 1;          // Generate a random key between 1 and 26
    return to_string(key);              // Convert int to string
}

// Encrypt function with random key generation for Text Files
void CeaserCipher::encryptTXT(const string &filename) {
	string key = generateRandomKey();
	storeKey(filename, key);

    ifstream file(filename);
	
	string result = "";
	char c;                 // reads each character from the file to convert
	while (file.get(c)) {
		// encrypt Uppercase letters
		if (isupper(c)) {
			result += char(int(c + stoi(key) - 'A') % 26 + 'A');
		// encrypt Lowercase letters
		} else if (islower(c)) {
			result += char(int(c + stoi(key) - 'a') % 26 + 'a');
		}else{
            result +=c;
        }
	}

	file.close();

	// open file again to write encrypted content back to file
	ofstream outFile(filename); // Rewriting occuring here
	outFile << result;
	outFile.close();
}

// Decrypt function that retrieves the saved key for Text Files
void CeaserCipher::decryptTXT(const string &filename) {
	string key = retrieveKey(filename);

	if (key.empty()) {                                              // If key is not found
        cerr << "Error: No key found for " << filename << endl;     // Print error message
        return;                                                     // Exit function
    }

    ifstream file(filename);
	
    string result = "";
	char c;  // reads each character from the file to convert
	while (file.get(c)) {
		// decrypt Uppercase letters
		if (isupper(c)) {
			result += char(int(c - stoi(key) - 'A' + 26) % 26 + 'A');
		// decrypt Lowercase letters
		} else if(islower(c)){
			result += char(int(c - stoi(key) - 'a' + 26) % 26 + 'a');
		}else {
            result += c;
        }
	}
	file.close();

	// save encrypted content back to file
	ofstream outFile(filename);
	outFile << result;
	outFile.close();
}

// NON TEXT FILE ENCRYPTION
void CeaserCipher::encryptNON(const string& filename) {
    // Step 1: Generate a Caesar cipher key
    string keyStr = generateRandomKey();
    int key = stoi(keyStr);  // Convert to int for use in byte shifting

    // Step 2: Open file in binary mode and read into buffer
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 3: Encrypt the content using Caesar cipher (byte-wise shift)
    for (char& byte : buffer) {
        byte = static_cast<char>((static_cast<unsigned char>(byte) + key) % 256);
    }

    // Step 4: Create new filename with ".enc" appended
    string encryptedFilename = filename + ".enc";

    // Step 5: Write encrypted content back to new file
    ofstream outFile(encryptedFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not open file " << encryptedFilename << " for writing." << endl;
        return;
    }

    outFile.write(buffer.data(), buffer.size());
    outFile.close();

    // Step 6: Store key using original filename
    storeKey(filename, keyStr);

    // Step 7: Delete the original file after successful encryption
    if (remove(filename.c_str()) != 0) {
        cerr << "Warning: Could not delete original file: " << filename << endl;
    } else {
        cout << "Original file deleted: " << filename << endl;
    }

    cout << "Encryption complete. Encrypted file saved as: " << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void CeaserCipher::decryptNON(const string& encryptedFilename) {
    // Step 1: Ensure the file has the .enc extension using 5 as .enc is 4 character and we need atleast one more charcter to act as the filename
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be an encrypted .enc file." << endl;
        return;
    }

    // Step 2: Derive original filename by removing .enc which is 4 character long
    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);

    // Step 3: Retrieve the encryption key
    string keyStr = retrieveKey(originalFilename);  // Use the unencrypted name for key lookup
    if (keyStr.empty()) {
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;
        return;
    }

    int key = stoi(keyStr);

    // Step 4: Open the encrypted file and read its content
    ifstream inFile(encryptedFilename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;
        return;
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 5: Decrypt the content by reversing the Caesar shift
    for (char& byte : buffer) {
        byte = static_cast<char>((static_cast<unsigned char>(byte) - key + 256) % 256);
    }

    // Step 6: Save the decrypted content to the original file name
    ofstream outFile(originalFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not open file " << originalFilename << " for writing." << endl;
        return;
    }

    outFile.write(buffer.data(), buffer.size());
    outFile.close();

    // Step 7: Delete the .enc file after successful decryption
    if (remove(encryptedFilename.c_str()) != 0) {
        cerr << "Warning: Failed to delete encrypted file " << encryptedFilename << endl;
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;
    }

    cout << "Decryption complete. Decrypted file saved as: " << originalFilename << endl;
}

/*
▗▄▄▖  ▗▄▖ ▗▄▄▄▖▗▖   ▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖ ▗▄▄▖▗▄▄▄▖     ▗▄▄▖▗▄▄▄▖▗▄▄▖ ▗▖ ▗▖▗▄▄▄▖▗▄▄▖ 
▐▌ ▐▌▐▌ ▐▌  █  ▐▌   ▐▌   ▐▌   ▐▛▚▖▐▌▐▌   ▐▌       ▐▌     █  ▐▌ ▐▌▐▌ ▐▌▐▌   ▐▌ ▐▌
▐▛▀▚▖▐▛▀▜▌  █  ▐▌   ▐▛▀▀▘▐▛▀▀▘▐▌ ▝▜▌▐▌   ▐▛▀▀▘    ▐▌     █  ▐▛▀▘ ▐▛▀▜▌▐▛▀▀▘▐▛▀▚▖
▐▌ ▐▌▐▌ ▐▌▗▄█▄▖▐▙▄▄▖▐▌   ▐▙▄▄▖▐▌  ▐▌▝▚▄▄▖▐▙▄▄▖    ▝▚▄▄▖▗▄█▄▖▐▌   ▐▌ ▐▌▐▙▄▄▖▐▌ ▐▌
*/

// Generate a random key between 2 and 9
string RailFenceEncryption::generateRandomKey() {
    return to_string((rand() % 8) + 2);
}

//Function to store the encryption key in Railkeylog.txt
void RailFenceEncryption::storeKey(const string &filename, const string& key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) { // Check if file is successfully opened
        keylog << filename << "|" << key << "|3" << endl; // Write filename, key, and encryption type (3 for Rail Fence) to the file
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string RailFenceEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";
    vector<tuple<string, string, int>> entries;  // Stores filename, key, and encryption type tuples
    string lastKey = "";       // Variable to hold the last found key
    bool found = false;     // Flag to check if the key was found

    // Read all entries from the key log file
    ifstream infile(keylogFile); // Open the key log file for reading
    if (!infile) { // Check if the file exists and can be opened
        cerr << "Error: Key log file not found!" << endl;
        return ""; // Return "" if file is not found
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string filePart, keyPart, typePart;

        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');

        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            filePart = line.substr(0, pos1);
            keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);  // Extract the key
            typePart = line.substr(pos2 + 1);  // Extract the type

            int type = stoi(typePart);  // Convert type to integer

            // Store the entry as a tuple
            entries.emplace_back(filePart, keyPart, type);

            // Check if the entry matches the filename and RailFence encryption type (type 3)
            if (filePart == filename && type == 3) {
                lastKey = keyPart;
                found = true;
            }
        }
    }
    infile.close(); // Close the input file

    if (!found) {   // If no key was found for the given filename
        cerr << "Error: No key found for " << filename << endl;
        return "";
    }

    // Remove only the last key entry for this file
    ofstream outfile(keylogFile); // Open the file for writing (this clears its contents)
    if (!outfile) { // Check if the file was successfully opened
        cerr << "Error: Unable to update key log file!" << endl;
        return ""; // Return -1 if file cannot be written to
    }

    bool removed = false; // Flag to track if the last key entry has been removed
    for (const auto &entry : entries) { // Iterate through all stored entries
        const string& file = get<0>(entry);
        const string& key = get<1>(entry);
        int type = get<2>(entry);

        if (file == filename && key == lastKey && type == 3 && !removed) {
            removed = true;  // Skip writing this specific key (remove it from log)
            continue; // Continue without writing this entry
        }

        // Write the remaining entries back to the log file
        outfile << file << "|" << key << "|" << type << endl;
    }
    outfile.close(); // Close the output file after updating it

    return lastKey;  // Return the last found key
}

// Encrypt function with random key generation for Text Files
void RailFenceEncryption::encryptTXT(const string &filename) {
    string key = generateRandomKey();
    storeKey(filename, key);

    ifstream file(filename);
    if (!file) {
        return;
    }
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Rail Fence Encoding
    vector<string> rail(stoi(key));
    int row = 0, dir = 1;

    for (char c : text) {
        rail[row] += c;
        if (row == 0) dir = 1;        // Move down
        if (row == stoi(key) - 1) dir = -1; // Move up
        row += dir;
    }

    string cipherText;
    for (string line : rail) {
        cipherText += line;
    }

    ofstream outFile(filename);
    outFile << cipherText;
    outFile.close();
}

// Decrypt function that retrieves the saved key for Text Files
void RailFenceEncryption::decryptTXT(const string &filename) {
    string key = retrieveKey(filename);
    if (stoi(key) == -1) {
        cerr << "Error: No key found for " << filename << endl;
        return;
    }

    ifstream file(filename);
    if (!file) {
        return;
    }
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    int n = cipherText.size();
    vector<string> rail(stoi(key), string(n, '\n'));

    // Mark the zigzag pattern
    int row = 0, dir = 1;
    for (int i = 0; i < n; i++) {
        rail[row][i] = '*';
        if (row == 0) dir = 1;
        if (row == stoi(key) - 1) dir = -1;
        row += dir;
    }

    // Place characters in the pattern
    int index = 0;
    for (int i = 0; i < stoi(key); i++) {
        for (int j = 0; j < n; j++) {
            if (rail[i][j] == '*' && index < n) {
                rail[i][j] = cipherText[index++];
            }
        }
    }

    // Read in zigzag order
    string decryptedText;
    row = 0, dir = 1;
    for (int i = 0; i < n; i++) {
        decryptedText += rail[row][i];
        if (row == 0) dir = 1;
        if (row == stoi(key) - 1) dir = -1;
        row += dir;
    }

    ofstream outFile(filename);
    outFile << decryptedText;
    outFile.close();
}

// NON TEXT FILE ENCRYPTION
void RailFenceEncryption::encryptNON(const string& filename) {
    // Step 1: Generate and store a random Rail Fence key
    string keyStr = generateRandomKey();
    int rails = stoi(keyStr);
    storeKey(filename, keyStr);

    // Step 2: Read the file in binary mode
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    vector<char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 3: Rail Fence encryption
    vector<vector<char>> rail(rails);
    int dir = 1, row = 0;

    for (char ch : buffer) {
        rail[row].push_back(ch);                // Add char to current row
        if (row == 0) dir = 1;                  // Set direction down
        else if (row == rails - 1) dir = -1;    // Set direction up
        row += dir;                             // Move to next row
    }
    

    vector<char> encrypted;
    for (const auto& r : rail) {
        encrypted.insert(encrypted.end(), r.begin(), r.end());
    }

    // Step 4: Write to .enc file
    string encryptedFilename = filename + ".enc";
    ofstream outFile(encryptedFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not write encrypted file: " << encryptedFilename << endl;
        return;
    }

    outFile.write(encrypted.data(), encrypted.size());
    outFile.close();

    // Step 5: Delete original file
    if (remove(filename.c_str()) != 0) {
        cerr << "Warning: Could not delete original file: " << filename << endl;
    } else {
        cout << "Original file deleted: " << filename << endl;
    }

    cout << "Encryption complete. File saved as: " << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void RailFenceEncryption::decryptNON(const string& encryptedFilename) {
    // Step 1: Ensure the file has the .enc extension
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be a .enc file." << endl;
        return;
    }

    // Step 2: Derive original filename
    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);

    // Step 3: Retrieve Rail Fence key
    string keyStr = retrieveKey(originalFilename);
    if (keyStr.empty()) {
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;
        return;
    }
    int rails = stoi(keyStr);

    // Step 4: Read encrypted file into buffer
    ifstream inFile(encryptedFilename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;
        return;
    }

    vector<char> encrypted((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 5: Rail Fence decryption
    vector<vector<char>> rail(rails, vector<char>(encrypted.size(), '\n'));
    int dir = 1, row = 0;

    // Mark positions
    for (size_t i = 0; i < encrypted.size(); ++i) {
        rail[row][i] = '*';
        if (row == 0) dir = 1;
        else if (row == rails - 1) dir = -1;
        row += dir;
    }

    // Fill marked positions with actual encrypted chars
    size_t index = 0;
    for (int r = 0; r < rails; ++r) {
        for (size_t c = 0; c < encrypted.size(); ++c) {
            if (rail[r][c] == '*' && index < encrypted.size()) {
                rail[r][c] = encrypted[index++];
            }
        }
    }

    // Read the rails in zigzag to decrypt
    vector<char> decrypted;
    row = 0; dir = 1;
    for (size_t i = 0; i < encrypted.size(); ++i) {
        decrypted.push_back(rail[row][i]);
        if (row == 0) dir = 1;
        else if (row == rails - 1) dir = -1;
        row += dir;
    }

    // Step 6: Write to original file
    ofstream outFile(originalFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not write decrypted file: " << originalFilename << endl;
        return;
    }

    outFile.write(decrypted.data(), decrypted.size());
    outFile.close();

    // Step 7: Delete encrypted file
    if (remove(encryptedFilename.c_str()) != 0) {
        cerr << "Warning: Could not delete encrypted file: " << encryptedFilename << endl;
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;
    }

    cout << "Decryption complete. File restored as: " << originalFilename << endl;
}