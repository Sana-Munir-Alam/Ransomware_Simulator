// #ifndef ENCRYPTION_H
// #define ENCRYPTION_H
// #include <string>
// using namespace std;

// void Encrypt(const string &filename);
// void Decrypt(const string &filename);

// #endif

#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <string>
using namespace std;

void Encrypt(const string &filename);
void Decrypt(const string &filename);
void CaesarEncrypt(const string &filename);
void CaesarDecrypt(const string &filename);
void PlayFairEncrypt(const string &filename);
void PlayFairDecrypt(const string &filename);

#endif
