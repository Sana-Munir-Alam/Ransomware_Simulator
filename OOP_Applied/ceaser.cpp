#include "encryption.h"
#include<iostream>
#include<cstdlib>  // library for radnom num generation
#include<ctime>
#include <fstream>
#include <sstream>

using namespace std;

// Function to store the encryption key in keylog.txt
void CeaserCipher::storeKey(const string &filename, const string &key) {
    ofstream keylog("ceaserkeylog.txt", ios::app); // Open ceaserkeylog.txt in append mode
    if (keylog) { // Check if file is successfully opened
        keylog << filename << " " << key << endl; // Write filename and key pair to the file
    }
}
// Function to retrieve the key from keylog.txt
string CeaserCipher::retrieveKey(const string &filename) {
    ifstream keylog("ceaserkeylog.txt"); // Open ceaserkeylog.txt for reading
    string file, key; // Variables to store file entries
    while (keylog >> file >> key) { // Read filename-key pairs from the file
        if (file == filename) { // If filename matches the requested file
            return key; // Return the corresponding key
        }
    }
    return "";  // Return empty string if key not found
}

string CeaserCipher::generateRandomKey() {
    srand(time(0));  // Seed for randomness (should be called once in main usually)
    int key = rand() % 26 + 1;  // Generate a random key between 1 and 26
    return to_string(key);  // Convert int to string
}

void CeaserCipher::encrypt(const string &filename) {
	string key = generateRandomKey();
	storeKey(filename, key);
	ifstream file(filename);  // open file to read
	
	string result = "";
	char c;  // reads each character from the file to convert
	while (file.get(c)) {
		// encrypt Uppercase letters
		if (isupper(c)) {
			result += char(int(c + stoi(key) - 'A') % 26 + 'A');
		// encrypt Lowercase letters
		} else if (islower(c)) {
			result += char(int(c + stoi(key) - 'a') % 26 + 'a');
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
	ifstream file(filename);
	
	string result = "";
	char c;  // reads each character from the file to convert
	while (file.get(c)) {
		// decrypt Uppercase letters
		if (isupper(c)) {
			result += char(int(c - stoi(key) - 'A' + 26) % 26 + 'A');
		// decrypt Lowercase letters
		} else {
			result += char(int(c - stoi(key) - 'a' + 26) % 26 + 'a');
		}
	}
	file.close();

	// save encrypted content back to file
	ofstream outFile(filename);
	
	outFile << result;
	outFile.close();
}
