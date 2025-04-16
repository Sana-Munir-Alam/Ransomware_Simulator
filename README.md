# NOTIFICATION for TEAM
The file A)Finalized Project is the final and latest folder containing our project. I will update you all about how it is working in one of the free slots. Currently if you run it you will not see encryption happening as i have not linked the process of encryption yet, As of now only .txt files that are exisiting on desktop and folders within are being traversed and getting encrypted/decrypted. I only have to work on the no .TXT files. After that the project would be declared as done.
Another thing being that the ,TXT files will be encrypted using (Vignere, Caeser, RailFence) but the non.TXT files will be encrypted using (Railfence, and Caeser) as their key is numerical and are able to handle the binary format file encryption and decryption. 
Currently Adeena is working on two more types of Encryption that use numerical keys and after we compile it in our project and it is working fine than we will let it remain else we will haev to think of something else.
If any one of you want to add another encryption method do tell, just make sure it's style matches with the already written encryptions in encryption.cpp [in order to prevent the code from crashing due to styles not matching].


## OOP Principles Used

| OOP Principle   | Type Used |
|----------------|-----------|
| **Inheritance** | Single Inheritance (All encryption classes inherit from `Encryption`). |
| **Polymorphism** | Runtime Polymorphism (Function overriding using `virtual` functions). |
| **Abstraction** | Implemented via a pure virtual base class (`Encryption`). |
| **Encapsulation** | Used in `Derived Encryption Classes` to protect the key generation method and the `key` variable. |


# Ransomware Simulator

# Inlcude the Raylib library
#include "/opt/homebrew/include/raylib.h"  for Macbook

# To Compile the Program in Macbook
g++ -o main main.cpp FileScanner.cpp encryption.cpp ransomware.cpp -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

#  To Run it
./main

# File Encryption and Decryption System

## Overview
This project is a GUI-based file encryption and decryption system implemented in **C++** using **raylib**. It allows users to create, encrypt, and decrypt text files with a responsive and interactive interface.

## Future Improvements
- Add support for **more encryption algorithms**.
- Have to now incorporate encryption and decryption on files that Do NOT hav e.TXT extension.
