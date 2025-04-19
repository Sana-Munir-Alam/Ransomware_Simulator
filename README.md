# NOTIFICATION for TEAM
Guys the project is complete Alhumdulillah.
It has a proper GUI system[Main.cpp].<br>A code [FileScanner.cpp] that would traverse the desktop and make a list of file and sub-files that exist in it and store it in a text file.<br>A code [Calling.cpp] that answers when to call for encryption and decryption.<br>A code [encryption.cpp] taht currently stores 3 type of encryptions that are applied to files.<br>A code [ransomeware.cpp] that genereats random password which is to be used to decrypt otherwise no decryption will occur.

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
