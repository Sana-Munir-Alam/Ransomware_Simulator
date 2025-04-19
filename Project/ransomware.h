#ifndef RANSOMWARE_H
#define RANSOMWARE_H
#include <string>
using namespace std;

string GenerateRandomPassword();
bool VerifyPassword(const string& password);
string GetGeneratedPassword();

#endif