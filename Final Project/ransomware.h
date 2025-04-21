#ifndef RANSOMWARE_H
#define RANSOMWARE_H
#include <string>
using namespace std;

string GenerateRandomPassword();                // Generates a random 5-character password using numbers and lowercase letters.
bool VerifyPassword(const string& password);    // Compares a given password with the previously generated one.
string GetGeneratedPassword();                  // Returns the currently stored/generated password (for testing/debugging).

#endif  // RANSOMWARE_H
