# NOTIFICATION for TEAM
The file A)Finalized Project is the final and latest folder containing our project. I will update you all about how it is working in one of the free slots.

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
