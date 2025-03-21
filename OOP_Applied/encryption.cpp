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

// Caesar cipher encryption methods
void CaesarEncryption::encrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    ostringstream oss;
    for (char ch : content) {
        oss << static_cast<int>(ch) << " ";  // Convert each character to ASCII
    }

    ofstream outFile(filename);
    outFile << oss.str();
    outFile.close();
}

void CaesarEncryption::decrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content;
    getline(file, content);
    file.close();

    istringstream iss(content);
    string token;
    string decryptedText;

    while (iss >> token) {
        decryptedText += static_cast<char>(stoi(token));  // Convert ASCII back to characters
    }

    ofstream outFile(filename);
    outFile << decryptedText;
    outFile.close();
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
