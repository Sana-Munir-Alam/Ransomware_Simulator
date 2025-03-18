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

#define MAX_LINES 11
#define BOX_WIDTH 400
#define LINE_HEIGHT 20

const int screenWidth = 800;
const int buttonWidth = 200;
const int buttonHeight = 50;
const int buttonSpacing = 20;

float totalWidth = (buttonWidth * 3) + (buttonSpacing * 2);
float startX = (screenWidth - totalWidth) / 2;
float buttonY = 300; // Y position for buttons

// Function prototypes for different operations
void DrawMainMenu();    // Function to display the main menu
void CreateFile();      // Function to create a file with user input
void EncryptFile();     // Function to encrypt a selected file
void DecryptFile();     // Function to decrypt a selected file

// Global variables
bool FileCreated = false;   // Boolean flag to track whether a file has been created
string SelectedFile;        // Stores the name of the currently selected file

int main() {
    InitWindow(800, 600, "Ransomware Simulation");  // Creates a window with the title "Ransomware Simulation"
    SetTargetFPS(60);                               // Sets the frame rate to 60 FPS

    // Main game loop - keeps running until the window is closed
    while (!WindowShouldClose()) {
        BeginDrawing();                 // Starts drawing the frame
        ClearBackground(RAYWHITE);      // Clears the screen with a white background
        DrawMainMenu();                 // Calls the function to render the main menu
        EndDrawing();                   // Ends drawing and updates the screen
    }

    CloseWindow();  // Closes the window when the loop ends
    return 0;
}

