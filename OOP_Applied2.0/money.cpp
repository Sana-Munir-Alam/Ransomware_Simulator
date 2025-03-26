#include "/opt/homebrew/include/raylib.h"   // Includes the Raylib library for GUI handling
#include "encryption.h"                     // Custom header file for encryption-related functions
#include "ransomware.h"                     // Custom header file for ransomware-related functions
#include "log.h"                            // Custom header file for logging actions
#include <iostream>
#include <fstream>                          // Library for file handling (read/write)
#include <vector>
#include <filesystem>                       // Library for handling file paths and directories
#include <chrono>                           // For working with time-related functions
#include <thread>                           // Includes support for multi-threading (pausing, sleeping, etc.)

using namespace std;
namespace fs = std::filesystem;             // Creating a shorthand namespace for filesystem functions

// Window States
enum WindowState { MAIN_MENU, PAY_MONEY, LOGIN, TRANSACTION, DECRYPT };
WindowState currentWindow = MAIN_MENU;

#define MAX_LINES 11
#define BOX_WIDTH 400
#define LINE_HEIGHT 20

const int screenWidth = 800;
const int buttonWidth = 200;
const int buttonHeight = 50;
const int buttonSpacing = 20;

float totalWidth = (buttonWidth * 3) + (buttonSpacing * 2);
float startX = (screenWidth - totalWidth) / 2;
float buttonDY = 300; // Y position for buttons decrytption
float buttonEY = 240; // Y position for buttons encryption
char correctAccount[10] = "123456789";
int requiredAmount = 10000;

// Function prototypes for different operations
void DrawMainMenu();    // Function to display the main menu
void CreateFile();      // Function to create a file with user input
void EncryptFile();     // Function to encrypt a selected file
void DecryptFile();     // Function to decrypt a selected file
void PayMoney();
void LoginScreen();
void TransactionScreen();

int main() {
    InitWindow(800, 600, "Ransomware Simulation");  // Creates a window with the title "Ransomware Simulation"
    SetTargetFPS(60);                               // Sets the frame rate to 60 FPS

    // Main game loop - keeps running until the window is closed
    while (!WindowShouldClose()) {
        BeginDrawing();                 // Starts drawing the frame
        ClearBackground(RAYWHITE);      // Clears the screen with a white background
        // Handle the current window state
        switch (currentWindow) {
            case MAIN_MENU: DrawMainMenu(); break;
            case PAY_MONEY: PayMoney(); break;
            case LOGIN: LoginScreen(); break;
            case TRANSACTION: TransactionScreen(); break;
            case DECRYPT: DecryptFile(); break;
        }
        EndDrawing();                   // Ends drawing and updates the screen
    }

    CloseWindow();  // Closes the window when the loop ends
    return 0;
}

