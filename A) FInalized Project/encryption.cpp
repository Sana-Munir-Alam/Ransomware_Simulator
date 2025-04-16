// This now contains all the encryption keys in the smae file, it also doesnt allow other Ciphers to decrypt it files as it's check thier ID first to get the key to decrypt [The srand is being called in the processing encryption part main wala, therefore we can use rand anywhere easily
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
    string randomKey; // Variable to store the generated key
    for (int i = 0; i < 4; i++) { // Loop to generate 4 random characters
        randomKey += 'A' + (rand() % 26); // Generates a random uppercase letter (A-Z) and appends it to the key
    }
    return randomKey; // Returns the generated key
}

void VigenereEncryption::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {
        keylog << filename << "|" << key << "|1" << endl; // 1 represents Vigenère encryption
    }
    keylog.close();
}

string VigenereEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";
    vector<tuple<string, string, int>> entries;
    string lastKey = "";
    bool found = false;

    ifstream infile(keylogFile);
    if (!infile) {
        cerr << "Error: Key log file not found!" << endl;
        return "";
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
            entries.emplace_back(filePart, keyPart, type);

            if (filePart == filename && type == 1) { // Check for Vigenère entries
                lastKey = keyPart;
                found = true;
            }
        }
    }
    infile.close();

    if (!found) {
        cerr << "Error: No Vigenère key found for " << filename << endl;
        return "";
    }

    // Rewrite the log without the last matching Vigenère entry
    ofstream outfile(keylogFile);
    if (!outfile) {
        cerr << "Error: Unable to update key log file!" << endl;
        return "";
    }

    bool removed = false;
    for (const auto& entry : entries) {
        const string& file = get<0>(entry);
        const string& key = get<1>(entry);
        int type = get<2>(entry);

        if (file == filename && key == lastKey && type == 1 && !removed) {
            removed = true;
            continue; // Skip this one (removing it)
        }
        outfile << file << "|" << key << "|" << type << endl;
    }
    outfile.close();

    return lastKey;
}

// Generate full key for encryption/decryption based on text length
string VigenereEncryption::generateKey(const string& text, const string& keyword) {
    string newKey(text.size(), ' '); // Create a string of same size as text with spaces
    int keyIndex = 0; // Index to track keyword position
    for (size_t i = 0; i < text.size(); i++) { // Loop through text
        if (!isalpha(text[i])) { // If character is not a letter
            newKey[i] = text[i]; // Preserve spaces, numbers, and punctuation
        } else {
            newKey[i] = keyword[keyIndex % keyword.size()]; // Assign corresponding keyword character
            keyIndex++; // Move to the next keyword character
        }
    }
    return newKey; // Return the generated key
}

// Encrypt function with random key generation
void VigenereEncryption::encrypt(const string &filename) {
    string key = generateRandomKey();  // Generate a random key
    storeKey(filename, key);  // Store filename-key pair

    ifstream file(filename); // Open input file
    if (!file) return; // Return if file opening fails
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // Read entire file into a string
    file.close(); // Close input file

    string Fullkey = generateKey(text, key); // Generate a full-length key for text
    string cipherText; // Variable to store encrypted text

    for (size_t i = 0; i < text.size(); i++) { // Loop through each character in text
        if (!isalpha(text[i])) { // If character is not a letter
            cipherText.push_back(text[i]); // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(text[i]) ? 'A' : 'a'; // Determine base ('A' or 'a') for case handling
            char x = (text[i] - base + (toupper(Fullkey[i]) - 'A')) % 26 + base; // Perform Vigenere encryption
            cipherText.push_back(x); // Append encrypted character
        }
    }

    ofstream outFile(filename); // Open file for writing (overwrite mode)
    outFile << cipherText; // Write encrypted text to file
    outFile.close(); // Close output file
}