// Function to draw the main menu and handle button clicks
void DrawMainMenu() {
    const char *Title = "Ransomware Simulation";
    int FontSize = 20;
    // Measure the width of the text
    int textWidth = MeasureText(Title, FontSize);
    // Center X position
    int centerX = (screenWidth - textWidth) / 2;
    // Draw the text centered horizontally at y = 50
    DrawText(Title, centerX, 50, FontSize, BLACK);

    // Detect mouse position
    Vector2 MousePosition = GetMousePosition();

    // Define clickable buttons with their positions and sizes
    Rectangle CreateButton = {300, 150, 200, 50};
    Rectangle EncryptButton = {300, 250, 200, 50};
    Rectangle DecryptButton = {300, 350, 200, 50};

    // Check if mouse is hovering over the buttons
    bool HoverCreate = CheckCollisionPointRec(MousePosition, CreateButton);
    bool HoverEncrypt = CheckCollisionPointRec(MousePosition, EncryptButton);
    bool HoverDecrypt = CheckCollisionPointRec(MousePosition, DecryptButton);

    // Change color if hovered, otherwise use default color
    Color CreateColor = HoverCreate ? DARKBLUE : BLUE;
    Color EncryptColor = HoverEncrypt ? DARKGREEN : GREEN;
    Color DecryptColor = HoverDecrypt ? MAROON : RED;

    // Draw the buttons on the screen
    DrawRectangleRec(CreateButton, CreateColor);
    DrawText("Create File", CreateButton.x + 20, CreateButton.y + 15, 20, WHITE);

    DrawRectangleRec(EncryptButton, EncryptColor);
    DrawText("Encrypt File", EncryptButton.x + 20, EncryptButton.y + 15, 20, WHITE);

    DrawRectangleRec(DecryptButton, DecryptColor);
    DrawText("Decrypt File", DecryptButton.x + 20, DecryptButton.y + 15, 20, WHITE);

    // Check if the user clicks the "Create File" button
    if (CheckCollisionPointRec(MousePosition, CreateButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { CreateFile(); }
    // Check if the user clicks the "Encrypt File" button
    if (CheckCollisionPointRec(MousePosition, EncryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { EncryptFile(); }
    // Check if the user clicks the "Decrypt File" button
    if (CheckCollisionPointRec(MousePosition, DecryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { DecryptFile(); }
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
            if (LineWidth + WordWidth >= MaxWidth) {
                Lines.push_back(CurrentLine);
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
    if (!Word.empty()) {    // Handlng Unfinished word
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
        Color BackColor = HoverBack ? MAROON : RED;

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
        Rectangle EncryptButton = {startX, buttonY, buttonWidth, buttonHeight};
        Rectangle CaesarButton = {startX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight};
        Rectangle PlayFairButton = {startX + (buttonWidth + buttonSpacing) * 2, buttonY, buttonWidth, buttonHeight};
        Rectangle BackButton = {300, 500, 200, 50};

        Vector2 MousePosition = GetMousePosition();

        // Check if mouse is hovering over the buttons
        bool HoverEncrypt = CheckCollisionPointRec(MousePosition, EncryptButton);
        bool HoverCaesar = CheckCollisionPointRec(MousePosition, CaesarButton);
        bool HoverPlayFair = CheckCollisionPointRec(MousePosition, PlayFairButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color EncryptColor = HoverEncrypt ? DARKGREEN : GREEN;
        Color CaesarColor = HoverCaesar ? DARKBLUE : BLUE;
        Color PlayFairColor = HoverPlayFair ? MAROON : RED;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // If encryption is NOT in progress, buttons work normally
        if (!encryptionInProgress) {
            // Encrypt Button [Reverse String encryptio]
            DrawRectangleRec(EncryptButton, EncryptColor);
            DrawText("Encrypt", EncryptButton.x + 70, EncryptButton.y + 15, 20, WHITE);

            if (CheckCollisionPointRec(MousePosition, EncryptButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                if (fs::exists(FullFilename)) {
                    ReverseEncryption revEnc; // CREATING REVERSE ENCRYPTION OBJECT
                    revEnc.encrypt(FullFilename);
                    //Encrypt(FullFilename);    DONT FORGET TO REMOVE THIS
                    LogAction("File " + FullFilename + " encrypted");
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
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, WHITE);

            if (CheckCollisionPointRec(MousePosition, CaesarButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                if (fs::exists(FullFilename)) {
                    CaesarEncryption caesarEnc; // CREATING CEASER ENCRYPTION OBJECT
                    caesarEnc.encrypt(FullFilename);
                    //CaesarEncrypt(FullFilename);  DONT FORGET TO REMOVE THIS
                    LogAction("File " + FullFilename + " encrypted using Caesar Cipher");
                    EncryptionSuccess = true;
                    encryptionInProgress = true;  // Lock other buttons
                } else {
                    errorMessage = "Error: File does not exist!";
                    errorStartTime = chrono::steady_clock::now();
                    showError = true;
                }
            }

            // PlayFair Button
            DrawRectangleRec(PlayFairButton, PlayFairColor);
            DrawText("PlayFair", PlayFairButton.x + 50, PlayFairButton.y + 15, 20, WHITE);

            if (CheckCollisionPointRec(MousePosition, PlayFairButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && FilenameIndex > 0) {
                string FullFilename = string(Filename) + ".txt";
                if (fs::exists(FullFilename)) {
                    PlayFairEncryption playFairEnc; // CREATING CEASER ENCRYPTION OBJECT
                    playFairEnc.encrypt(FullFilename);
                    //PlayFairEncrypt(FullFilename); DONT FORGET TO REMOVE THIS
                    LogAction("File " + FullFilename + " encrypted using PlayFair Cipher");
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

            DrawRectangleRec(CaesarButton, GRAY);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(PlayFairButton, GRAY);
            DrawText("PlayFair", PlayFairButton.x + 50, PlayFairButton.y + 15, 20, DARKGRAY);
        }

        // Back Button (always active)
        DrawRectangleRec(BackButton, BackColor);
        DrawText("BACK", BackButton.x + 75, BackButton.y + 15, 20, WHITE);

        if (CheckCollisionPointRec(MousePosition, BackButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            done = true;  // Exit
            encryptionInProgress = false;  // Reset encryption state
        }

        // Display success message
        if (EncryptionSuccess) {
            DrawText("Encryption Successful!", 280, 400, 20, BLACK);
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
        Rectangle DecryptButton = {startX, buttonY, buttonWidth, buttonHeight};
        Rectangle CaesarButton = {startX + buttonWidth + buttonSpacing, buttonY, buttonWidth, buttonHeight};
        Rectangle PlayFairButton = {startX + (buttonWidth + buttonSpacing) * 2, buttonY, buttonWidth, buttonHeight};
        Rectangle BackButton = {300, 500, 200, 50};

        // Check if mouse is hovering over the buttons
        bool HoverDecrypt = CheckCollisionPointRec(MousePosition, DecryptButton);
        bool HoverCaesar = CheckCollisionPointRec(MousePosition, CaesarButton);
        bool HoverPlayFair = CheckCollisionPointRec(MousePosition, PlayFairButton);
        bool HoverBack = CheckCollisionPointRec(MousePosition, BackButton);

        // Change color if hovered, otherwise use default color
        Color DecryptColor = HoverDecrypt ? DARKGREEN : GREEN;
        Color CaesarColor = HoverCaesar ? DARKBLUE : BLUE;
        Color PlayFairColor = HoverPlayFair ? MAROON : RED;
        Color BackColor = HoverBack ? DARKGRAY : GRAY;

        // Draw Buttons (Disabled if decryption is in progress)
        if (!DecryptionInProgress) {
            DrawRectangleRec(DecryptButton, DecryptColor);
            DrawText("Decrypt", DecryptButton.x + 70, DecryptButton.y + 15, 20, WHITE);

            DrawRectangleRec(CaesarButton, CaesarColor);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, WHITE);

            DrawRectangleRec(PlayFairButton, PlayFairColor);
            DrawText("PlayFair", PlayFairButton.x + 50, PlayFairButton.y + 15, 20, WHITE);
        } else {
            // If encryption is in progress, make the buttons look inactive (gray)
            DrawRectangleRec(DecryptButton, GRAY);
            DrawText("Decrypt", DecryptButton.x + 70, DecryptButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(CaesarButton, GRAY);
            DrawText("Caesar", CaesarButton.x + 60, CaesarButton.y + 15, 20, DARKGRAY);

            DrawRectangleRec(PlayFairButton, GRAY);
            DrawText("PlayFair", PlayFairButton.x + 50, PlayFairButton.y + 15, 20, DARKGRAY);
        }

        // Always Draw Back Button
        DrawRectangleRec(BackButton, BackColor);
        DrawText("BACK", BackButton.x + 75, BackButton.y + 15, 20, WHITE);

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

            if (CheckCollisionPointRec(MousePosition, CaesarButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    CaesarEncryption caesarEnc; // CREATING OBEJCT FOR DECRYPTION
                    caesarEnc.decrypt(FullFilename);
                    //CaesarDecrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted using Caesar Cipher");
                    DecryptionSuccess = true;
                } else {
                    errorMessage = "Incorrect password!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                }
            }

            if (CheckCollisionPointRec(MousePosition, PlayFairButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                DecryptionInProgress = true;
                string FullFilename = string(Filename) + ".txt";

                if (!fs::exists(FullFilename)) {
                    errorMessage = "Error: File does not exist!";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                    DecryptionInProgress = false;
                } else if (VerifyPassword(password)) {
                    PlayFairEncryption playFairEnc; //CREATING OBJECT
                    playFairEnc.decrypt(FullFilename);
                    LogAction("File " + FullFilename + " decrypted using PlayFair Cipher");
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
            done = true;
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