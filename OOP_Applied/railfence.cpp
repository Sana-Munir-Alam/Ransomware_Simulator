#include "railfence.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Store the key in keylog.txt
void RailFenceEncryption::storeKey(const string &filename, int key) {
    ifstream keylogIn("keylog.txt");
    vector<pair<string, int>> keys;
    string file;
    int oldKey;
    
    // Read existing keys
    while (keylogIn >> file >> oldKey) {
        if (file == filename)
            oldKey = key;  // Update existing key
        keys.push_back({file, oldKey});
    }
    keylogIn.close();

    // Write back all keys with the updated one
    ofstream keylogOut("keylog.txt");
    for (auto &pair : keys) {
        keylogOut << pair.first << " " << pair.second << endl;
    }
    keylogOut.close();
}


// Retrieve the key from keylog.txt
int RailFenceEncryption::retrieveKey(const string &filename) {
    ifstream keylog("keylog.txt");
    string file;
    int key;
    while (keylog >> file >> key) {
        if (file == filename) {
            return key;
        }
    }
    cerr << "Warning: No key found for " << filename << ". Using default key = 3.\n";
    return 3;  // Use default if key is missing
}


// Encrypt function
void RailFenceEncryption::encrypt(const string &filename) {
    int key = 3; // Default rail height (change as needed)
    storeKey(filename, key);

    ifstream file(filename);
    if (!file) return;
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    // Rail Fence Encoding
    vector<string> rail(key);
    int row = 0, dir = 1;

    for (char c : text) {
        rail[row] += c;
        if (row == 0) dir = 1;        // Move down
        if (row == key - 1) dir = -1; // Move up
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

// Decrypt function
void RailFenceEncryption::decrypt(const string &filename) {
    int key = retrieveKey(filename);
    if (key == -1) {
        cerr << "Error: No key found for " << filename << endl;
        return;
    }

    ifstream file(filename);
    if (!file) return;
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    int n = cipherText.size();
    vector<string> rail(key, string(n, '\n'));

    // Mark the zigzag pattern
    int row = 0, dir = 1;
    for (int i = 0; i < n; i++) {
        rail[row][i] = '*';
        if (row == 0) dir = 1;
        if (row == key - 1) dir = -1;
        row += dir;
    }

    // Place characters in the pattern
    int index = 0;
    for (int i = 0; i < key; i++) {
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
        if (row == key - 1) dir = -1;
        row += dir;
    }

    ofstream outFile(filename);
    outFile << decryptedText;
    outFile.close();
}
