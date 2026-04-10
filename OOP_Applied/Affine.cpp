//For header file (there are some extra declarations here...I will remove them)
class AffineEncryption : public Encryption {
    private:
    string generateTextKey();
    string generateBinaryKey();
        bool isCoprime(int a, int m);
        int modInverse(int a, int m);  
    public:
        void encryptTXT(const string &filename) override;               
        void decryptTXT(const string &filename) override;               
        void encryptNON(const string &filename) override;               
        void decryptNON(const string &filename) override;               
        void storeKey(const string &filename, const string &key);       
        string retrieveKey(const string &filename);                     
    };
    


AffineEncryption::AffineEncryption() {
    srand(time(0));  
}

bool AffineEncryption::isCoprime(int a, int m) {
    while (m) {
        int temp = m;
        m = a % m;
        a = temp;
    }
    return a == 1;
}

// Helper: Modular inverse using Extended Euclidean Algorithm
int AffineEncryption::modInverse(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1) return 0;

    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}


string AffineEncryption::generateTextKey() {
    int a = rand() % 25 + 1;
    while (!isCoprime(a, 26)) {
        a = rand() % 26;
    }
    int b = rand() % 26;
    return to_string(a) + "," + to_string(b);
}

string AffineEncryption::generateBinaryKey() {
    int a = rand() % 255 + 1;
    while (!isCoprime(a, 256)) {
        a = rand() % 256;
    }
    int b = rand() % 256;
    return to_string(a) + "," + to_string(b);
}


// Store the key in keylog.txt
void AffineEncryption::storeKey(const string& filename, const string& key) {
    ofstream keylog("keylog.txt", ios::app);
    if (keylog) {
        keylog << filename << "|" << key << "|4" << endl; 
    }
    keylog.close();
}

// Retrieve and remove the last Affine key used
string AffineEncryption::retrieveKey(const string& filename) {
    string keylogFile = "keylog.txt";
    vector<tuple<string, string, int>> entries;
    string lastKey = "";
    bool found = false;

    ifstream infile(keylogFile);
    if (!infile) {
        cerr << "Error: Key log file not found!" << endl;
        return "";
    }

    string line;
    while (getline(infile, line)) {
        size_t pos1 = line.find('|');
        size_t pos2 = line.rfind('|');
        if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
            string filePart = line.substr(0, pos1);
            string keyPart = line.substr(pos1 + 1, pos2 - pos1 - 1);
            int type = stoi(line.substr(pos2 + 1));
            entries.emplace_back(filePart, keyPart, type);

            if (filePart == filename && type == 4) {
                lastKey = keyPart;
                found = true;
            }
        }
    }
    infile.close();

    if (!found) {
        cerr << "Error: No key found for " << filename << endl;
        return "";
    }

    ofstream outfile(keylogFile);
    if (!outfile) {
        cerr << "Error: Unable to update key log file!" << endl;
        return "";
    }

    bool removed = false;
    for (const auto& entry : entries) {
        const string& file = get<0>(entry);
        const string& key = get<1>(entry);
        int type = get<2>(entry);

        if (file == filename && key == lastKey && type == 4 && !removed) {
            removed = true;
            continue;
        }
        outfile << file << "|" << key << "|" << type << endl;
    }
    outfile.close();

    return lastKey;
}



void AffineEncryption::encryptTXT(const string& filename) {
    string key = generateTextKey(); // or generateBinaryKey()
    storeKey(filename, key);

    int a = stoi(key.substr(0, key.find(',')));
    int b = stoi(key.substr(key.find(',') + 1));

    ifstream file(filename);
    if (!file) return;
    string text((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string cipher = "";
    for (char c : text) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char enc = ((a * (c - base) + b) % 26) + base;
            cipher += enc;
        } else {
            cipher += c;
        }
    }

    ofstream outFile(filename + ".enc");
    outFile << cipher;
    outFile.close();
}


