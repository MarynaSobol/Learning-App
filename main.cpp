#include "raylib.h"                  // Include the Raylib library for window management, input, and drawing
#include "AppTypes.h"                // Include custom application types, screens, and user structs
#include "ContentData.h"             // Include language content databases for C and C++
#include "UI.h"                      // Include custom user interface components and theme settings
#include <algorithm>                 // Include standard algorithms like sort and shuffle
#include <random>                    // Include random number generators for shuffling code blocks
#include <cstring>                   // Include standard C string manipulation functions like strlen
#include <fstream>                   // Include file stream library for reading and writing files
#include <sstream>                   // Include string stream library for text formatting and parsing

// Load user accounts and their XP data from the local text file users.txt
std::vector<User> LoadUsers() {
    std::vector<User> users;                 // Create an empty list to store loaded users
    std::ifstream file("users.txt");         // Open the users text file for reading
    if (!file.is_open()) {                   // Check if the file failed to open (e.g., does not exist yet)
        return {                             // Return default fallback users if the file cannot be opened
            {"Alice_Coder", "123", 450, 5},  // Default user Alice with 450 XP
            {"Bob_CPlusPlus", "123", 320, 3},// Default user Bob with 320 XP
            {"Charlie_Dev", "123", 180, 2}   // Default user Charlie with 180 XP
        };
    }

    std::string line;                        // Variable to store each line read from the file
    while (std::getline(file, line)) {       // Read the file line by line until the end
        if (line.empty()) continue;          // Skip empty lines in the file
        std::stringstream ss(line);          // Wrap the line into a string stream for parsing
        std::string username, password;      // Variables to hold parsed username and password
        int xp, levels;                      // Variables to hold parsed XP and completed levels

        if (std::getline(ss, username, '|') && // Extract username up to the vertical bar delimiter
            std::getline(ss, password, '|') && // Extract password up to the vertical bar delimiter
            (ss >> xp) && (ss.ignore()) &&     // Extract XP points and ignore the delimiter character
            (ss >> levels)) {                  // Extract the number of completed levels
            users.push_back({ username, password, xp, levels }); // Add the parsed user to our list
        }
    }
    file.close();                            // Close the file stream after reading all lines

    if (users.empty()) {                     // Check if the loaded user list ended up completely empty
        users = {                            // Fallback to default users if file data was invalid or empty
            {"Alice_Coder", "123", 450, 5},
            {"Bob_CPlusPlus", "123", 320, 3},
            {"Charlie_Dev", "123", 180, 2}
        };
    }
    return users;                            // Return the final list of loaded users
}

// Save all registered user accounts and their current XP into the local users.txt file
void SaveUsers(const std::vector<User>& users) {
    std::ofstream file("users.txt");         // Open the users text file for writing (overwriting old data)
    if (file.is_open()) {                    // Verify that the file opened successfully
        for (const auto& user : users) {     // Loop through every user in the list
            file << user.username << "|" << user.password << "|" << user.xp << "|" << user.levelsCompleted << "\n"; // Write user details separated by vertical bars
        }
        file.close();                        // Close the file stream after writing all data
    }
}

// Initializes a Constructor puzzle level by setting up slots and shuffling code blocks
void InitConstructorLevel(AppState& state, const ConstructorLevel& level) {
    state.blocks.clear();                    // Clear any existing draggable code blocks from previous attempts
    int numSlots = (int)level.correctLines.size(); // Count how many target slots are needed for this level
    state.slotBlockIndex.assign(numSlots, -1);     // Reset all slots to be empty (-1)

    std::vector<std::string> allTexts = level.correctLines; // Start building the pool with correct code lines
    for (const auto& extra : level.extraBlocks) {           // Loop through any extra decoy code blocks
        allTexts.push_back(extra);                          // Add decoy blocks to the pool to increase difficulty
    }

    std::mt19937 rng(1337 + state.selectedConstructorLevel); // Create a seeded random number generator for shuffling
    std::shuffle(allTexts.begin(), allTexts.end(), rng);     // Randomly shuffle all code blocks in the pool

    int startX = 140;                        // Define the starting horizontal X coordinate for the block pool
    int startY = 485;                        // Define the starting vertical Y coordinate for the block pool
    int colWidth = 460;                      // Define the width of each code block box
    int rowHeight = 46;                      // Define the height spacing for each row in the pool

    for (size_t i = 0; i < allTexts.size(); ++i) { // Loop through every shuffled code text block
        DraggableBlock b;                    // Create a new draggable block object
        b.text = allTexts[i];                // Assign the code text to the block
        int col = (int)(i % 2);              // Calculate column index (0 for left column, 1 for right column)
        int row = (int)(i / 2);              // Calculate row index based on item position
        b.rect = { (float)(startX + col * (colWidth + 30)), (float)(startY + row * rowHeight), (float)colWidth, 38.0f }; // Set rectangle boundaries for mouse interaction
        b.currentSlot = -1;                  // Set initial slot assignment to -1 (sitting in the bottom pool)
        b.isDragging = false;                // Ensure the block is not currently being dragged
        b.dragOffset = { 0, 0 };             // Reset drag offset coordinates
        state.blocks.push_back(b);           // Add the block to the application state list
    }

    state.isCarMoving = false;               // Reset car movement status to stopped
    state.carX = 140.0f;                     // Reset car horizontal starting position
    state.carTargetLine = 0;                 // Reset car target validation line index
    state.carCrashed = false;                // Reset car crash status to false
    state.levelSuccess = false;              // Reset level success status to false
    state.showErrorModal = false;            // Hide error popup modal window
    state.firstErrorLine = -1;               // Reset the tracking index of the first mistake line
}

// Initializes a Visualizer walkthrough lesson by resetting line counters and states
void InitVisualizerLevel(AppState& state, const VisualizerLevel& level) {
    state.showIntroModal = true;                   // Show the initial code overview popup window
    state.revealedLinesCount = 1;                  // Start with only the first line of code visible
    state.explanationVisible.assign(level.steps.size(), false); // Hide all explanation dropdown boxes initially
    state.stepUnderstood.assign(level.steps.size(), false);     // Reset all "Got It!" completion flags to false
}

