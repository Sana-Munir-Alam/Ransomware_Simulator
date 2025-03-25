#include "encryption.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

// Reverse-based encryption methods
void ReverseEncryption::encrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    reverse(content.begin(), content.end());
    
    ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

void ReverseEncryption::decrypt(const string &filename) {
    encrypt(filename);  // Reversing again decrypts
}

// PlayFair XOR encryption methods
void PlayFairEncryption::encrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    char key = 'K';  // XOR key
    for (char &ch : content) {
        ch ^= key;
    }

    ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

void PlayFairEncryption::decrypt(const string &filename) {
    encrypt(filename);  // XOR is reversible
}

// THIS IS GOING TO BE THE PART FROM WHERE OUR REAL ENCRYPTION/DECRYPTION ALGORITHMS WORKS
/*
__     _____ ____ _____ _   _ _____ ____  _____      _____ _   _  ____ ______   ______ _____ ___ ___  _   _ 
\ \   / |_ _/ ___| ____| \ | | ____|  _ \| ____|    | ____| \ | |/ ___|  _ \ \ / |  _ |_   _|_ _/ _ \| \ | |
 \ \ / / | | |  _|  _| |  \| |  _| | |_) |  _|      |  _| |  \| | |   | |_) \ V /| |_) || |  | | | | |  \| |
  \ V /  | | |_| | |___| |\  | |___|  _ <| |___     | |___| |\  | |___|  _ < | | |  __/ | |  | | |_| | |\  |
   \_/  |___\____|_____|_| \_|_____|_| \_|_____|    |_____|_| \_|\____|_| \_\|_| |_|    |_| |___\___/|_| \_|
*/

// Function to generate a random 4-letter key
string VigenereEncryption::generateRandomKey() {
    string randomKey; // Variable to store the generated key
    for (int i = 0; i < 4; i++) { // Loop to generate 4 random characters
        randomKey += 'A' + (rand() % 26); // Generates a random uppercase letter (A-Z) and appends it to the key
    }
    return randomKey; // Returns the generated key
}

// Function to store the encryption key in keylog.txt
void VigenereEncryption::storeKey(const string &filename, const string &key) {
    ofstream keylog("keylog.txt", ios::app); // Open keylog.txt in append mode
    if (keylog) { // Check if file is successfully opened
        keylog << filename << " " << key << endl; // Write filename and key pair to the file
    }
    keylog.close();
}

// Function to retrieve and remove the last encryption key for a given file
string VigenereEncryption::retrieveKey(const string &filename) {
    string keylogFile = "keylog.txt"; // File where keys are logged
    vector<pair<string, string>> entries; // Stores file-key pairs from the log
    string lastKey = ""; // Variable to hold the last found key
    bool found = false; // Flag to check if the key was found

    // Read all entries from the key log file
    ifstream infile(keylogFile); // Open the key log file for reading
    if (!infile) { // Check if the file exists and can be opened
        cerr << "Error: Key log file not found!" << endl;
        return ""; // Return empty string if file is not found
    }

    string file, key; 
    while (infile >> file >> key) { // Read each file-key pair from the file
        entries.push_back({file, key}); // Store the pair in the vector
        if (file == filename) { // Check if the filename matches the given one
            lastKey = key;  // Store the latest matching key
            found = true; // Set flag to indicate the key was found
        }
    }
    infile.close(); // Close the input file

    if (!found) { // If no key was found for the given filename
        cerr << "Error: No key found for " << filename << endl;
        return ""; // Return an empty string
    }

    // Remove only the last key entry for this file
    ofstream outfile(keylogFile); // Open the file for writing (this clears its contents)
    if (!outfile) { // Check if the file was successfully opened
        cerr << "Error: Unable to update key log file!" << endl;
        return ""; // Return empty string if file cannot be written to
    }

    bool removed = false; // Flag to track if the last key entry has been removed
    for (const auto &entry : entries) { // Iterate through all stored entries
        if (entry.first == filename && entry.second == lastKey && !removed) {
            removed = true;  // Skip writing this specific key (remove it from log)
        } else {
            outfile << entry.first << " " << entry.second << endl; // Write entry back to file
        }
    }
    outfile.close(); // Close the output file after updating it

    return lastKey; // Return the last found key
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
    ____ _____    _    ____  _____ ____      ______   ______  _   _ _____ ____  
  / ___| ____|  / \  / ___|| ____|  _ \     / ___\ \ / |  _ \| | | | ____|  _ \ 
 | |   |  _|   / _ \ \___ \|  _| | |_) |   | |    \ V /| |_) | |_| |  _| | |_) |
 | |___| |___ / ___ \ ___) | |___|  _ <    | |___  | | |  __/|  _  | |___|  _ < 
  \____|_____/_/   \_|____/|_____|_| \_\    \____| |_| |_|   |_| |_|_____|_| \_\
*/

//Function to store the encryption key in ceaserkeylog.txt
void CeaserCipher::storeKey(const string &filename, const string &key) {
    ofstream keylog("ceaserkeylog.txt", ios::app); // Open ceaserkeylog.txt in append mode
    if (keylog) { // Check if file is successfully opened
        keylog << filename << " " << key << endl; // Write filename and key pair to the file
    }
    keylog.close();
}

string CeaserCipher::retrieveKey(const string &filename) {
    string keylogFile = "ceaserkeylog.txt";
    vector<pair<string, string>> entries; // Stores file-key pairs from the log
    string lastKey = ""; // Variable to hold the last found key
    bool found = false; // Flag to check if the key was found

    // Read all entries from the key log file
    ifstream infile(keylogFile); // Open the key log file for reading
    if (!infile) { // Check if the file exists and can be opened
        cerr << "Error: Key log file not found!" << endl;
        return ""; // Return empty string if file is not found
    }

    string file, key; 
    while (infile >> file >> key) { // Read each file-key pair from the file
        entries.push_back({file, key}); // Store the pair in the vector
        if (file == filename) { // Check if the filename matches the given one
            lastKey = key;  // Store the latest matching key
            found = true; // Set flag to indicate the key was found
        }
    }
    infile.close(); // Close the input file

    if (!found) { // If no key was found for the given filename
        cerr << "Error: No key found for " << filename << endl;
        return ""; // Return an empty string
    }

    // Remove only the last key entry for this file
    ofstream outfile(keylogFile); // Open the file for writing (this clears its contents)
    if (!outfile) { // Check if the file was successfully opened
        cerr << "Error: Unable to update key log file!" << endl;
        return ""; // Return empty string if file cannot be written to
    }

    bool removed = false; // Flag to track if the last key entry has been removed
    for (const auto &entry : entries) { // Iterate through all stored entries
        if (entry.first == filename && entry.second == lastKey && !removed) {
            removed = true;  // Skip writing this specific key (remove it from log)
        } else {
            outfile << entry.first << " " << entry.second << endl; // Write entry back to file
        }
    }
    outfile.close(); // Close the output file after updating it

    return lastKey; // Return the last found key
}

string CeaserCipher::generateRandomKey() {
    srand(time(0));  // Seed for randomness (should be called once in main usually)
    int key = rand() % 26 + 1;  // Generate a random key between 1 and 26
    return to_string(key);  // Convert int to string
}

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
