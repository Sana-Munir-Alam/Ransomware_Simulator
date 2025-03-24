# NOTIFICATION for TEAM
the file OOp_Applied has beend edited to now work based upon OOP principle and has most of the functionality done such as create file, encrypt file and decrypt file. The code Alhamdulillah works.

## OOP Principles Used

| OOP Principle   | Type Used |
|----------------|-----------|
| **Inheritance** | Single Inheritance (All encryption classes inherit from `Encryption`). |
| **Polymorphism** | Runtime Polymorphism (Function overriding using `virtual` functions). |
| **Abstraction** | Implemented via a pure virtual base class (`Encryption`). |
| **Encapsulation** | Used in `VigenereEncryption` to protect the key generation method and the `key` variable. |


# Ransomware Simulator

# Inlcude the Raylib library
#include "/opt/homebrew/include/raylib.h"  for Macbook

# To Compile the Program in Macbook
g++ -o wrap wrap.cpp encryption.cpp ransomware.cpp log.cpp -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit

#  To Run it
./wrap

# File Encryption and Decryption System

## Overview
This project is a GUI-based file encryption and decryption system implemented in **C++** using **raylib**. It allows users to create, encrypt, and decrypt text files with a responsive and interactive interface.

## Main Functionalities

### GUI Interface
- A window with a resolution of **800x600**.
- The interface includes a **title**, **buttons** for file operations, and **text fields** for user input.
- Buttons change appearance when hovered over and are clickable.

### File Creation
- Users can enter a **filename** (without the `.txt` extension) and input content.
- The text input field supports **multi-line text entry** with **automatic word wrapping**.
- Pressing **Enter** in the text content area adds a new line.
- The **Tab** key switches focus between the filename input field and the content field.
- Once a file is created, it is saved in the system.

### Encryption
- The user enters a filename (without `.txt`).
- Selects an encryption method.
- If the file exists, the chosen encryption method is applied.
- Displays **"Encryption Successful!"** upon completion.
- If the file is missing, an **error message** appears for 3 seconds.
- Encryption is performed using functions from `encryption.h`.
- A file **cannot be encrypted multiple times at once** (users must perform the process separately for multiple encryptions).

### Decryption
- The user selects an **encrypted file** and provides a **password** for decryption.
- The system checks if the correct password is entered before decrypting.
- If incorrect or missing, an **error message** appears.
- **Multi-decryption is prevented** (users must decrypt files one at a time).

### Button Interaction
- The buttons (**Create File, Encrypt File, Decrypt File**) have a **hover effect**.
- Clicking a button triggers the respective function.

### Error Handling
- **Prevents overwriting** existing files by checking for duplicate names.
-  **Prevents log.txt encryption** by mistake.
- **Prevents multi-encryption and multi-decryption** to avoid accidental repeated operations.
- Provides **visual indicators** (such as text highlights) to show active input fields.

### Performance Considerations
- Uses **efficient text wrapping** to ensure readability.
- Implements **smooth UI updates** within the main event loop.

## Technologies Used
- **C++**
- **raylib** (for graphical interface and UI rendering)
- **std::filesystem** (for file handling)
- **chrono** (for managing error message timing)

## Future Improvements
- Add support for **more encryption algorithms**.
- Have to work on that Money Simulation
