// #include "encryption.h"
// #include <fstream>
// #include <iostream>
// #include <algorithm>
// using namespace std;

// void Encrypt(const string &filename) {
//     ifstream file(filename);
//     if (!file) return;
//     string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
//     file.close();

//     reverse(content.begin(), content.end());

//     ofstream outFile(filename);
//     outFile << content;
//     outFile.close();
// }

// void Decrypt(const std::string &filename) {
//     Encrypt(filename);  // Since encryption is reversing, doing it again decrypts.
// }

#include "encryption.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

using namespace std;

// Reverse-based encryption (original function)
void Encrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    reverse(content.begin(), content.end());

    ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

// Reverse-based decryption (same as encryption)
void Decrypt(const string &filename) {
    Encrypt(filename);  // Since encryption is reversing, doing it again decrypts.
}

// Caesar Encryption (Convert characters to ASCII values)
void CaesarEncrypt(const string &filename) {
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

// Caesar Decryption (Convert ASCII values back to characters)
void CaesarDecrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content;
    getline(file, content);
    file.close();

    istringstream iss(content);
    string token;
    string decryptedText;

    while (iss >> token) {
        decryptedText += static_cast<char>(stoi(token));  // Convert ASCII values back to characters
    }

    ofstream outFile(filename);
    outFile << decryptedText;
    outFile.close();
}

// PlayFair XOR Encryption
void PlayFairEncrypt(const string &filename) {
    ifstream file(filename);
    if (!file) return;
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    char key = 'K';  // XOR key
    for (char &ch : content) {
        ch ^= key;  // XOR each character with the key
    }

    ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

// PlayFair XOR Decryption (Same as encryption because XOR is reversible)
void PlayFairDecrypt(const string &filename) {
    PlayFairEncrypt(filename);  // XOR encryption and decryption are identical
}
