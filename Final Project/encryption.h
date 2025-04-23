#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <iostream>

using namespace std;

// Base class for encryption is also an Abstract class due to these Pure Virtual Function
class Encryption {
public:
    virtual void encryptTXT(const string &filename) = 0; // Pure virtual function for encryption Text File
    virtual void decryptTXT(const string &filename) = 0; // Pure virtual function for decryption Text File

    virtual void encryptNON(const string &filename){    // virtual function for encryption Non-Text File
        cout << "encryptNON not supported by this cipher.\n";
    }
    virtual void decryptNON(const string &filename){    // virtual function for decryption Non-Text File
        cout << "encryptNON not supported by this cipher.\n";
    }
    virtual ~Encryption() {} // Virtual destructor to ensure proper cleanup of derived classes

};

// Vigenère cipher encryption child class of Encryption
class VigenereEncryption : public Encryption {
    private:
        string generateRandomKey();     // Generates a random encryption key for Vigenère cipher
    public:
        string generateKey(const string& text, const string& keyword);      // Generates a repeating key wrapping around the file content
        void encryptTXT(const string &filename) override;                   // Overrides encrypt function for Vigenère cipher Text File
        void decryptTXT(const string &filename) override;                   // Overrides decrypt function for Vigenère cipher Text File
        void storeKey(const string &filename, const string &key);           // Stores the encryption key in a file named "keylog.txt"
        string retrieveKey(const string &filename);                         // Retrieves the stored encryption key from "keylog.txt"
};

// Ceaser cipher encryption child class of Encryption
class CeaserCipher : public Encryption{
	private:
        string generateRandomKey();     // Generates a random encryption key for Ceaser cipher
    public:
        void encryptTXT(const string &filename) override;               // Overrides encrypt function for Ceaser cipher Text File
        void decryptTXT(const string &filename) override;               // Overrides decrypt function for Ceaser cipher Text File
        void encryptNON(const string &filename) override;               // Overrides encrypt function for Ceaser cipher Non-Text File
        void decryptNON(const string &filename) override;               // Overrides decrypt function for Ceaser cipher Non-Text File
        void storeKey(const string &filename, const string &key);       // Stores the encryption key in a file named "keylog.txt"
        string retrieveKey(const string &filename);                     // Retrieves the stored encryption key from "keylog.txt"
};

// Railfence Cipher encryption child class of encryption
class RailFenceEncryption : public Encryption {
    private:
        string generateRandomKey();      // Generates a random encryption key for Railfence cipher
    public:
        void encryptTXT(const string &filename) override;               // Overrides encrypt function for RailFence cipher Text File
        void decryptTXT(const string &filename) override;               // Overrides decrypt function for RailFence cipher Text File
        void encryptNON(const string &filename) override;               // Overrides encrypt function for RailFence cipher Non-Text File
        void decryptNON(const string &filename) override;               // Overrides decrypt function for RailFence cipher Non-Text File
        void storeKey(const string &filename, const string& key);       // Stores the encryption key in a file named "keylog.txt"
        string retrieveKey(const string &filename);                     // Retrieves the stored encryption key from "keylog.txt"
};

// Affine Cipher encryption child class of encryption
class AffineEncryption : public Encryption {
    private:
        string generateTextKey();       // Generates a random encryption key for Affine cipher [Text-File]
        string generateBinaryKey();     // Generates a random encryption key for Affine cipher [Non-Text File]
        bool isCoprime(int a, int m);   // Function to check if two numbers 'a' and 'm' are coprime (i.e., GCD(a, m) == 1)
        int modInverse(int a, int m);  
    public:
        void encryptTXT(const string &filename) override;               // Overrides encrypt function for Affine cipher Text File               
        void decryptTXT(const string &filename) override;               // Overrides decrypt function for Affine cipher Text File               
        void encryptNON(const string &filename) override;               // Overrides encrypt function for Affine cipher Non-Text File               
        void decryptNON(const string &filename) override;               // Overrides decrypt function for Affine cipher Non-Text File
        void storeKey(const string &filename, const string &key);       // Stores the encryption key in a file named "keylog.txt"       
        string retrieveKey(const string &filename);                     // Retrieves the stored encryption key from "keylog.txt"                    
    };
#endif // ENCRYPTION_H
