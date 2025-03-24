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

// Vigenere Cipher encryption methods
string VigenereEncryption::generateKey(const string& text, const string& keyword) {
    string newKey(text.size(), ' ');
    int keyIndex = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (!isalpha(text[i])) {
            newKey[i] = text[i]; // Preserve spaces, numbers, and punctuation in the key (they are not cyphered)
        } else {
            newKey[i] = keyword[keyIndex % keyword.size()];
            keyIndex++;
        }
    }
    return newKey;
}

void VigenereEncryption::encrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    string keyword = "KEYWORD"; // Example key, can be modified
    string key = generateKey(text, keyword);
    string cipherText;
    
    for (size_t i = 0; i < text.size(); i++) {
        if (!isalpha(text[i])) {
            cipherText.push_back(text[i]); // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(text[i]) ? 'A' : 'a';
            char x = (text[i] - base + (toupper(key[i]) - 'A')) % 26 + base;
            cipherText.push_back(x);
        }
    }
    
    ofstream outFile(filename);
    outFile << cipherText;
    outFile.close();
}

void VigenereEncryption::decrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string cipherText((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    string keyword = "KEYWORD"; // Example key, should match encryption key
    string key = generateKey(cipherText, keyword);
    string originalText;
    
    for (size_t i = 0; i < cipherText.size(); i++) {
        if (!isalpha(cipherText[i])) {
            originalText.push_back(cipherText[i]); // Preserve spaces, numbers, and punctuation
        } else {
            char base = isupper(cipherText[i]) ? 'A' : 'a';
            char x = (cipherText[i] - base - (toupper(key[i]) - 'A') + 26) % 26 + base;
            originalText.push_back(x);
        }
    }
    
    ofstream outFile(filename);
    outFile << originalText;
    outFile.close();
}
