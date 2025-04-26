# NOTIFICATION for TEAM
Guys the project is complete Alhumdulillah. All the encryptions have been integrated and tested to see if they are working properly.<br>
All files have been properly commented, kindly check it once so that no typo exist, only main.cpp file is left to have more comments will do later. <br>
The final folder of our project is named [Final Project].<br>

# Ransomware Simulation – OOP Project

## Overview

This project is a **simulation of a ransomware attack**, designed as an object-oriented programming (OOP) project. It mimics how a ransomware application might behave.
This project is a GUI-based file encryption and decryption system implemented in **C++** using **raylib**.

The user interface appears as a regular text editing app, but upon saving a file, a simulated "system crash" is triggered. This initiates a mock ransomware attack that encrypts all files on the user's Desktop using a combination of four different encryption algorithms. The user is prompted to "pay a ransom" and, upon doing so, receives a password to unlock and decrypt their files.

It has a proper GUI system [Main.cpp].<br>
A code [FileScanner.cpp] that would traverse the desktop and make a list of file and sub-files that exist in it and store it in a text file.<br>
A code [Calling.cpp] that answers when to call for encryption and decryption.<br>
A code [encryption.cpp] that currently has 4 types of encryptions that are applied to files, More encryption can be added anytime.<br>
A code [ransomeware.cpp] that genereats random password which is to be used to decrypt otherwise no decryption will occur.

---

## How It Works

1. **User Interaction**:
   - User launches a GUI that resembles a basic notepad.
   - They create a file, type its content, and click save.
   - A progress bar halts at 87%, followed by a system "crash" animation.
   - A message is displayed, simulating a ransomware attack.

2. **Behind the Scenes**:
   - All files on the user’s Desktop are scanned and encrypted using a combination of:
     - **Vigenère Cipher**
     - **Caesar Cipher**
     - **Rail Fence Cipher**
     - **Affine Cipher**
   - The system prompts the user to make a "payment".
   - Upon "payment," a password is generated.
   - If the correct password is entered, the decryption process begins.
   - All files are returned to their original, readable state.
---

## Project Structure

| File               | Description |
|--------------------|-------------|
| `main.cpp`         | Handles GUI interaction and simulates the ransomware attack workflow. |
| `Calling.`        | Acts as a handler that decides whether to initiate encryption or decryption based on input. |
| `encryption.`     | Implements the encryption/decryption logic using four classical ciphers via OOP inheritance and polymorphism. |
| `FileScanner.`    | Scans the user’s Desktop and logs paths of files to be encrypted or decrypted. |
| `ransomware.`     | Handles the generation, storage, and verification of the decryption password. |

---

## Object-Oriented Design

The codebase follows core OOP principles:
- **Abstraction**: An abstract `Encryption` base class defines a common interface for all encryption techniques.
- **Polymorphism**: Each cipher class overrides the base class's `encryptTXT()` and `decryptTXT()` methods.
- **Encapsulation**: Each cipher manages its own key generation and storage logic.
- **Modularity**: Each major responsibility is split across separate files for maintainability and clarity.

### Encryption Algorithms Implemented:
- **Vigenère Cipher**: Text-based encryption using a keyword.
- **Caesar Cipher**: Shifts characters by a fixed number; also supports non-text files.
- **Rail Fence Cipher**: Rearranges characters in a zigzag pattern; works with both text and non-text data.
- **Affine Cipher**: Uses mathematical functions for character substitution; includes binary-safe handling.

---

# Ransomware Simulator

# Inlcude the Raylib library
#include "/opt/homebrew/include/raylib.h"  for Macbook

# To Compile the Program in Macbook
g++ -o main main.cpp FileScanner.cpp Calling.cpp encryption.cpp ransomware.cpp -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

#  To Run it
./main

---
# Things Left to do
Create a Project Report including class diagram.<br>
