#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

using namespace std;

// Base class for encryption
class Encryption {
public:
    virtual void encrypt(const string &filename) = 0;
    virtual void decrypt(const string &filename) = 0;
    virtual ~Encryption() {}
};

// Reverse-based encryption
class ReverseEncryption : public Encryption {
public:
    void encrypt(const string &filename) override;
    void decrypt(const string &filename) override;
};

// Caesar cipher encryption
class CaesarEncryption : public Encryption {
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

#endif // ENCRYPTION_H