void AffineEncryption::decryptTXT(const string& filename) {
    string key = retrieveKey(filename);
    if (key.empty()) return;

    int a = stoi(key.substr(0, key.find(',')));
    int b = stoi(key.substr(key.find(',') + 1));
    int a_inv = modInverse(a, 26);  // Add this line
    if (a_inv == 0) {
        cerr << "Error: Modular inverse not found for a = " << a << ". Cannot decrypt." << endl;
        return;
    }
    


    ifstream file(filename);
    if (!file) return;
    string cipher((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    string plain = "";
    for (char c : cipher) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            char dec = (a_inv * ((c - base - b + 26)) % 26) + base;
            plain += dec;
        } else {
            plain += c;
        }
    }
    string decryptedFilename = filename + ".dec";
    ofstream outFile(decryptedFilename);
    cout << "Decrypted file saved as: " << decryptedFilename << endl;
    
    outFile << plain;
    outFile.close();
}


// NON TEXT FILE ENCRYPTION
void AffineEncryption::encryptNON(const string& filename) {
    // Step 1: Generate random coprime 'a' and random 'b'
    int a = rand() % 256;
while (!isCoprime(a, 256)) {
    a = rand() % 256;
}
int b = rand() % 256;


    // Store the key as a string
    string keyStr = to_string(a) + "," + to_string(b);

    storeKey(filename, keyStr);

    // Step 2: Read the file in binary mode
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }
    vector<unsigned char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 3: Encrypt each byte
    for (auto& byte : buffer) {
        byte = static_cast<unsigned char>((a * byte + b) % 256);
    }

    // Step 4: Write encrypted data to .enc file
    string encryptedFilename = filename + ".enc";
    ofstream outFile(encryptedFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not write encrypted file: " << encryptedFilename << endl;
        return;
    }
    outFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    outFile.close();

    // Step 5: Delete original file
    if (remove(filename.c_str()) != 0) {
        cerr << "Warning: Could not delete original file: " << filename << endl;
    } else {
        cout << "Original file deleted: " << filename << endl;
    }

    cout << "Encryption complete. File saved as: " << encryptedFilename << endl;
}

// NON TEXT FILE DECRYPTION
void AffineEncryption::decryptNON(const string& encryptedFilename) {
    // Step 1: Check if the file ends in .enc
    if (encryptedFilename.size() < 5 || encryptedFilename.substr(encryptedFilename.size() - 4) != ".enc") {
        cerr << "Error: File " << encryptedFilename << " does not appear to be a .enc file." << endl;
        return;
    }

    // Step 2: Derive original filename
    string originalFilename = encryptedFilename.substr(0, encryptedFilename.size() - 4);

    // Step 3: Retrieve key (a and b)
    string keyStr = retrieveKey(originalFilename);
    if (keyStr.empty()) {
        cerr << "Error: Could not retrieve encryption key for file: " << originalFilename << endl;
        return;
    }

    int a, b;
    sscanf(keyStr.c_str(), "%d,%d", &a, &b); 


    // Step 4: Compute modular inverse of a (mod 256)
    int a_inv = modInverse(a, 256);
    if (a_inv == -1) {
        cerr << "Error: No modular inverse found for a = " << a << " under mod 256." << endl;
        return;
    }
    

    // Step 5: Read encrypted binary data
    ifstream inFile(encryptedFilename, ios::binary);
    if (!inFile) {
        cerr << "Error: Could not open file " << encryptedFilename << " for reading." << endl;
        return;
    }
    vector<unsigned char> buffer((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Step 6: Decrypt each byte
    for (auto& byte : buffer) {
        byte = static_cast<unsigned char>((a_inv * (byte - b + 256)) % 256); // +256 to prevent negative
    }

    // Step 7: Write to original file
    ofstream outFile(originalFilename, ios::binary);
    if (!outFile) {
        cerr << "Error: Could not write decrypted file: " << originalFilename << endl;
        return;
    }
    outFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    outFile.close();

    // Step 8: Delete encrypted file
    if (remove(encryptedFilename.c_str()) != 0) {
        cerr << "Warning: Could not delete encrypted file: " << encryptedFilename << endl;
    } else {
        cout << "Encrypted file deleted: " << encryptedFilename << endl;
    }

    cout << "Decryption complete. File restored as: " << originalFilename << endl;
}
