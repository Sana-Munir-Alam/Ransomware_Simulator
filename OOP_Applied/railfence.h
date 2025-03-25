#ifndef RAILFENCE_H
#define RAILFENCE_H

#include "encryption.h"
#include <string>

class RailFenceEncryption : public Encryption {
public:
    void encrypt(const std::string &filename) override;
    void decrypt(const std::string &filename) override;

private:
    void storeKey(const std::string &filename, int key);
    int retrieveKey(const std::string &filename);
};

#endif // RAILFENCE_H