// Decrypt function that retrieves the saved key
void VigenereEncryption::decrypt(const string &filename) {
    string key = retrieveKey(filename); // Retrieve key for the file
    if (key.empty()) { // If key is not found
        cerr << "Error: No key found for " << filename << endl; // Print error message
        return; // Exit function
    }

    ifstream file(filename); // Open encrypted file
    if (!file) return; // Return if file opening fails
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // Read encrypted file
    file.close(); // Close input file

    string fullKey = generateKey(cipherText, key); // Generate a full-length key for decryption
    string originalText; // Variable to store decrypted text

    for (size_t i = 0; i < cipherText.size(); i++) { // Loop through each character in encrypted text
        if (!isalpha(cipherText[i])) { // If character is not a letter
            originalText.push_back(cipherText[i]); // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(cipherText[i]) ? 'A' : 'a'; // Determine base ('A' or 'a') for case handling
            char x = (cipherText[i] - base - (toupper(fullKey[i]) - 'A') + 26) % 26 + base; // Perform Vigenere decryption
            originalText.push_back(x); // Append decrypted character
        }
    }

    ofstream outFile(filename); // Open file for writing (overwrite mode)
    outFile << originalText; // Write decrypted text to file
    outFile.close(); // Close output file
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
    if (keylog) { // Check if file is successfully opened
        keylog << filename << "|" << key << "|2" << endl; // Write filename, key, and encryption type (2 for Caesar) to the file
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string CeaserCipher::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt";
    vector<tuple<string, string, int>> entries; // Stores file-key-type tuples
    string lastKey = ""; // Variable to hold the last found key
    bool found = false; // Flag to check if the key was found

    // Read all entries from the key log file
    ifstream infile(keylogFile); // Open the key log file for reading
    if (!infile) { // Check if the file exists and can be opened
        cerr << "Error: Key log file not found!" << endl;
        return ""; // Return empty string if file is not found
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
            entries.emplace_back(filePart, keyPart, type); // Store the entry as a tuple

            if (filePart == filename && type == 2) { // Check for Caesar cipher entries (type 2)
                lastKey = keyPart;
                found = true;
            }
        }
    }
    infile.close(); // Close the input file

    if (!found) { // If no key was found for the given filename
        cerr << "Error: No Caesar key found for " << filename << endl;
        return ""; // Return an empty string
    }

    // Remove only the last key entry for this file
    ofstream outfile(keylogFile); // Open the file for writing (this clears its contents)
    if (!outfile) { // Check if the file was successfully opened
        cerr << "Error: Unable to update key log file!" << endl;
        return ""; // Return empty string if file cannot be written to
    }

    bool removed = false; // Flag to track if the last key entry has been removed
    for (const auto& entry : entries) { // Iterate through all stored entries
        const string& file = get<0>(entry);
        const string& key = get<1>(entry);
        int type = get<2>(entry);

        if (file == filename && key == lastKey && type == 2 && !removed) {
            removed = true;  // Skip writing this specific key (remove it from log)
            continue; // Continue without writing this entry
        }
        outfile << file << "|" << key << "|" << type << endl; // Write entry back to file
    }
    outfile.close(); // Close the output file after updating it

    return lastKey; // Return the last found key
}

// Function to generate a random key
string CeaserCipher::generateRandomKey() {
    int key = rand() % 26 + 1;  // Generate a random key between 1 and 26
    return to_string(key);  // Convert int to string
}

// Encrypt function with random key generation
void CeaserCipher::encrypt(const string &filename) {
	string key = generateRandomKey();
	storeKey(filename, key);

    ifstream file(filename);
	
	string result = "";
	char c;  // reads each character from the file to convert
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
	ofstream outFile(filename);
	outFile << result;
	outFile.close();
}

// Decrypt function that retrieves the saved key
void CeaserCipher::decrypt(const string &filename) {
	string key = retrieveKey(filename);

	if (key.empty()) { // If key is not found
        cerr << "Error: No key found for " << filename << endl; // Print error message
        return; // Exit function
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

// Encrypt function with random key generation
void RailFenceEncryption::encrypt(const string &filename) {
    string key = generateRandomKey();
    storeKey(filename, key);

    ifstream file(filename);
    if (!file) return;
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

// Decrypt function that retrieves the saved key
void RailFenceEncryption::decrypt(const string &filename) {
    string key = retrieveKey(filename);
    if (stoi(key) == -1) {
        cerr << "Error: No key found for " << filename << endl;
        return;
    }

    ifstream file(filename);
    if (!file) return;
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