// Function to draw the main menu and handle button clicks
void DrawMainMenu() {
    const char *Title = "Ransomware Simulation";
    int FontSize = 20;
    int textWidth = MeasureText(Title, FontSize);   // Measure the width of the text
    int centerX = (screenWidth - textWidth) / 2;    // Center X position
    DrawText(Title, centerX, 50, FontSize, BLACK);  // Draw the text centered horizontally at y = 50

    // Detect mouse position
    Vector2 MousePosition = GetMousePosition();

    // Define clickable buttons with their positions and sizes
    Rectangle CreateButton = {300, 150, 200, 50};
    Rectangle EncryptButton = {300, 250, 200, 50};
    //Rectangle DecryptButton = {300, 350, 200, 50};
    Rectangle PaymentButton = {300, 350, 200, 50};

    // Check if mouse is hovering over the buttons
    bool HoverCreate = CheckCollisionPointRec(MousePosition, CreateButton);
    bool HoverEncrypt = CheckCollisionPointRec(MousePosition, EncryptButton);
    bool HoverPayment = CheckCollisionPointRec(MousePosition, PaymentButton);

    // Change color if hovered, otherwise use default color
    Color CreateColor = HoverCreate ? DARKBLUE : BLUE;
    Color EncryptColor = HoverEncrypt ? DARKGREEN : GREEN;
    Color PaymentColor = HoverPayment ? MAROON : RED;

    // Draw the buttons on the screen
    DrawRectangleRec(CreateButton, CreateColor);
    DrawText("Create File", CreateButton.x + 20, CreateButton.y + 15, 20, WHITE);

    DrawRectangleRec(EncryptButton, EncryptColor);
    DrawText("Encrypt File", EncryptButton.x + 20, EncryptButton.y + 15, 20, WHITE);

    DrawRectangleRec(PaymentButton, PaymentColor);
    DrawText("Payment", PaymentButton.x + 20, PaymentButton.y + 15, 20, WHITE);

    // Check if the user clicks the "Create File" button
    if (CheckCollisionPointRec(MousePosition, CreateButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { CreateFile(); }
    // Check if the user clicks the "Encrypt File" button
    if (CheckCollisionPointRec(MousePosition, EncryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { EncryptFile(); }
    // Check if the user clicks the "Payment" button
    if (CheckCollisionPointRec(MousePosition, PaymentButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { PayMoney(); }
}

// Function to handle file creation with user input
vector<string> WrapText(const string &Text, int MaxWidth, int FontSize) {
    vector<string> Lines;   // Stores the wrapped lines of text
    string CurrentLine;     // Holds the text for the current line
    string Word;            // Holds individual words while processing
    int LineWidth = 0;      // Tracks the current line's width
    
    for (char c : Text) {   // Reads Character by character
        if (c == ' ' || c == '\n') {    // Handling Space and NewLine
            int WordWidth = MeasureText(Word.c_str(), FontSize);    // Calculating Pixel Width of text
            if (LineWidth + WordWidth >= MaxWidth) {    // If the line width is filled with text
                Lines.push_back(CurrentLine);           // Save the current line
                CurrentLine = Word + " ";
                LineWidth = WordWidth + MeasureText(" ", FontSize);
            } else {
                CurrentLine += Word + " ";
                LineWidth += WordWidth + MeasureText(" ", FontSize);
            }
            Word.clear();
            if (c == '\n') {                    // When the ENTER button is pressed
                Lines.push_back(CurrentLine);   // Save the current line
                CurrentLine.clear();            // Start a new line
                LineWidth = 0;                  // Reset width
            }
        } else {
            Word += c;  // Simply Save in the character
        }
    }
    if (!Word.empty()) {    // Handling Unfinished word
        if (LineWidth + MeasureText(Word.c_str(), FontSize) > MaxWidth) {
            Lines.push_back(CurrentLine);   // Save full line
            CurrentLine = Word;             // Start new line with last word
        } else {
            CurrentLine += Word;            // Append word to last line
        }
    }
    if (!CurrentLine.empty()) {
        Lines.push_back(CurrentLine);       //Ensures that the last line of text is always stored in Lines before returning.
    }
    return Lines;
}

void CreateFile() {
    char Filename[128] = "";
    string Content;
    int FilenameIndex = 0;
    bool EnteringFilename = true;
    bool done = false;
    bool CreationSuccess = false;
    bool FileExistsError = false;

    Rectangle ContentBox = {150, 230, 500, MAX_LINES * LINE_HEIGHT}; // Start x coordinate, start y-coordinate, width, height
    bool isEditingContent = false;
    int FontSize = 20;
    
    while (!done) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Enter Filename (without .txt):", 250, 100, 20, BLACK);
        DrawText("Enter Content:", 250, 200, 20, BLACK);

        // Highlight the active input field
        if (EnteringFilename) {
            DrawRectangleLines(250, 130, 300, 30, RED); // Highlight filename input box
        } else {
            DrawRectangleLinesEx(ContentBox, 1, RED); // Highlight content input box
        }

        DrawText(Filename, 260, 140, 20, BLACK);

        // Wrap the text content to fit inside the ContentBox width, breaking it into multiple lines
        vector<string> wrappedText = WrapText(Content, ContentBox.width, FontSize);
        // Loop through each wrapped line and draw it inside the ContentBox
        for (size_t i = 0; i < wrappedText.size() && i < MAX_LINES; i++) {
            /*
                Draw each line of wrapped text inside the content box:
                - wrappedText[i].c_str() → Converts the string to a C-style string.
                - ContentBox.x + 5       → Adds padding (5 pixels) from the left edge of the box.
                - ContentBox.y + 5       → Adds padding (5 pixels) from the top edge of the box.
                - i * LINE_HEIGHT        → Moves each line downward so they don’t overlap.
            */
            DrawText(wrappedText[i].c_str(), ContentBox.x + 5, ContentBox.y + 5 + i * LINE_HEIGHT, FontSize, BLACK);
        }

        if (EnteringFilename) {
            int key = GetCharPressed();
            if (key > 0 && FilenameIndex < 127) {
                Filename[FilenameIndex++] = (char)key;
                Filename[FilenameIndex] = '\0'; // Null-terminate the string
            }
            if (IsKeyPressed(KEY_BACKSPACE) && FilenameIndex > 0) {
                Filename[--FilenameIndex] = '\0';
            }
        } else {
            int key = GetCharPressed();
            if (key > 0) { Content += (char)key; }
            if (IsKeyPressed(KEY_BACKSPACE) && !Content.empty()) { Content.pop_back(); }
            if (IsKeyPressed(KEY_ENTER)) { Content += '\n'; }   // Add newline when Enter is pressed
        }

        // Toggle between filename and content fields using TAB key
        if (IsKeyPressed(KEY_TAB)) { EnteringFilename = !EnteringFilename; }

        Vector2 MousePosition = GetMousePosition();

        // Define Button Boxes [Save and Back]
        Rectangle SaveButton = {190, 500, buttonWidth, buttonHeight};
        Rectangle BackButton = {190 + buttonWidth + buttonSpacing, 500, buttonWidth, buttonHeight};

        // Check if mouse is hovering over the buttons
        bool HoverSave = CheckCollisionPointRec(MousePosition, SaveButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color SaveColor = HoverSave ? DARKGREEN : GREEN;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // Draw Save Button
        DrawRectangleRec(SaveButton, SaveColor);
        DrawText("Save File", SaveButton.x + 50, SaveButton.y + 15, 20, WHITE);

        // Draw Back Button
        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 75, BackButton.y + 15, 20, WHITE);

        // Save the file when the Save button is clicked or Enter key is pressed
        if ((CheckCollisionPointRec(MousePosition, SaveButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            if (FilenameIndex > 0) {  // Ensure the filename is not empty
                string FullFilename = string(Filename) + ".txt";  // Append .txt extension
                // Check if file already exists
                ifstream existingFile(FullFilename);
                if (existingFile) {  
                    FileExistsError = true;  // File already exists
                    existingFile.close();
                } else {
                    ofstream file(FullFilename);  // Create and open the file
                    file << Content;  // Write content to the file
                    file.close();  // Close the file
                    LogAction("File " + FullFilename + " created");
                    CreationSuccess = true;  // Set success flag to true
                    FileExistsError = false; // Reset error flag if file is successfully created
                }
            }
        }

        // Handle Back Button Click
        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            done = true;  // Exit loop without saving
        }

        // Display messages
        if (CreationSuccess) {
            DrawText("File Creation successful!", 300, 470, 20, BLUE);
        } else if (FileExistsError) {
            DrawText("Error: File already exists!", 300, 470, 20, RED);
        }

        EndDrawing();
    }
}

void EncryptFile() {
    char Filename[128] = "";  // Buffer to store the filename input from the user
    int FilenameIndex = 0;  // Tracks the length of the filename
    bool done = false;  // Flag to indicate when to exit the loop
    bool EncryptionSuccess = false;  // Flag to indicate whether encryption was successful
    bool encryptionInProgress = false;  // Flag to lock other buttons after encryption starts

    string errorMessage = "";  // Stores the error message if an error occurs
    chrono::time_point<chrono::steady_clock> errorStartTime;  // Stores the time when an error occurs
    bool showError = false;  // Flag to control the display of the error message

    while (!done) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display input prompts
        DrawText("Enter Filename (without .txt):", 250, 100, 20, BLACK);
        DrawRectangleLines(250, 130, 300, 30, RED);
        DrawText(Filename, 260, 140, 20, BLACK);

        // Handle keyboard input for filename
        int key = GetCharPressed();
        if (key > 0 && FilenameIndex < 127) {
            Filename[FilenameIndex++] = (char)key;
            Filename[FilenameIndex] = '\0';
        }

        if (IsKeyPressed(KEY_BACKSPACE) && FilenameIndex > 0) {
            Filename[--FilenameIndex] = '\0';
        }

        // Define buttons
        Rectangle EncryptButton = {startX, buttonEY, buttonWidth, buttonHeight};
        Rectangle VigenereButton = {startX, buttonEY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight};
        Rectangle CaesarButton = {startX + buttonWidth + buttonSpacing, buttonEY, buttonWidth, buttonHeight};
        Rectangle RailFenceButton = {startX + (buttonWidth + buttonSpacing) * 2, buttonEY, buttonWidth, buttonHeight};
        Rectangle BackButton = {300, 500, 200, 50};

        Vector2 MousePosition = GetMousePosition();

        // Check if mouse is hovering over the buttons
        bool HoverEncrypt = CheckCollisionPointRec(MousePosition, EncryptButton);
        bool HoverVigenere = CheckCollisionPointRec(MousePosition, VigenereButton);
        bool HoverCaesar = CheckCollisionPointRec(MousePosition, CaesarButton);
        bool HoverRailFence = CheckCollisionPointRec(MousePosition, RailFenceButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color EncryptColor = HoverEncrypt ? DARKGREEN : GREEN;
        Color VigenereColor = HoverVigenere ? BROWN : BEIGE;
        Color CaesarColor = HoverCaesar ? DARKBLUE : BLUE;
        Color RailFenceColor = HoverRailFence ? MAROON : RED;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // If encryption is NOT in progress, buttons work normally
        if (!encryptionInProgress) {
            // Encrypt Button [Reverse String encryptio]
            DrawRectangleRec(EncryptButton, EncryptColor);
            DrawText("Encrypt", EncryptButton.x + 70, EncryptButton.y + 15, 20, WHITE);    // ALign the text in the center

            if (CheckCollisionPointRec(MousePosition, EncryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                // Prevent encryption of log.txt
                if (FullFilename == "log.txt") {
                    errorMessage = "Error: Cannot encrypt log file!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }else if (fs::exists(FullFilename)) {
                    ReverseEncryption revEnc; // CREATING REVERSE ENCRYPTION OBJECT
                    revEnc.encrypt(FullFilename);
                    LogAction("File " + FullFilename + " encrypted");
                    EncryptionSuccess = true;
                    encryptionInProgress = true;  // Lock other buttons
                } else {
                    errorMessage = "Error: File does not exist!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }
            }

            // Vignere Button
            DrawRectangleRec(VigenereButton, VigenereColor);
            DrawText("Vigenere", VigenereButton.x + 70, VigenereButton.y + 15, 20, WHITE);    // ALign the text in the center

            if (CheckCollisionPointRec(MousePosition, VigenereButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                // Prevent encryption of log.txt
                if (FullFilename == "log.txt") {
                    errorMessage = "Error: Cannot encrypt log file!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }else if (fs::exists(FullFilename)) {
                    VigenereEncryption vigEnc; // CREATING REVERSE ENCRYPTION OBJECT
                    vigEnc.encrypt(FullFilename);
                    LogAction("File " + FullFilename + " encrypted using Vignere Cypher");
                    EncryptionSuccess = true;
                    encryptionInProgress = true;  // Lock other buttons
                } else {
                    errorMessage = "Error: File does not exist!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }
            }

            // Caesar Button
            DrawRectangleRec(CaesarButton, CaesarColor);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, WHITE);    // ALign the text in the center

            if (CheckCollisionPointRec(MousePosition, CaesarButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                // Prevent encryption of log.txt
                if (FullFilename == "log.txt") {
                    errorMessage = "Error: Cannot encrypt log file!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                } else if (fs::exists(FullFilename)) {
                    CeaserCipher caesarEnc; // CREATING CEASER ENCRYPTION OBJECT
                    caesarEnc.encrypt(FullFilename);
                    LogAction("File " + FullFilename + " encrypted using Caesar Cipher");
                    EncryptionSuccess = true;
                    encryptionInProgress = true;  // Lock other buttons
                } else {
                    errorMessage = "Error: File does not exist!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }
            }

            // RailFence Button
            DrawRectangleRec(RailFenceButton, RailFenceColor);
            DrawText("RailFence", RailFenceButton.x + 50, RailFenceButton.y + 15, 20, WHITE);    // ALign the text in the center

            if (CheckCollisionPointRec(MousePosition, RailFenceButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                // Prevent encryption of log.txt
                if (FullFilename == "log.txt") {
                    errorMessage = "Error: Cannot encrypt log file!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                } else if (fs::exists(FullFilename)) {
                    RailFenceEncryption railEnc; // CREATING CEASER ENCRYPTION OBJECT
                    railEnc.encrypt(FullFilename);
                    LogAction("File " + FullFilename + " encrypted using RailFence Cipher");
                    EncryptionSuccess = true;
                    encryptionInProgress = true;  // Lock other buttons
                } else {
                    errorMessage = "Error: File does not exist!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }
            }
        } else {
            // If encryption is in progress, make the buttons look inactive (gray)
            DrawRectangleRec(EncryptButton, GRAY);
            DrawText("Encrypt", EncryptButton.x + 70, EncryptButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(VigenereButton, GRAY);
            DrawText("Vignere", VigenereButton.x + 70, VigenereButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(CaesarButton, GRAY);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(RailFenceButton, GRAY);
            DrawText("RailFence", RailFenceButton.x + 50, RailFenceButton.y + 15, 20, DARKGRAY);
        }

        // Back Button (always active)
        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 75, BackButton.y + 15, 20, WHITE);

        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            done = true;  // Exit
            encryptionInProgress = false;  // Reset encryption state
        }

        // Display success message
        if (EncryptionSuccess) {
            DrawText("Encryption Successful!", 280, 430, 20, BLACK);
        }

        // Display error message for 3 seconds
        if (showError) {
            int textWidth = MeasureText(errorMessage.c_str(), 20);
            int textX = (screenWidth - textWidth) / 2;
            DrawText(errorMessage.c_str(), textX, 430, 20, RED);
            if (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - errorStartTime).count() > 3) {
                showError = false;
            }
        }

        EndDrawing();
    }
}

void DecryptFile() {
    char Filename[128] = "";  // Buffer to store filename input
    char password[128] = "";  // Buffer to store password input
    int FilenameIndex = 0;  // Tracks filename length
    int passwordIndex = 0;  // Tracks password length
    bool EnteringFilename = true;  // Tracks if the user is entering the filename or password
    bool done = false;  // Flag to exit the loop
    bool DecryptionSuccess = false;  // Flag to indicate decryption success
    bool DecryptionInProgress = false; // Ensures only one decryption can be performed at a time

    string errorMessage = "";  // Stores error messages
    chrono::time_point<chrono::steady_clock> errorStartTime;  // Time of error occurrence
    bool showError = false;  // Flag to track error display

    while (!done) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display input prompts
        DrawText("Enter Filename (without .txt):", 250, 100, 20, BLACK);
        DrawText("Enter Password:", 250, 200, 20, BLACK);

        // Highlight the active input box
        if (EnteringFilename) {
            DrawRectangleLines(250, 130, 300, 30, RED); // File
        } else {
            DrawRectangleLines(250, 230, 300, 30, RED); // Password
        }

        DrawText(Filename, 260, 140, 20, BLACK);  // Display entered filename

        // Hide password input with asterisks
        string hiddenPassword(passwordIndex, '*');
        DrawText(hiddenPassword.c_str(), 260, 240, 20, BLACK);

        // Handle keyboard input
        int key = GetCharPressed();
        if (key > 0) {
            if (EnteringFilename && FilenameIndex < 127) {
                Filename[FilenameIndex++] = (char)key;
                Filename[FilenameIndex] = '\0';
            } else if (!EnteringFilename && passwordIndex < 127) {
                password[passwordIndex++] = (char)key;
                password[passwordIndex] = '\0';
            }
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (EnteringFilename && FilenameIndex > 0) {
                Filename[--FilenameIndex] = '\0';
            } else if (!EnteringFilename && passwordIndex > 0) {
                password[--passwordIndex] = '\0';
            }
        }

        // Switch input field with TAB key
        if (IsKeyPressed(KEY_TAB)) {
            EnteringFilename = !EnteringFilename;
        }

        Vector2 MousePosition = GetMousePosition();

        // Define Buttons
        Rectangle DecryptButton = {startX, buttonDY, buttonWidth, buttonHeight};
        Rectangle VigenereButton = {startX, buttonDY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight};
        Rectangle CaesarButton = {startX + buttonWidth + buttonSpacing, buttonDY, buttonWidth, buttonHeight};
        Rectangle RailFenceButton = {startX + (buttonWidth + buttonSpacing) * 2, buttonDY, buttonWidth, buttonHeight};
        Rectangle BackButton = {300, 500, 200, 50};

        // Check if mouse is hovering over the buttons
        bool HoverDecrypt = CheckCollisionPointRec(MousePosition, DecryptButton);
        bool HoverVigenere = CheckCollisionPointRec(MousePosition, VigenereButton);
        bool HoverCaesar = CheckCollisionPointRec(MousePosition, CaesarButton);
        bool HoverRailFence = CheckCollisionPointRec(MousePosition, RailFenceButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color DecryptColor = HoverDecrypt ? DARKGREEN : GREEN;
        Color VigenereColor = HoverVigenere ? BROWN : BEIGE;
        Color CaesarColor = HoverCaesar ? DARKBLUE : BLUE;
        Color RailFenceColor = HoverRailFence ? MAROON : RED;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // Draw Buttons (Disabled if decryption is in progress)
        if (!DecryptionInProgress) {
            DrawRectangleRec(DecryptButton, DecryptColor);
            DrawText("Decrypt", DecryptButton.x + 70, DecryptButton.y + 15, 20, WHITE);    // ALign the text in the center

            DrawRectangleRec(VigenereButton, VigenereColor);
            DrawText("Vignere", VigenereButton.x + 60, VigenereButton.y + 15, 20, WHITE);    // ALign the text in the center

            DrawRectangleRec(CaesarButton, CaesarColor);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, WHITE);    // ALign the text in the center

            DrawRectangleRec(RailFenceButton, RailFenceColor);
            DrawText("RailFence", RailFenceButton.x + 60, RailFenceButton.y + 15, 20, WHITE);    // ALign the text in the center
        } else {
            // If decryption is in progress, make the buttons look inactive (gray)
            DrawRectangleRec(DecryptButton, GRAY);
            DrawText("Decrypt", DecryptButton.x + 70, DecryptButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(VigenereButton, GRAY);
            DrawText("Vignere", VigenereButton.x + 60, VigenereButton.y + 15, 20, WHITE);

            DrawRectangleRec(CaesarButton, GRAY);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(RailFenceButton, GRAY);
            DrawText("RailFence", RailFenceButton.x + 60, RailFenceButton.y + 15, 20, DARKGRAY);
        }

        // Always Draw Back Button
        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 75, BackButton.y + 15, 20, WHITE);    // ALign the text in the center

        // Handle decryption logic (only if no decryption is already running)
        if (!DecryptionInProgress) {
            if (CheckCollisionPointRec(MousePosition, DecryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;  // Lock other buttons
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    ReverseEncryption revEnc;   //Creating Object for Decryption
                    revEnc.encrypt(FullFilename);
                    //Decrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted");
                    DecryptionSuccess = true;
                } else {
                    errorMessage = "Incorrect password!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                }
            }

            if (CheckCollisionPointRec(MousePosition, VigenereButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;  // Lock other buttons
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    VigenereEncryption vigEnc;  //Creating Object for Decryption
                    vigEnc.decrypt(FullFilename);
                    //Decrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted using Vignere Cypher");
                    DecryptionSuccess = true;
                } else {
                    errorMessage = "Incorrect password!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                }
            }

            if (CheckCollisionPointRec(MousePosition, CaesarButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    CeaserCipher caesarEnc; // CREATING OBEJCT FOR DECRYPTION
                    caesarEnc.decrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted using Caesar Cipher");
                    DecryptionSuccess = true;
                } else {
                    errorMessage = "Incorrect password!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                }
            }

            if (CheckCollisionPointRec(MousePosition, RailFenceButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    RailFenceEncryption railEnc; //CREATING OBJECT
                    railEnc.decrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted using Rail Fence Cipher");
                    DecryptionSuccess = true;
                } else {
                    errorMessage = "Incorrect password!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                }
            }
        }

        // Handle back button click (Always active)
        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = MAIN_MENU;
            return;
        }

        // Show messages
        if (DecryptionSuccess) DrawText("Decryption successful!", 300, 430, 20, BLUE);

        // Show the error message for 3 seconds
        if (showError) {
            int screenWidth = 800;
            int textWidth = MeasureText(errorMessage.c_str(), 20);
            int textX = (screenWidth - textWidth) / 2;

            DrawText(errorMessage.c_str(), textX, 430, 20, RED);
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - errorStartTime).count() > 3) {
                showError = false;  // Hide the message after 3 seconds
            }
        }
        EndDrawing();
    }
}

// MONEY MONEY MONEY

void PayMoney() {
    bool done = false;
    while(!done){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Measure text width dynamically
        int text1Width = MeasureText("Your Important Files Are Encrypted!", 20);
        int text2Width = MeasureText("To Access Them, Pay $10,000 to:", 20);
        int text3Width = MeasureText("Account: 123-456-789", 20);

        // Calculate centered positions
        int text1X = (screenWidth - text1Width) / 2;
        int text2X = (screenWidth - text2Width) / 2;
        int text3X = (screenWidth - text3Width) / 2;

        // Draw text centered
        DrawText("Your Important Files Are Encrypted!", text1X, 100, 20, RED);
        DrawText("To Access Them, Pay $10,000 to:", text2X, 150, 20, BLACK);
        DrawText("Account: 123-456-789", text3X, 180, 20, BLACK);
    
        Vector2 MousePosition = GetMousePosition();
        
        // Define Buttons
        Rectangle ProceedButton = {190, 500, buttonWidth, buttonHeight};
        Rectangle BackButton = {190 + buttonWidth + buttonSpacing, 500, buttonWidth, buttonHeight};

        // Check if mouse is hovering over the buttons
        bool HoverProceed = CheckCollisionPointRec(MousePosition, ProceedButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color ProceedColor = HoverProceed ? DARKBLUE : BLUE;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;
    
        DrawRectangleRec(ProceedButton, ProceedColor);
        DrawText("Proceed >>", ProceedButton.x + 50, ProceedButton.y + 15, 20, WHITE);
    
        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 70, BackButton.y + 15, 20, WHITE);

        // Handle Proceed button click (Always active)
        if (CheckCollisionPointRec(MousePosition, ProceedButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = LOGIN;
            return;
        }
        // Handle back button click (Always active)
        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = MAIN_MENU;
            return;
        }
        EndDrawing();
    }
}

void LoginScreen() {
    char EnteredAccount[12] = "";
    char EnteredPassword[10] = "";
    float EnteredAmount = 0;
    int PasswordIndex = 0;
    int AccountIndex = 0;
    bool showPassword = false;
    bool done = false;
    bool EnteringValue = true;

    while(!done){
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Measure text width dynamically
        int titleWidth = MeasureText("Login to User Bank Account", 30);
        int prompt1Width = MeasureText("Enter Account No", 20);
        int prompt2Width = MeasureText("Enter Password:", 20);
        
        int enteredAccountWidth = MeasureText(EnteredAccount, 20);
        int enteredPasswordWidth = MeasureText(EnteredPassword, 20);

        // Calculate centered positions
        int titleX = (screenWidth - titleWidth) / 2;
        int prompt1X = (screenWidth - prompt1Width) / 2;
        int prompt2X = (screenWidth - prompt2Width) / 2;
        
        int inputBoxX = (screenWidth - 300) / 2;

        int enteredAccountX = (screenWidth - enteredAccountWidth) / 2;
        int enteredPasswordX = (screenWidth - enteredPasswordWidth) / 2;
        
        // Draw centered text
        DrawText("Login to User Bank Account", titleX, 100, 30, BLACK);
        DrawText("Enter Account No", prompt1X, 180, 20, BLACK);
        DrawText("Enter Password:", prompt2X, 280, 20, BLACK);

        // Highlight the active input box
        if (EnteringValue) {
            DrawRectangleLines(inputBoxX, 210, 300, 30, RED); // Account input box
        } else {
            DrawRectangleLines(inputBoxX, 310, 300, 30, RED); // Password input box
        }

        DrawText(EnteredAccount, enteredAccountX, 210, 20, BLACK); // Draw entered text within input box

        // Hide password input with asterisks
        string hiddenPassword(PasswordIndex, '*');
        DrawText(hiddenPassword.c_str(), enteredPasswordX, 310, 20, BLACK);

        // Handle keyboard input
        int key = GetCharPressed();
        if (key > 0) {
            if (EnteringValue && AccountIndex < 11) {
                EnteredAccount[AccountIndex++] = (char)key;
                EnteredAccount[AccountIndex] = '\0';
            } else if (!EnteringValue && PasswordIndex < 9) {
                EnteredPassword[PasswordIndex++] = (char)key;
                EnteredPassword[PasswordIndex] = '\0';
            }
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (EnteringValue && AccountIndex > 0) {
                EnteredAccount[--AccountIndex] = '\0';
            } else if (!EnteringValue && PasswordIndex > 0) {
                EnteredPassword[--PasswordIndex] = '\0';
            }
        }

        // Switch input field with TAB key
        if (IsKeyPressed(KEY_TAB)) {
            EnteringValue = !EnteringValue;
        }

        Vector2 MousePosition = GetMousePosition();
        
        // Define Buttons
        Rectangle LoginButton = {190, 500, buttonWidth, buttonHeight};
        Rectangle BackButton = {190 + buttonWidth + buttonSpacing, 500, buttonWidth, buttonHeight};

        // Check if mouse is hovering over the buttons
        bool HoverLogin = CheckCollisionPointRec(MousePosition, LoginButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color LoginColor = HoverLogin ? DARKBLUE : BLUE;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;
    
        DrawRectangleRec(LoginButton, LoginColor);
        DrawText("Login", LoginButton.x + 70, LoginButton.y + 15, 20, WHITE);
    
        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 70, BackButton.y + 15, 20, WHITE);

        // Handle Login button click (Always active)
        if (CheckCollisionPointRec(MousePosition, LoginButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = TRANSACTION;
            return;
        }
        // Handle back button click (Always active)
        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = MAIN_MENU;
            return;
        }
        EndDrawing();
    }
}

void TransactionScreen() {
    char EnteredAccount[12] = "";
    char EnteredAmount[10] = "";
    int AccountIndex = 0;
    int AmountIndex = 0;
    bool EnteringAccount = true;
    bool done = false;

    while (!done) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Measure text widths
        int titleWidth = MeasureText("Ransom Payment Transaction", 30);
        int prompt1Width = MeasureText("Enter Ransom Account:", 20);
        int prompt2Width = MeasureText("Enter Amount:", 20);

        int enteredAccountWidth = MeasureText(EnteredAccount, 20);
        int enteredAmountWidth = MeasureText(EnteredAmount, 20);

        // Centered X positions
        int titleX = (screenWidth - titleWidth) / 2;
        int prompt1X = (screenWidth - prompt1Width) / 2;
        int prompt2X = (screenWidth - prompt2Width) / 2;

        int inputBoxX = (screenWidth - 300) / 2;

        int enteredAccountX = (screenWidth - enteredAccountWidth) / 2;
        int enteredAmountX = (screenWidth - enteredAmountWidth) / 2;

        // Draw centered text
        DrawText("Ransom Payment Transaction", titleX, 100, 30, BLACK);
        DrawText("Enter Ransom Account:", prompt1X, 180, 20, BLACK);
        DrawText("Enter Amount:", prompt2X, 280, 20, BLACK);

        // Highlight the active input box
        if (EnteringAccount) {
            DrawRectangleLines(inputBoxX, 210, 300, 30, RED);
        } else {
            DrawRectangleLines(inputBoxX, 310, 300, 30, RED);
        }

        // Display entered values centered
        DrawText(EnteredAccount, enteredAccountX, 210, 20, BLACK);
        DrawText(EnteredAmount, enteredAmountX, 310, 20, BLACK);

        // Handle keyboard input
        int key = GetCharPressed();
        if (key > 0) {
            if (EnteringAccount && AccountIndex < 11) {
                EnteredAccount[AccountIndex++] = (char)key;
                EnteredAccount[AccountIndex] = '\0';
            } else if (!EnteringAccount && AmountIndex < 9) {
                if (isdigit(key) || key == '.') {  // Only allow digits and decimal point for amount
                    EnteredAmount[AmountIndex++] = (char)key;
                    EnteredAmount[AmountIndex] = '\0';
                }
            }
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (EnteringAccount && AccountIndex > 0) {
                EnteredAccount[--AccountIndex] = '\0';
            } else if (!EnteringAccount && AmountIndex > 0) {
                EnteredAmount[--AmountIndex] = '\0';
            }
        }

        // Switch input field with TAB key
        if (IsKeyPressed(KEY_TAB)) {
            EnteringAccount = !EnteringAccount;
        }

        Vector2 MousePosition = GetMousePosition();

        // Define buttons
        Rectangle PayButton = {190, 500, buttonWidth, buttonHeight};
        Rectangle BackButton = {190 + buttonWidth + buttonSpacing, 500, buttonWidth, buttonHeight};

        // Check if mouse is hovering over the buttons
        bool HoverPay = CheckCollisionPointRec(MousePosition, PayButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color PayColor = HoverPay ? DARKBLUE : BLUE;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // Change color if hovered
        DrawRectangleRec(PayButton, PayColor);
        DrawText("Pay", PayButton.x + 80, PayButton.y + 15, 20, WHITE);

        DrawRectangleRec(BackButton, BackColor);
        DrawText("Back", BackButton.x + 70, BackButton.y + 15, 20, WHITE);

        // Handle Pay button click
        if (HoverPay && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (strcmp(EnteredAccount, correctAccount) == 0 && atof(EnteredAmount) == requiredAmount) {
                for (int i = 0; i <= 100; i += 10) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText("Processing Transaction...", 280, 250, 20, BLACK);
                    DrawRectangle(250, 300, i * 3, 30, GREEN);
                    EndDrawing();
                    WaitTime(0.2);
                }
                currentWindow = DECRYPT;
                return;
            }
        }

        // Handle back button click (Always active)
        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            currentWindow = MAIN_MENU;
            return;
        }

        EndDrawing();
    }
}
