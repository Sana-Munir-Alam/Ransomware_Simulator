#include "ransomware.h"
#include <iostream>
using namespace std;

bool VerifyPassword(const string& password) {
    return password == "1A234";  // Compare with the correct password
}