# NOTIFICATION for TEAM
The Project is done in a sense That it has Proper GUI, 3 encryption [Vignere, Ceaser, RailFence] That are able to encrypt and decrypt any type of text files, proper password is being generated before eecuting the decryption methods.
The whole encryption and Decryption thing is happening properly within the GUI interface. The only problem is that our program is encrypting and decrypting it only when this FinalProject folder content are on the desktop, and not in a folder. SO i am gonna work on that and hopefully find a solution to this.
if you have any suggestion do tell me


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