// Helper function to handle keyboard typing input inside active text boxes in Raylib
void UpdateTextInput(char* buffer, int maxLen) {
    int key = GetCharPressed();              // Get the Unicode value of the key pressed this frame
    while (key > 0) {                        // Loop while there are queued key presses
        if ((key >= 32) && (key <= 125)) {   // Check if the character is a standard printable ASCII symbol
            int currentLen = (int)strlen(buffer); // Get the current length of the text buffer
            if (currentLen < maxLen - 1) {   // Ensure the text length does not exceed maximum allowed size
                buffer[currentLen] = (char)key; // Append the typed character to the end of the buffer
                buffer[currentLen + 1] = '\0'; // Add a null-terminator string ending after the character
            }
        }
        key = GetCharPressed();              // Fetch the next character in the input queue
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {        // Check if the user pressed the Backspace key
        int currentLen = (int)strlen(buffer); // Get current text buffer length
        if (currentLen > 0) {                // Verify there is text to delete
            buffer[currentLen - 1] = '\0';   // Remove the last character by placing a null-terminator earlier
        }
    }
}

// Main program entry point where the window loop and screen state logic are executed
int main() {
    const int screenWidth = 1200;            // Set window display width to 1200 pixels
    const int screenHeight = 800;            // Set window display height to 800 pixels
    InitWindow(screenWidth, screenHeight, "C / C++ Interactive Learning Studio"); // Initialize the Raylib application window with a title
    InitAppFont();                           // Load custom font textures for clean text rendering
    SetTargetFPS(60);                        // Lock the rendering frame rate to 60 frames per second

    AppState state;                          // Create the main application state container object
    state.registeredUsers = LoadUsers();     // Load all user accounts from the text file upon startup
    LanguageDatabase db = GetDatabase(state.language); // Load theory, quiz, and level databases for the default language

    Color kahootColors[4] = {                // Define 4 distinct bright colors for quiz answer buttons (Kahoot style)
        { 226, 27, 60, 255 },                // Red answer button color
        { 19, 104, 206, 255 },               // Blue answer button color
        { 216, 158, 0, 255 },                // Yellow/Gold answer button color
        { 38, 137, 12, 255 }                 // Green answer button color
    };

    while (!WindowShouldClose()) {           // Main game loop running until the user closes the window
        float dt = GetFrameTime();           // Get time elapsed since the last frame for smooth animations
        Vector2 mouse = GetMousePosition();  // Track current mouse cursor coordinates on screen

        BeginDrawing();                      // Start drawing graphics frame
        ClearBackground(Theme::BG_DARK);     // Fill the screen background with the dark theme color

        // ====================================================================
        // SCREEN 0A: AUTH MENU (Welcome / Login / Register choice)
        // ====================================================================
        if (state.currentScreen == ScreenState::AUTH_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 180, 42, Theme::TEXT_LIGHT); // Draw main title text centered near top
            DrawCenteredText("Master programming with interactive challenges & Duolingo-style XP", 240, 20, Theme::TEXT_MUTED); // Draw subtitle description

            float btnW = 360.0f;             // Set width for menu buttons
            float btnH = 60.0f;              // Set height for menu buttons
            float centerX = (screenWidth - btnW) / 2.0f; // Calculate horizontal center alignment coordinate

            if (DrawButton({ centerX, 360.0f, btnW, btnH }, "Login", Theme::BTN_PRIMARY, 24)) { // Draw Login button and check if clicked
                state.inputUsername[0] = '\0'; // Clear username input buffer
                state.inputPassword[0] = '\0'; // Clear password input buffer
                state.authErrorMsg = "";       // Clear any previous error messages
                state.activeInputBox = 0;      // Reset active text box focus
                state.currentScreen = ScreenState::LOGIN; // Switch screen state to Login screen
            }
            if (DrawButton({ centerX, 445.0f, btnW, btnH }, "Register New Account", Theme::SUCCESS_COL, 24)) { // Draw Register button and check if clicked
                state.inputUsername[0] = '\0'; // Clear username input buffer
                state.inputPassword[0] = '\0'; // Clear password input buffer
                state.authErrorMsg = "";       // Clear previous error messages
                state.activeInputBox = 0;      // Reset active text box focus
                state.currentScreen = ScreenState::REGISTER; // Switch screen state to Register screen
            }
        }

        // ====================================================================
        // SCREEN 0B: LOGIN SCREEN
        // ====================================================================
        else if (state.currentScreen == ScreenState::LOGIN) {
            DrawCenteredText("USER LOGIN", 140, 36, Theme::TEXT_LIGHT); // Draw login screen title centered

            Rectangle userBox = { 400, 260, 400, 45 }; // Define rectangle bounds for username text box
            Rectangle passBox = { 400, 360, 400, 45 }; // Define rectangle bounds for password text box

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check if the left mouse button was clicked this frame
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1; // Focus username box if clicked inside
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2; // Focus password box if clicked inside
                else state.activeInputBox = 0; // Remove focus if clicked outside text boxes
            }

            // Username input box
            DrawCustomText("Username:", 400, 230, 20, Theme::TEXT_MUTED); // Draw label above username box
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Highlight green if active, blue otherwise
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG); // Draw background box for username
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder); // Draw border lines for username box
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT); // Draw currently typed username text
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32); // Accept keyboard input if username box is active

            // Password input box
            DrawCustomText("Password:", 400, 330, 20, Theme::TEXT_MUTED); // Draw label above password box
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Highlight green if active, blue otherwise
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG); // Draw background box for password
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder); // Draw border lines for password box
            std::string maskedPass(strlen(state.inputPassword), '*'); // Create masked password string using asterisks
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT); // Draw masked password characters on screen
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32); // Accept keyboard input if password box is active

            if (!state.authErrorMsg.empty()) { // Check if there is an authentication error message to display
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL); // Draw error message in red text
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Sign In", Theme::SUCCESS_COL, 20)) { // Draw Sign In button and check if clicked
                bool found = false;                    // Flag to track whether account credentials match
                for (size_t i = 0; i < state.registeredUsers.size(); ++i) { // Loop through registered users database
                    if (state.registeredUsers[i].username == state.inputUsername && // Check if username matches
                        state.registeredUsers[i].password == state.inputPassword) { // Check if password matches
                        state.currentUserIndex = (int)i; // Set current logged-in user index
                        found = true;                    // Mark credentials as found
                        break;                           // Exit search loop
                    }
                }
                if (found) {                             // If user account was successfully verified
                    state.currentScreen = ScreenState::MAIN_MENU; // Transition to main menu screen
                }
                else {                                   // If credentials were incorrect
                    state.authErrorMsg = "Invalid username or password!"; // Display error message
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check if clicked
                state.currentScreen = ScreenState::AUTH_MENU; // Return to welcome auth menu screen
            }
        }

        // ====================================================================
        // SCREEN 0C: REGISTER SCREEN
        // ====================================================================
        else if (state.currentScreen == ScreenState::REGISTER) {
            DrawCenteredText("CREATE ACCOUNT", 140, 36, Theme::TEXT_LIGHT); // Draw register screen title centered

            Rectangle userBox = { 400, 260, 400, 45 }; // Define rectangle for registration username box
            Rectangle passBox = { 400, 360, 400, 45 }; // Define rectangle for registration password box

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check if left mouse button was clicked
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1; // Focus username box
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2; // Focus password box
                else state.activeInputBox = 0; // Clear focus if clicked outside
            }

            DrawCustomText("Choose Username:", 400, 230, 20, Theme::TEXT_MUTED); // Draw username label
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Highlight border
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG); // Draw username box background
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder); // Draw username box border lines
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT); // Draw typed username
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32); // Input text into username buffer

            DrawCustomText("Choose Password:", 400, 330, 20, Theme::TEXT_MUTED); // Draw password label
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Highlight border
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG); // Draw password box background
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder); // Draw password box border lines
            std::string maskedPass(strlen(state.inputPassword), '*'); // Mask password input with asterisks
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT); // Draw masked password
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32); // Input text into password buffer

            if (!state.authErrorMsg.empty()) { // Check for registration error message
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL); // Draw error text in red
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Register", Theme::SUCCESS_COL, 20)) { // Draw Register button and check click
                std::string u(state.inputUsername); // Store entered username as string
                std::string p(state.inputPassword); // Store entered password as string
                if (u.empty() || p.empty()) {       // Check if either field is left blank
                    state.authErrorMsg = "Fields cannot be empty!"; // Show error for empty fields
                }
                else {
                    bool exists = false;            // Flag to check if username is already taken
                    for (auto& user : state.registeredUsers) { // Loop through registered users
                        if (user.username == u) exists = true; // Mark as existing if username matches
                    }
                    if (exists) {                   // If username is already taken
                        state.authErrorMsg = "Username already taken!"; // Show error message
                    }
                    else {
                        User newUser{ u, p, 0, 0 }; // Create new user object with 0 XP and 0 completed levels
                        state.registeredUsers.push_back(newUser); // Add new user to the registered list
                        state.currentUserIndex = (int)state.registeredUsers.size() - 1; // Log in as the new user
                        SaveUsers(state.registeredUsers); // Save updated user list into users.txt file
                        state.currentScreen = ScreenState::MAIN_MENU; // Transition to main menu screen
                    }
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::AUTH_MENU; // Return to welcome auth menu screen
            }
        }

        else {
            const char* activeLangText = (state.language == TargetLang::LANG_C) ? "Active Lang: C" : "Active Lang: C++"; // Determine active language text label
            DrawRectangleRounded({ 20, 20, 210, 36 }, 0.3f, 6, Theme::PANEL_BG); // Draw background panel badge for language indicator
            DrawCustomText(activeLangText, 35, 29, 18, Theme::WARNING_COL); // Draw active programming language badge text

            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) { // Check if a user is currently logged in
                User& currUser = state.registeredUsers[state.currentUserIndex]; // Reference current logged-in user data
                std::string userBadge = TextFormat("User: %s | %d XP", currUser.username.c_str(), currUser.xp); // Format username and XP text badge
                DrawRectangleRounded({ 245, 20, 260, 36 }, 0.3f, 6, Theme::PANEL_BG); // Draw background panel for user stats badge
                DrawCustomText(userBadge.c_str(), 260, 29, 16, Theme::SUCCESS_COL); // Draw user name and XP counter on screen
            }
        }

        // ====================================================================
        // SCREEN 1: MAIN MENU
        // ====================================================================
        if (state.currentScreen == ScreenState::MAIN_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 100, 42, Theme::TEXT_LIGHT); // Draw main menu title centered
            DrawCenteredText("Interactive Theory, Quiz, Code Constructor & Visualizer", 155, 20, Theme::TEXT_MUTED); // Draw main menu subtitle

            if (DrawButton({ screenWidth - 340.0f, 20.0f, 150.0f, 42.0f }, "Leaderboard", Theme::WARNING_COL, 18)) { // Draw Leaderboard button in top right
                state.currentScreen = ScreenState::LEADERBOARD; // Switch to leaderboard ranking screen
            }
            if (DrawButton({ screenWidth - 170.0f, 20.0f, 150.0f, 42.0f }, "Settings", Theme::BTN_SECONDARY, 20)) { // Draw Settings button in top right
                state.currentScreen = ScreenState::SETTINGS; // Switch to settings screen
            }

            if (DrawButton({ 20, 70, 150, 36 }, "Log Out", Theme::ERROR_COL, 16)) { // Draw Log Out button in top left
                state.currentUserIndex = -1;             // Clear current user login session
                state.currentScreen = ScreenState::AUTH_MENU; // Return to welcome auth menu screen
            }

            float btnW = 360.0f;                     // Set menu option button width
            float btnH = 60.0f;                      // Set menu option button height
            float centerX = (screenWidth - btnW) / 2.0f; // Calculate horizontal center alignment position
            float startY = 240.0f;                   // Set starting vertical coordinate for menu buttons
            float gap = 75.0f;                       // Set vertical distance gap between menu buttons

            if (DrawButton({ centerX, startY, btnW, btnH }, "Theory", Theme::BTN_PRIMARY, 24)) { // Draw Theory button and check click
                state.currentScreen = ScreenState::THEORY_TOPICS; // Switch to theory topics list screen
            }
            if (DrawButton({ centerX, startY + gap, btnW, btnH }, "Quiz", Theme::BTN_PRIMARY, 24)) { // Draw Quiz button and check click
                state.currentQuestion = 0;           // Reset quiz question counter to the beginning
                state.score = 0;                     // Reset quiz score counter to zero
                state.quizFinished = false;          // Reset quiz finished completion flag
                state.selectedAnswer = -1;           // Clear selected answer choice index
                state.feedbackTimer = 0.0f;          // Reset answer feedback timer countdown
                state.currentScreen = ScreenState::QUIZ; // Switch to quiz screen
            }
            if (DrawButton({ centerX, startY + gap * 2, btnW, btnH }, "Constructor", Theme::BTN_PRIMARY, 24)) { // Draw Constructor button and check click
                state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS; // Switch to constructor puzzle levels list
            }
            if (DrawButton({ centerX, startY + gap * 3, btnW, btnH }, "Visualizer", Theme::BTN_PRIMARY, 24)) { // Draw Visualizer button and check click
                state.currentScreen = ScreenState::VISUALIZER_LEVELS; // Switch to visualizer code lessons list
            }
        }

        // ====================================================================
        // SCREEN 1.5: LEADERBOARD (Duolingo Style Ranking)
        // ====================================================================
        else if (state.currentScreen == ScreenState::LEADERBOARD) {
            DrawCenteredText("DIAMOND LEAGUE LEADERBOARD", 80, 36, Theme::WARNING_COL); // Draw leaderboard title centered
            DrawCenteredText("Top coders ranked by earned XP. Complete quizzes & constructor to climb!", 125, 18, Theme::TEXT_MUTED); // Draw leaderboard description

            std::vector<User> sortedUsers = state.registeredUsers; // Copy registered users list to sort them by XP
            std::sort(sortedUsers.begin(), sortedUsers.end(), [](const User& a, const User& b) {
                return a.xp > b.xp;                            // Sort users in descending order based on total XP points
                });

            float startY = 175.0f;                   // Define starting vertical position for ranking rows
            for (size_t i = 0; i < sortedUsers.size(); ++i) { // Loop through all sorted users in ranking
                Rectangle rowRect = { 300.0f, startY + (float)(i * 65.0f), 600.0f, 52.0f }; // Set dimensions for ranking table row

                Color rowCol = Theme::PANEL_BG;      // Set default panel background color for row
                if (state.currentUserIndex >= 0 && sortedUsers[i].username == state.registeredUsers[state.currentUserIndex].username) {
                    rowCol = { 40, 90, 70, 255 };    // Highlight row with greenish tint if it belongs to currently logged-in user
                }

                DrawRectangleRounded(rowRect, 0.2f, 6, rowCol); // Draw rounded rectangle background for the leaderboard row
                DrawRectangleRoundedLines(rowRect, 0.2f, 6, Theme::BTN_PRIMARY); // Draw border line around the row box

                std::string rankStr = TextFormat("#%d", (int)i + 1); // Format rank number string (e.g., #1, #2)
                Color rankCol = Theme::TEXT_LIGHT;   // Set default rank text color
                if (i == 0) rankCol = { 255, 215, 0, 255 };   // Gold color for 1st place rank
                else if (i == 1) rankCol = { 192, 192, 192, 255 }; // Silver color for 2nd place rank
                else if (i == 2) rankCol = { 205, 127, 50, 255 };  // Bronze color for 3rd place rank

                DrawCustomText(rankStr.c_str(), (int)rowRect.x + 25, (int)rowRect.y + 15, 22, rankCol); // Draw rank position number
                DrawCustomText(sortedUsers[i].username.c_str(), (int)rowRect.x + 90, (int)rowRect.y + 15, 20, Theme::TEXT_LIGHT); // Draw user's name

                std::string xpStr = TextFormat("%d XP", sortedUsers[i].xp); // Format user's total XP string
                int xpWidth = (int)MeasureCustomText(xpStr.c_str(), 20);    // Measure width of XP text for right-alignment
                DrawCustomText(xpStr.c_str(), (int)(rowRect.x + rowRect.width - xpWidth - 25), (int)rowRect.y + 15, 20, Theme::WARNING_COL); // Draw XP score right-aligned
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 2: SETTINGS
        // ====================================================================
        else if (state.currentScreen == ScreenState::SETTINGS) {
            DrawCenteredText("SETTINGS", 120, 38, Theme::TEXT_LIGHT); // Draw settings title centered
            DrawCenteredText("Choose your target programming language:", 190, 22, Theme::TEXT_MUTED); // Draw prompt text

            Color cBtnColor = (state.language == TargetLang::LANG_C) ? Theme::SUCCESS_COL : Theme::BTN_SECONDARY;   // Green if C is selected, grey otherwise
            Color cppBtnColor = (state.language == TargetLang::LANG_CPP) ? Theme::SUCCESS_COL : Theme::BTN_SECONDARY; // Green if C++ is selected, grey otherwise

            if (DrawButton({ 360, 270, 220, 80 }, "C Language", cBtnColor, 24)) { // Draw C Language selection button and check click
                state.language = TargetLang::LANG_C; // Switch active language setting to C
                db = GetDatabase(state.language);    // Reload database content for C language
            }
            if (DrawButton({ 620, 270, 220, 80 }, "C++ Language", cppBtnColor, 24)) { // Draw C++ Language selection button and check click
                state.language = TargetLang::LANG_CPP; // Switch active language setting to C++
                db = GetDatabase(state.language);    // Reload database content for C++ language
            }

            DrawCenteredText("All theory topics, quizzes, constructor puzzles, and visualizer code", 410, 18, Theme::TEXT_MUTED); // Draw explanation line 1
            DrawCenteredText("automatically adapt to your selected language.", 435, 18, Theme::TEXT_MUTED); // Draw explanation line 2

            if (DrawButton({ 480, 690, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 3: THEORY TOPIC SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::THEORY_TOPICS) {
            DrawCenteredText("SELECT A THEORY TOPIC", 90, 34, Theme::TEXT_LIGHT); // Draw theory selection title centered

            for (size_t i = 0; i < db.theoryTopics.size(); ++i) { // Loop through all available theory topics in database
                Rectangle btnRect = { 350.0f, 190.0f + i * 85.0f, 500.0f, 65.0f }; // Set button dimensions for each topic row
                if (DrawButton(btnRect, db.theoryTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 24)) { // Draw topic button and check click
                    state.selectedTopic = (int)i; // Save selected topic index
                    state.currentCard = 0;        // Reset flashcard index to the first card
                    state.currentScreen = ScreenState::THEORY_CARDS; // Switch to flashcards viewing screen
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 4: THEORY FLASHCARDS
        // ====================================================================
        else if (state.currentScreen == ScreenState::THEORY_CARDS) {
            const auto& topic = db.theoryTopics[state.selectedTopic]; // Reference currently selected theory topic
            const auto& card = topic.cards[state.currentCard];        // Reference currently active flashcard

            DrawCenteredText(topic.topicName.c_str(), 70, 28, Theme::TEXT_MUTED); // Draw topic category name at top

            Rectangle cardRect = { 220, 130, 760, 420 }; // Define rectangle bounds for flashcard container card
            DrawRectangleRounded(cardRect, 0.06f, 10, Theme::PANEL_BG); // Draw rounded card background panel
            DrawRectangleRoundedLines(cardRect, 0.06f, 10, Theme::BTN_PRIMARY); // Draw card border outline

            std::string progress = TextFormat("Card %d of %d", state.currentCard + 1, (int)topic.cards.size()); // Format card progress text (e.g., Card 1 of 4)
            DrawCustomText(progress.c_str(), 250, 155, 18, Theme::WARNING_COL); // Draw card progress indicator
            DrawCenteredText(card.title.c_str(), 195, 32, Theme::TEXT_LIGHT); // Draw flashcard title centered on card
            DrawLine(260, 245, 940, 245, Theme::BTN_SECONDARY); // Draw dividing horizontal line below title

            DrawTextWrapped(card.content, { 260, 275, 680, 240 }, 24, Theme::TEXT_LIGHT); // Draw detailed flashcard text content with auto-wrapping

            if (state.currentCard > 0) { // Check if not on the first card
                if (DrawButton({ 220, 580, 180, 50 }, "< Previous", Theme::BTN_PRIMARY, 20)) { // Draw Previous button and check click
                    state.currentCard--; // Move to previous flashcard index
                }
            }
            if (state.currentCard < (int)topic.cards.size() - 1) { // Check if there are more cards remaining
                if (DrawButton({ 800, 580, 180, 50 }, "Next >", Theme::BTN_PRIMARY, 20)) { // Draw Next button and check click
                    state.currentCard++; // Move to next flashcard index
                }
            }
            else { // If on the final card of the topic
                if (DrawButton({ 800, 580, 180, 50 }, "Finish Topic", Theme::SUCCESS_COL, 20)) { // Draw Finish button and check click
                    if (state.currentUserIndex >= 0) { // Check if user is logged in
                        state.registeredUsers[state.currentUserIndex].xp += 20; // Reward user with 20 XP for finishing theory
                        SaveUsers(state.registeredUsers); // Save updated XP progress into users.txt file
                    }
                    state.currentScreen = ScreenState::THEORY_TOPICS; // Return to theory topics list screen
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 5: QUIZ (KAHOOT STYLE)
        // ====================================================================
        else if (state.currentScreen == ScreenState::QUIZ) {
            if (!state.quizFinished) { // Check if quiz is still ongoing
                const auto& q = db.quizQuestions[state.currentQuestion]; // Reference active quiz question

                std::string header = TextFormat("QUESTION %d OF %d", state.currentQuestion + 1, (int)db.quizQuestions.size()); // Format question progress header text
                DrawCenteredText(header.c_str(), 75, 24, Theme::WARNING_COL); // Draw question counter header

                Rectangle qBox = { 150, 120, 900, 160 }; // Define rectangle bounds for question container box
                DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG); // Draw rounded background box for question text
                DrawTextWrapped(q.question, { 180, 165, 840, 100 }, 28, Theme::TEXT_LIGHT); // Draw question text wrapped inside box

                for (int i = 0; i < 4; ++i) { // Loop through all 4 answer options
                    int col = i % 2;           // Calculate option column index (0 or 1)
                    int row = i / 2;           // Calculate option row index (0 or 1 for 2x2 grid)
                    Rectangle optRect = { 150.0f + col * 460.0f, 320.0f + row * 145.0f, 440.0f, 125.0f }; // Set button rectangle for option

                    Color btnCol = kahootColors[i]; // Get default distinct Kahoot color for option button
                    if (state.selectedAnswer != -1) { // Check if an answer has already been clicked this question
                        if (i == q.correctIndex) btnCol = Theme::SUCCESS_COL; // Turn green if this option is the correct answer
                        else if (i == state.selectedAnswer) btnCol = Theme::ERROR_COL; // Turn red if user picked this incorrect option
                        else btnCol = Theme::BTN_SECONDARY; // Turn grey for other unselected options
                    }

                    if (DrawButton(optRect, q.options[i].c_str(), btnCol, 24) && state.selectedAnswer == -1) { // Draw option button and check if clicked while no answer chosen yet
                        state.selectedAnswer = i; // Save index of chosen answer
                        if (i == q.correctIndex) { // Check if chosen answer is correct
                            state.score++;         // Increase correct answer score counter
                            if (state.currentUserIndex >= 0) { // Check if user is logged in
                                state.registeredUsers[state.currentUserIndex].xp += 15; // Reward user with 15 XP for correct answer
                                SaveUsers(state.registeredUsers); // Save updated XP into users.txt file
                            }
                        }
                        state.feedbackTimer = 1.4f; // Set feedback pause countdown timer to 1.4 seconds
                    }
                }

                if (state.selectedAnswer != -1) { // Check if feedback pause is active after answering
                    state.feedbackTimer -= dt;    // Decrement feedback timer by elapsed frame time
                    const char* statusMsg = (state.selectedAnswer == q.correctIndex) ? "Correct!" : "Incorrect!"; // Determine feedback message text
                    Color statusCol = (state.selectedAnswer == q.correctIndex) ? Theme::SUCCESS_COL : Theme::ERROR_COL; // Green for correct, red for incorrect
                    DrawCenteredText(statusMsg, 630, 30, statusCol); // Draw status feedback message centered at bottom

                    if (state.feedbackTimer <= 0.0f) { // Check if feedback time has expired
                        state.selectedAnswer = -1;     // Reset selected answer for next question
                        state.currentQuestion++;       // Move to next question index
                        if (state.currentQuestion >= (int)db.quizQuestions.size()) { // Check if all questions are completed
                            state.quizFinished = true; // Mark quiz as finished
                        }
                    }
                }
            }
            else { // If quiz is completed
                DrawCenteredText("QUIZ COMPLETED!", 180, 44, Theme::SUCCESS_COL); // Draw completion title in green
                std::string scoreStr = TextFormat("Your Final Score: %d / %d", state.score, (int)db.quizQuestions.size()); // Format final score results text
                DrawCenteredText(scoreStr.c_str(), 260, 32, Theme::TEXT_LIGHT); // Draw final score centered

                if (DrawButton({ 480, 380, 240, 55 }, "Retry Quiz", Theme::BTN_PRIMARY, 22)) { // Draw Retry button and check click
                    state.currentQuestion = 0; // Reset question counter back to start
                    state.score = 0;           // Reset score counter to zero
                    state.quizFinished = false; // Reset quiz finished flag
                    state.selectedAnswer = -1; // Reset selected answer index
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 6: CONSTRUCTOR LEVEL SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::CONSTRUCTOR_LEVELS) {
            DrawCenteredText("CODE CONSTRUCTOR - SELECT LEVEL", 90, 34, Theme::TEXT_LIGHT); // Draw constructor levels title centered
            DrawCenteredText("Drag and drop code blocks into the right order and watch the car inspect your code!", 140, 20, Theme::TEXT_MUTED); // Draw instruction subtitle

            for (size_t i = 0; i < db.constructorLevels.size(); ++i) { // Loop through available constructor levels in database
                Rectangle btnRect = { 350.0f, 220.0f + i * 85.0f, 500.0f, 65.0f }; // Set button rectangle bounds for each level row
                if (DrawButton(btnRect, db.constructorLevels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 24)) { // Draw level button and check click
                    state.selectedConstructorLevel = (int)i; // Save chosen constructor level index
                    InitConstructorLevel(state, db.constructorLevels[i]); // Initialize puzzle slots and shuffle blocks for level
                    state.currentScreen = ScreenState::CONSTRUCTOR_GAME; // Switch to drag-and-drop constructor game screen
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 7: CONSTRUCTOR GAME
        // ====================================================================
        else if (state.currentScreen == ScreenState::CONSTRUCTOR_GAME) {
            const auto& level = db.constructorLevels[state.selectedConstructorLevel]; // Reference current constructor level data
            int numSlots = (int)level.correctLines.size(); // Count total code slots needed for this level

            DrawCenteredText(level.levelTitle.c_str(), 25, 28, Theme::TEXT_LIGHT); // Draw level title centered at top
            DrawCenteredText("Drag code blocks from the bottom pool into the empty slots:", 60, 18, Theme::TEXT_MUTED); // Draw gameplay instruction subtitle

            std::vector<Rectangle> slotRects(numSlots); // Create list of target slot rectangles
            for (int i = 0; i < numSlots; ++i) {        // Loop through each slot index
                slotRects[i] = { 240.0f, 135.0f + i * 48.0f, 720.0f, 40.0f }; // Set rectangle position and size for target slot
            }

            DrawLine(200, 122, 1000, 122, Theme::BTN_SECONDARY); // Draw horizontal track line for inspection car
            DrawCar(state.carX, 82, state.carCrashed);           // Draw the animated inspection car above the slots

            if (state.isCarMoving && !state.carCrashed && !state.levelSuccess) { // Check if car is currently driving across code slots
                state.carX += 260.0f * dt;                       // Move car horizontally across screen based on delta time
                float stepWidth = 720.0f / (float)numSlots;      // Calculate pixel width per slot on the track
                int currentInspectSlot = (int)((state.carX - 200.0f) / stepWidth); // Determine which slot the car is currently inspecting

                if (currentInspectSlot >= 0 && currentInspectSlot < numSlots) { // Check if car is inside valid slot range
                    int bIdx = state.slotBlockIndex[currentInspectSlot];        // Get index of block placed in this slot (-1 if empty)
                    bool slotCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[currentInspectSlot]); // Verify if placed block matches correct code line

                    if (!slotCorrect) { // If code block is wrong or slot is empty
                        state.carCrashed = true; // Trigger car crash state
                        state.isCarMoving = false; // Stop car movement
                        state.firstErrorLine = currentInspectSlot; // Record index where first error occurred
                        state.showErrorModal = true; // Show detailed error report popup window
                    }
                    else { // If code block is correct
                        state.carTargetLine = currentInspectSlot + 1; // Advance successfully inspected line count
                    }
                }
                else if (currentInspectSlot >= numSlots) { // If car successfully passed the final slot
                    state.levelSuccess = true;             // Mark level as successfully completed
                    state.isCarMoving = false;             // Stop car movement
                    state.carTargetLine = numSlots;        // Set target line count to maximum
                    if (state.currentUserIndex >= 0) {     // Check if user is logged in
                        state.registeredUsers[state.currentUserIndex].xp += 50; // Reward user with 50 XP for solving constructor level
                        SaveUsers(state.registeredUsers);  // Save updated XP progress into users.txt file
                    }
                }
            }

            for (int i = 0; i < numSlots; ++i) { // Loop through all target slots to draw them
                Color slotCol = Theme::PANEL_BG; // Default background color for empty slot
                if (state.carTargetLine > i) {
                    slotCol = Theme::SUCCESS_COL; // Turn green if line was successfully inspected by car
                }
                else if (state.carCrashed) {
                    int bIdx = state.slotBlockIndex[i]; // Get block index in slot
                    bool isCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[i]); // Check if correct
                    if (!isCorrect) slotCol = Theme::ERROR_COL; // Turn red if car crashed and this slot has a mistake
                }

                DrawRectangleRounded(slotRects[i], 0.2f, 6, slotCol); // Draw rounded rectangle background for slot
                DrawRectangleRoundedLines(slotRects[i], 0.2f, 6, Theme::BTN_SECONDARY); // Draw slot border lines
                DrawCustomText(TextFormat("%d.", i + 1), (int)slotRects[i].x - 35, (int)slotRects[i].y + 10, 20, Theme::TEXT_MUTED); // Draw line number label next to slot
            }

            DrawLine(100, 465, 1100, 465, Theme::BTN_SECONDARY); // Draw dividing line separating slots from bottom pool
            DrawCustomText("Available Code Blocks:", 140, 442, 18, Theme::TEXT_MUTED); // Draw title label for available blocks pool

            bool interactionLocked = state.isCarMoving || state.showErrorModal || state.levelSuccess; // Lock dragging if car is moving or modal is open
            if (!interactionLocked) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check if left mouse button was pressed
                    for (int i = (int)state.blocks.size() - 1; i >= 0; --i) { // Loop backwards through blocks to pick top-most one first
                        if (CheckCollisionPointRec(mouse, state.blocks[i].rect)) { // Check if mouse clicked inside block rectangle
                            state.blocks[i].isDragging = true; // Mark this block as being dragged
                            state.blocks[i].dragOffset = { mouse.x - state.blocks[i].rect.x, mouse.y - state.blocks[i].rect.y }; // Store click offset inside block
                            if (state.blocks[i].currentSlot != -1) { // If block was previously snapped inside a slot
                                state.slotBlockIndex[state.blocks[i].currentSlot] = -1; // Free up that slot
                                state.blocks[i].currentSlot = -1; // Remove block slot assignment
                            }
                            break; // Stop searching after picking one block
                        }
                    }
                }

                for (auto& b : state.blocks) { // Loop through all blocks to update dragging position
                    if (b.isDragging) {       // If this block is currently being dragged by mouse
                        b.rect.x = mouse.x - b.dragOffset.x; // Update block horizontal coordinate following mouse
                        b.rect.y = mouse.y - b.dragOffset.y; // Update block vertical coordinate following mouse

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { // Check if mouse button was released
                            b.isDragging = false; // Stop dragging state
                            for (int s = 0; s < numSlots; ++s) { // Loop through target slots to check for drop snapshot
                                if (CheckCollisionPointRec(mouse, slotRects[s]) && state.slotBlockIndex[s] == -1) { // If dropped over an empty slot
                                    b.currentSlot = s; // Assign block to this slot index
                                    b.rect.x = slotRects[s].x + 10; // Align block X inside slot
                                    b.rect.y = slotRects[s].y + 1;  // Align block Y inside slot
                                    state.slotBlockIndex[s] = (int)(&b - &state.blocks[0]); // Record block index inside slot tracker
                                    break; // Exit loop once snapped
                                }
                            }
                        }
                    }
                }
            }

            for (size_t i = 0; i < state.blocks.size(); ++i) { // Loop through all blocks to draw them on screen
                const auto& b = state.blocks[i];              // Reference current block
                Color blockColor = Theme::BTN_PRIMARY;        // Set default primary blue color for block

                if (b.currentSlot != -1) {                    // If block is placed in a slot
                    if (state.carTargetLine > b.currentSlot) {
                        blockColor = Theme::SUCCESS_COL;      // Turn green if verified by inspection car
                    }
                    else if (state.carCrashed && b.text != level.correctLines[b.currentSlot]) {
                        blockColor = Theme::ERROR_COL;        // Turn red if car crashed and block is incorrect
                    }
                }

                DrawRectangleRounded(b.rect, 0.2f, 6, blockColor); // Draw rounded rectangle background for code block
                DrawRectangleRoundedLines(b.rect, 0.2f, 6, Theme::TEXT_LIGHT); // Draw border line around block
                DrawCustomText(b.text.c_str(), (int)b.rect.x + 14, (int)b.rect.y + 9, 20, Theme::TEXT_LIGHT); // Draw code text inside block
            }

            if (!state.isCarMoving && !state.levelSuccess && !state.showErrorModal) { // If puzzle is ready to be tested
                if (DrawButton({ 740, 720, 220, 50 }, "Ready (Run Car)", Theme::SUCCESS_COL, 20)) { // Draw Run Car button and check click
                    state.carX = 200.0f;       // Reset car starting position
                    state.carTargetLine = 0;   // Reset verified line counter
                    state.carCrashed = false;  // Reset crash flag
                    state.isCarMoving = true;  // Start car movement animation
                }
            }

            if (DrawButton({ 240, 720, 220, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }

            if (state.levelSuccess) { // If level was successfully completed
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 185 }); // Draw dark translucent overlay background
                Rectangle modal = { 280, 220, 640, 320 }; // Define rectangle bounds for success popup window modal
                DrawRectangleRounded(modal, 0.08f, 10, Theme::PANEL_BG); // Draw rounded modal background panel
                DrawRectangleRoundedLines(modal, 0.08f, 10, Theme::SUCCESS_COL); // Draw green modal border outline

                DrawCenteredText("CONGRATULATIONS!", 265, 36, Theme::SUCCESS_COL); // Draw success title in green text
                DrawCenteredText("The car passed every line! +50 XP earned.", 330, 22, Theme::TEXT_LIGHT); // Draw reward message text centered

                if (DrawButton({ 340, 440, 230, 50 }, "Play Again", Theme::BTN_PRIMARY, 20)) { // Draw Play Again button and check click
                    InitConstructorLevel(state, level); // Restart constructor level puzzle
                }
                if (DrawButton({ 630, 440, 230, 50 }, "Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Main Menu button and check click
                    state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
                }
            }

            if (state.showErrorModal) { // If car crashed due to code errors
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 195 }); // Draw dark translucent overlay background
                Rectangle modal = { 140, 90, 920, 620 }; // Define rectangle bounds for error report popup window modal
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG); // Draw rounded error modal background panel
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::ERROR_COL); // Draw red error modal border outline

                DrawCenteredText("CAR CRASHED! CODE ERRORS DETECTED", 115, 30, Theme::ERROR_COL); // Draw crash title in red text
                std::string crashInfo = TextFormat("First breakdown occurred at Line %d. Review your mistakes below:", state.firstErrorLine + 1); // Format error breakdown summary text
                DrawCenteredText(crashInfo.c_str(), 155, 20, Theme::WARNING_COL); // Draw error summary subtitle in orange

                std::string errorReport = ""; // Initialize empty string for detailed error report log
                for (int s = 0; s < numSlots; ++s) { // Loop through all slots to find mistakes
                    int bIdx = state.slotBlockIndex[s]; // Get block index in slot
                    std::string userCode = (bIdx == -1) ? "[Empty Slot]" : state.blocks[bIdx].text; // Get user code or mark empty
                    if (userCode != level.correctLines[s]) { // If user code does not match correct line
                        errorReport += TextFormat("Line %d:\n  Your block: %s\n  Correct block: %s\n  Why: %s\n\n",
                            s + 1, userCode.c_str(), level.correctLines[s].c_str(), level.slotExplanations[s].c_str()); // Append mistake explanation to report log
                    }
                }

                DrawTextWrapped(errorReport, { 180, 195, 840, 410 }, 18, Theme::TEXT_LIGHT); // Draw detailed error log text inside modal with auto-wrapping

                if (DrawButton({ 330, 635, 240, 50 }, "Retry Level", Theme::BTN_PRIMARY, 20)) { // Draw Retry button and check click
                    InitConstructorLevel(state, level); // Reset and restart constructor level puzzle
                }
                if (DrawButton({ 630, 635, 240, 50 }, "Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Main Menu button and check click
                    state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
                }
            }
        }

        // ====================================================================
        // SCREEN 8: VISUALIZER LEVEL SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::VISUALIZER_LEVELS) {
            DrawCenteredText("CODE VISUALIZER - SELECT LEVEL", 90, 34, Theme::TEXT_LIGHT); // Draw visualizer selection title centered
            DrawCenteredText("Inspect code execution line-by-line with interactive explanations", 140, 20, Theme::TEXT_MUTED); // Draw subtitle instructions

            for (size_t i = 0; i < db.visualizerLevels.size(); ++i) { // Loop through available visualizer lessons in database
                Rectangle btnRect = { 350.0f, 220.0f + i * 85.0f, 500.0f, 65.0f }; // Set button rectangle bounds for lesson row
                if (DrawButton(btnRect, db.visualizerLevels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 24)) { // Draw lesson button and check click
                    state.selectedVisualizerLevel = (int)i; // Save chosen visualizer lesson index
                    InitVisualizerLevel(state, db.visualizerLevels[i]); // Initialize walkthrough states and modals for lesson
                    state.currentScreen = ScreenState::VISUALIZER_VIEW; // Switch to step-by-step visualizer view screen
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }
        }

        // ====================================================================
        // SCREEN 9: VISUALIZER STEP-BY-STEP VIEW
        // ====================================================================
        else if (state.currentScreen == ScreenState::VISUALIZER_VIEW) {
            const auto& level = db.visualizerLevels[state.selectedVisualizerLevel]; // Reference current visualizer lesson data
            DrawCenteredText(level.levelTitle.c_str(), 30, 30, Theme::TEXT_LIGHT); // Draw lesson title centered at top

            float curY = 85.0f; // Set starting vertical Y position for code lines display
            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) { // Loop through all currently unlocked code steps
                Rectangle lineBox = { 140.0f, curY, 720.0f, 38.0f }; // Set rectangle box dimensions for code line row
                DrawRectangleRounded(lineBox, 0.2f, 6, Theme::PANEL_BG); // Draw rounded background box for code line
                DrawRectangleRoundedLines(lineBox, 0.2f, 6, Theme::BTN_PRIMARY); // Draw border outline around code box
                DrawCustomText(level.steps[i].codeLine.c_str(), 160, (int)curY + 9, 20, Theme::TEXT_LIGHT); // Draw actual line of code text

                if (state.stepUnderstood[i]) { // Check if this code step has already been marked as understood
                    const char* toggleLabel = state.explanationVisible[i] ? "Hide Info" : "Show Info"; // Determine toggle button label text
                    if (DrawButton({ 880.0f, curY, 160.0f, 38.0f }, toggleLabel, Theme::BTN_SECONDARY, 18)) { // Draw toggle button and check click
                        state.explanationVisible[i] = !state.explanationVisible[i]; // Flip explanation visibility flag on click
                    }
                }

                curY += 44.0f; // Move vertical cursor down for next element

                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) { // If explanation is hidden and step not yet understood
                    if (DrawButton({ 160.0f, curY, 190.0f, 36.0f }, "Explanation", Theme::WARNING_COL, 18)) { // Draw Explanation button and check click
                        state.explanationVisible[i] = true; // Open explanation panel for this line
                    }
                    curY += 44.0f; // Move vertical cursor down
                }

                if (state.explanationVisible[i]) { // If explanation panel is currently open
                    Rectangle expBox = { 160.0f, curY, 700.0f, 52.0f }; // Set dimensions for explanation text box
                    DrawRectangleRounded(expBox, 0.2f, 6, { 30, 58, 50, 255 }); // Draw dark greenish rounded background box for explanation
                    DrawRectangleRoundedLines(expBox, 0.2f, 6, Theme::SUCCESS_COL); // Draw green border outline around explanation box
                    DrawTextWrapped(level.steps[i].explanation, { 175.0f, curY + 8.0f, 670.0f, 40.0f }, 18, Theme::TEXT_LIGHT); // Draw explanation text wrapped inside box
                    curY += 58.0f; // Move vertical cursor down

                    if (!state.stepUnderstood[i]) { // If step has not been marked understood yet
                        if (DrawButton({ 160.0f, curY, 160.0f, 36.0f }, "Got It!", Theme::SUCCESS_COL, 18)) { // Draw "Got It!" completion button and check click
                            state.stepUnderstood[i] = true; // Mark step as understood
                            state.explanationVisible[i] = false; // Collapse explanation panel
                            if (state.revealedLinesCount < (int)level.steps.size()) { // Check if there are more hidden lines in lesson
                                state.revealedLinesCount++; // Unlock and reveal the next line of code
                            }
                            if (state.currentUserIndex >= 0) { // Check if user is logged in
                                state.registeredUsers[state.currentUserIndex].xp += 5; // Reward user with 5 XP for understanding a line
                                SaveUsers(state.registeredUsers); // Save updated XP progress into users.txt file
                            }
                        }
                        curY += 44.0f; // Move vertical cursor down
                    }
                }
            }

            if (state.stepUnderstood.back()) { // Check if the final step of the lesson has been understood
                DrawCenteredText("All lines analyzed! You can toggle any explanation above or return to the menu.", 675, 20, Theme::SUCCESS_COL); // Draw completion message centered at bottom
            }

            if (DrawButton({ 480, 720, 240, 50 }, "Exit to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Exit button and check click
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
            }

            if (state.showIntroModal) { // If initial full-code overview modal popup is active
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 205 }); // Draw dark translucent overlay background
                Rectangle modal = { 220, 110, 760, 560 }; // Define rectangle bounds for code overview modal popup window
                DrawRectangleRounded(modal, 0.06f, 10, Theme::PANEL_BG); // Draw rounded modal background panel
                DrawRectangleRoundedLines(modal, 0.06f, 10, Theme::BTN_PRIMARY); // Draw primary blue modal border outline

                DrawCenteredText("CODE OVERVIEW", 140, 32, Theme::WARNING_COL); // Draw modal title centered in orange text
                DrawCenteredText("Here is the complete code we are going to break down now:", 185, 20, Theme::TEXT_LIGHT); // Draw subtitle prompt text

                Rectangle codeBox = { 270, 225, 660, 330 }; // Define rectangle bounds for full-code display box container
                DrawRectangleRounded(codeBox, 0.05f, 8, Theme::BG_DARK); // Draw dark background box for full code preview
                DrawTextWrapped(level.fullCode, { 295, 245, 610, 290 }, 22, Theme::SUCCESS_COL); // Draw complete lesson code text inside box in green

                if (DrawButton({ 320, 585, 220, 50 }, "Cancel (Main Menu)", Theme::ERROR_COL, 20)) { // Draw Cancel button and check click
                    state.currentScreen = ScreenState::MAIN_MENU; // Return to main menu screen
                }
                if (DrawButton({ 660, 585, 220, 50 }, "Let's Go!", Theme::SUCCESS_COL, 20)) { // Draw "Let's Go!" start button and check click
                    state.showIntroModal = false; // Hide intro modal and start step-by-step lesson view
                }
            }
        }

        EndDrawing();                          // End drawing graphics frame and display it on screen
    }

    UnloadAppFont();                           // Unload custom font textures from system memory
    CloseWindow();                             // Close Raylib window and shut down graphics context
    return 0;                                  // Return exit code 0 indicating successful program termination
}