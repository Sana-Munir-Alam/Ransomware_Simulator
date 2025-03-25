#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

// Base class for encryption
class Encryption {
public:
    virtual void encrypt(const string &filename) = 0; // Pure virtual function for encryption
    virtual void decrypt(const string &filename) = 0; // Pure virtual function for decryption
    virtual ~Encryption() {} // Virtual destructor to ensure proper cleanup of derived classes

};

// Reverse-based encryption
class ReverseEncryption : public Encryption {
public:
    void encrypt(const string &filename) override;
    void decrypt(const string &filename) override;
};

// PlayFair XOR encryption
class PlayFairEncryption : public Encryption {
public:
    void encrypt(const string &filename) override;
    void decrypt(const string &filename) override;
};

// Vigenère cipher encryption child class of Encryption
class VigenereEncryption : public Encryption {
    private:
        string generateRandomKey();  // Generates a random encryption key for Vigenère cipher
    public:
        //VigenereEncryption(const string &keyword);
        string generateKey(const string& text, const string& keyword);  // Generates a repeating key based on the file content
        void encrypt(const string &filename) override;  // Overrides encrypt function for Vigenère cipher
        void decrypt(const string &filename) override;  // Overrides decrypt function for Vigenère cipher
        void storeKey(const string &filename, const string &key); // Stores the encryption key in a file named "keylog.txt"
        string retrieveKey(const string &filename); // Retrieves the stored encryption key from "keylog.txt"
};

// Ceaser cipher encryption child class of Encryption
class CeaserCipher : public Encryption{
	private:
    string generateRandomKey();
    public:
        void encrypt(const string &filename) override;
        void decrypt(const string &filename) override;
        void storeKey(const string &filename, const string &key); // Stores the encryption key in a file named "keylog.txt"
        string retrieveKey(const string &filename); // Retrieves the stored encryption key from "keylog.txt"
};


#endif // ENCRYPTION_H
