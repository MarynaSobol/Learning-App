// Purpose of main.cpp: Serves as the main entry point and core application loop for the C / C++ Interactive Learning Studio. It manages user authentication, file persistence, daily streak and lives tracking, text and code editor input, answer evaluation, and the rendering and interactive logic for all application screens including Theory, Quiz, Code Constructor, Code Visualizer, Practice Mode, and Exam Mode.

#include "raylib.h" // Include Raylib graphics and input library
#include "AppTypes.h" // Include custom application data structures and enums
#include "ContentData.h" // Include curriculum database declarations for C and C++
#include "UI.h" // Include custom UI drawing helper functions and theme colors
#include <algorithm> // Include standard algorithms such as sort, min, max, clamp, and shuffle
#include <random> // Include random number generator utilities
#include <cstring> // Include C-string manipulation functions like strlen
#include <fstream> // Include file stream classes for reading and writing user data
#include <sstream> // Include string stream for parsing delimited text lines
#include <ctime> // Include C time utilities for retrieving the current calendar date

std::string StripWhitespace(const std::string& s) { // Define helper function to remove all whitespace characters from a string
    std::string out; // Initialize empty output string for compact text
    for (char c : s) { // Iterate through each character in the input string
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') out.push_back(c); // Append character only if it is not space, tab, or newline
    }
    return out; // Return the whitespace-stripped string
}

std::string NormalizeAnswer(const std::string& s) { // Define helper function to trim whitespace and convert an exam answer to lowercase
    size_t start = s.find_first_not_of(" \t\r\n"); // Find index of the first non-whitespace character
    if (start == std::string::npos) return ""; // Return empty string if input contains only whitespace
    size_t end = s.find_last_not_of(" \t\r\n"); // Find index of the last non-whitespace character
    std::string trimmed = s.substr(start, end - start + 1); // Extract the trimmed substring
    for (char& c : trimmed) { // Iterate over each character in the trimmed string
        if (c >= 'A' && c <= 'Z') c = (char)(c - 'A' + 'a'); // Convert uppercase ASCII letters to lowercase
    }
    return trimmed; // Return the normalized lowercase string
}

void UpdateCodeEditorInput(std::string& buffer, float& backspaceTimer, float dt) { // Handle multi-line keyboard input for the Practice code editor
    int key = GetCharPressed(); // Retrieve the next typed Unicode character
    while (key > 0) { // Process all queued character presses for this frame
        if (key >= 32 && key <= 125 && buffer.size() < 2000) { // Accept printable ASCII characters up to 2000 characters total
            buffer.push_back((char)key); // Append typed character to the code buffer
        }
        key = GetCharPressed(); // Fetch the next character from the input queue
    }
    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && buffer.size() < 2000) { // Check if Enter or Numpad Enter was pressed within size limit
        buffer.push_back('\n'); // Insert a newline character into the code buffer
    }
    if (IsKeyPressed(KEY_TAB) && buffer.size() + 4 < 2000) { // Check if Tab was pressed with room for four spaces
        buffer += "    "; // Insert four spaces for code indentation
    }
    if (IsKeyPressed(KEY_BACKSPACE)) { // Handle initial Backspace press
        if (!buffer.empty()) buffer.pop_back(); // Delete the last character if buffer is not empty
        backspaceTimer = 0.35f; // Set initial hold delay before key repeat starts
    }
    else if (IsKeyDown(KEY_BACKSPACE)) { // Handle continuous deletion while Backspace is held down
        backspaceTimer -= dt; // Decrease repeat timer by frame delta time
        if (backspaceTimer <= 0.0f && !buffer.empty()) { // Delete another character when repeat interval elapses
            buffer.pop_back(); // Remove the last character from the buffer
            backspaceTimer = 0.045f; // Reset timer to fast repeat rate
        }
    }
}

void UpdateExamTextInput(std::string& buffer, float& backspaceTimer, float dt) { // Handle single-line keyboard input for Exam text answers
    int key = GetCharPressed(); // Retrieve the next typed character
    while (key > 0) { // Process all queued characters in the current frame
        if (key >= 32 && key <= 125 && buffer.size() < 80) { // Accept printable ASCII characters up to 80 characters
            buffer.push_back((char)key); // Append typed character to the answer buffer
        }
        key = GetCharPressed(); // Fetch the next queued character
    }
    if (IsKeyPressed(KEY_BACKSPACE)) { // Handle initial Backspace press
        if (!buffer.empty()) buffer.pop_back(); // Remove the trailing character if buffer is non-empty
        backspaceTimer = 0.35f; // Set initial delay before continuous deletion begins
    }
    else if (IsKeyDown(KEY_BACKSPACE)) { // Handle continuous Backspace while key remains held
        backspaceTimer -= dt; // Count down hold timer by delta time
        if (backspaceTimer <= 0.0f && !buffer.empty()) { // Trigger repeated deletion once interval expires
            buffer.pop_back(); // Remove the last character
            backspaceTimer = 0.045f; // Reset to rapid repeat interval
        }
    }
}

void DrawCodeWithCursor(const std::string& code, Rectangle bounds, float scrollY, int fontSize, bool showCursor) { // Render numbered code lines with a blinking cursor inside the Practice editor
    std::istringstream ss(code); // Create a stream to split the code buffer by newlines
    std::string line; // Hold each individual line of code
    float curY = bounds.y - scrollY; // Set starting vertical draw position shifted by scroll offset
    float lastLineWidth = 0.0f; // Track pixel width of the last rendered line for cursor placement
    float lastLineY = curY; // Track Y coordinate of the last rendered line
    int lineNum = 1; // Initialize 1-based line number counter

    if (code.empty()) { // Handle rendering when the code editor is completely empty
        if (showCursor && ((int)(GetTime() * 2.0) % 2 == 0)) { // Toggle cursor visibility every half second
            DrawCustomText("|", (int)bounds.x + 38, (int)curY, fontSize, Theme::SUCCESS_COL); // Draw blinking cursor at the start of line 1
        }
        return; // Exit early since there are no lines to draw
    }

    while (std::getline(ss, line, '\n')) { // Loop through each newline-delimited line of code
        DrawCustomText(TextFormat("%2d", lineNum), (int)bounds.x, (int)curY, fontSize - 2, Theme::TEXT_MUTED); // Render right-aligned line number in the gutter
        DrawCustomText(line.c_str(), (int)bounds.x + 38, (int)curY, fontSize, Theme::TEXT_LIGHT); // Render the line of source code next to the gutter
        lastLineWidth = MeasureCustomText(line.c_str(), fontSize); // Measure rendered line width to locate the cursor
        lastLineY = curY; // Record current line's Y position
        curY += fontSize + 6; // Advance vertical cursor to the next row
        lineNum++; // Increment line number counter
    }
    if (!code.empty() && code.back() == '\n') { // Handle trailing newline when user just pressed Enter
        DrawCustomText(TextFormat("%2d", lineNum), (int)bounds.x, (int)curY, fontSize - 2, Theme::TEXT_MUTED); // Draw line number for the new empty line
        lastLineWidth = 0.0f; // Place cursor at the start of the empty line
        lastLineY = curY; // Move cursor Y position to the new empty line
    }
    if (showCursor && ((int)(GetTime() * 2.0) % 2 == 0)) { // Render blinking cursor when editor is active
        DrawCustomText("|", (int)(bounds.x + 38 + lastLineWidth), (int)lastLineY, fontSize, Theme::SUCCESS_COL); // Draw green vertical bar at the end of the last line
    }
}

bool EvaluatePracticeCode(const std::string& userCode, const PracticeTask& task, std::string& outFeedback) { // Validate user-written Practice code against syntax checks and task rules
    std::string compact = StripWhitespace(userCode); // Produce whitespace-free copy of user code for token matching
    if (compact.empty()) { // Reject empty submissions immediately
        outFeedback = "Code editor is empty. Please write your solution before clicking Check."; // Set empty submission feedback message
        return false; // Report failure
    }

    std::vector<std::string> errors; // Collect all detected syntax and rule violations

    int braceBalance = 0, parenBalance = 0, quoteCount = 0; // Initialize counters for braces, parentheses, and double quotes
    for (size_t i = 0; i < userCode.size(); ++i) { // Scan every character in the user's code
        char c = userCode[i]; // Read current character
        if (c == '{') braceBalance++; // Increment brace counter on opening brace
        else if (c == '}') braceBalance--; // Decrement brace counter on closing brace
        else if (c == '(') parenBalance++; // Increment parenthesis counter on opening parenthesis
        else if (c == ')') parenBalance--; // Decrement parenthesis counter on closing parenthesis
        else if (c == '"' && (i == 0 || userCode[i - 1] != '\\')) quoteCount++; // Count unescaped double quotes
    }
    if (braceBalance != 0) errors.push_back("- Unmatched curly braces '{' and '}'. Check that every opened block is closed."); // Flag unbalanced curly braces
    if (parenBalance != 0) errors.push_back("- Unmatched parentheses '(' and ')'. Check your function calls and conditions."); // Flag unbalanced parentheses
    if (quoteCount % 2 != 0) errors.push_back("- Unclosed string literal. Make sure every '\"' has a matching closing '\"'."); // Flag odd number of string quotes
    if (compact.find(';') == std::string::npos) errors.push_back("- Missing semicolon ';'. Statements in C/C++ must end with a semicolon."); // Flag missing statement semicolon

    for (const auto& rule : task.rules) { // Check each task-specific validation rule
        std::string target = StripWhitespace(rule.requiredToken); // Normalize the required token by stripping whitespace
        if (compact.find(target) == std::string::npos) { // Check if the required token is absent from the user's code
            errors.push_back("- " + rule.errorFeedback); // Append the rule's diagnostic message to the error list
        }
    }

    if (errors.empty()) { // Succeed if no syntax or rule errors were found
        outFeedback = task.expectedOutput; // Populate feedback with the task's expected program output
        return true; // Return pass status
    }

    outFeedback = ""; // Initialize diagnostic feedback output string
    for (const auto& e : errors) { // Concatenate every error message on its own line
        outFeedback += e + "\n"; // Append error line with trailing newline
    }
    return false; // Return fail status
}

bool IsExamAnswerCorrect(const ExamQuestion& q, int chosenOpt, const std::string& typedText) { // Grade a single Exam question response
    if (q.type == ExamQuestionType::MULTIPLE_CHOICE) { // Check if the question uses multiple choice format
        return chosenOpt == q.correctOptionIndex; // Compare selected option index against the correct index
    }
    std::string normUser = NormalizeAnswer(typedText); // Normalize the user's typed text answer
    if (normUser.empty()) return false; // Treat blank text answers as incorrect
    for (const auto& acc : q.acceptedAnswers) { // Check against all acceptable answer variants
        if (normUser == NormalizeAnswer(acc)) return true; // Return true if normalized input matches any accepted answer
    }
    return false; // Return false if no accepted answer matched
}

std::string GetLetterGrade(int score, int total) { // Convert a numeric exam score into a descriptive letter grade
    if (total <= 0) return "N/A"; // Guard against division by zero
    float pct = (100.0f * score) / total; // Calculate percentage score
    if (pct >= 90.0f) return "A (Excellent)"; // Assign grade A for 90% and above
    if (pct >= 80.0f) return "B (Good)"; // Assign grade B for 80% to 89%
    if (pct >= 70.0f) return "C (Satisfactory)"; // Assign grade C for 70% to 79%
    if (pct >= 60.0f) return "D (Pass)"; // Assign grade D for 60% to 69%
    return "F (Needs Review)"; // Assign grade F below 60%
}

std::string GetTodayDate() { // Define helper function to return the current date formatted as YYYY-MM-DD
    std::time_t t = std::time(nullptr); // Get the current system time as a timestamp
    std::tm tm_now{}; // Initialize a calendar time structure to zero
#ifdef _WIN32 // Check if compiling on a Windows platform
    localtime_s(&tm_now, &t); // Safely convert timestamp to local time on Windows
#else // Handle non-Windows platforms such as Linux and macOS
    localtime_r(&t, &tm_now); // Safely convert timestamp to local time on POSIX systems
#endif // End platform-specific local time conversion
    char buf[16]; // Allocate a character buffer to store the formatted date string
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_now); // Format the date into YYYY-MM-DD inside the buffer
    return std::string(buf); // Convert the C-string buffer to std::string and return it
}

void SyncUserDailyStatus(User& user) { // Define function to update daily streak and reset daily lives on a new day
    std::string today = GetTodayDate(); // Fetch today's date string
    if (user.lastActiveDate != today) { // Check if the user has not logged in yet today
        user.lives = 5; // Reset the user's daily lives to 5
        if (!user.lastActiveDate.empty()) { // Check if the user has a previous active date recorded
            user.streak += 1; // Increment the user's daily learning streak
        }
        else { // Handle the case where no prior active date exists
            user.streak = 1; // Initialize the user's daily streak to 1
        }
        user.lastActiveDate = today; // Update the user's last active date to today
    }
    if (user.weakTopic.empty()) { // Check if the user does not have a weak topic assigned
        user.weakTopic = "Pointers & Syntax"; // Assign a default topic for practice
    }
}

std::vector<User> LoadUsers() { // Define function to load registered user profiles from disk
    std::vector<User> users; // Create a vector to store loaded user profiles
    std::ifstream file("users.txt"); // Open the users data file for reading
    std::string today = GetTodayDate(); // Fetch today's date for default values

    if (!file.is_open()) { // Check if the users file does not exist or failed to open
        return { // Return a default list of sample users
            {"Alice_Coder", "123", 450, 5, 7, 5, today, "STL & Vectors"}, // Default user Alice_Coder
            {"Bob_CPlusPlus", "123", 320, 3, 4, 4, today, "Memory Management"}, // Default user Bob_CPlusPlus
            {"Charlie_Dev", "123", 180, 2, 2, 5, today, "Control Flow"} // Default user Charlie_Dev
        };
    }

    std::string line; // Declare a string to hold each line read from the file
    while (std::getline(file, line)) { // Read the file line by line until the end
        if (line.empty()) continue; // Skip blank lines
        std::stringstream ss(line); // Wrap the current line in a stringstream for parsing
        std::string username, password, lastDate, weakTopic; // Declare string fields for user data
        int xp = 0, levels = 0, streak = 1, lives = 5; // Initialize numeric fields with defaults

        if (std::getline(ss, username, '|') && // Read the username up to the pipe delimiter
            std::getline(ss, password, '|') && // Read the password up to the pipe delimiter
            (ss >> xp) && (ss.ignore()) && // Read XP value and discard the following delimiter
            (ss >> levels)) { // Read completed levels count
            if (ss.peek() == '|') { // Check if a streak field follows
                ss.ignore(); // Discard the pipe delimiter
                ss >> streak; // Read the user's daily streak
                if (ss.peek() == '|') { // Check if a lives field follows
                    ss.ignore(); // Discard the pipe delimiter
                    ss >> lives; // Read the user's remaining lives
                    if (ss.peek() == '|') { // Check if date and weak topic fields follow
                        ss.ignore(); // Discard the pipe delimiter
                        std::getline(ss, lastDate, '|'); // Read the last active date up to the next pipe
                        std::getline(ss, weakTopic); // Read the remaining text as the weak topic
                    }
                }
            }
            if (lastDate.empty()) lastDate = today; // Fallback to today's date if missing
            if (weakTopic.empty()) weakTopic = "Pointers & Syntax"; // Fallback to default weak topic if missing
            users.push_back({ username, password, xp, levels, streak, lives, lastDate, weakTopic }); // Append the parsed user to the list
        }
    }
    file.close(); // Close the input file stream

    if (users.empty()) { // Check if no valid users were parsed from the file
        users = { // Populate the vector with default sample users
            {"Alice_Coder", "123", 450, 5, 7, 5, today, "STL & Vectors"}, // Default user Alice_Coder
            {"Bob_CPlusPlus", "123", 320, 3, 4, 4, today, "Memory Management"}, // Default user Bob_CPlusPlus
            {"Charlie_Dev", "123", 180, 2, 2, 5, today, "Control Flow"} // Default user Charlie_Dev
        };
    }
    return users; // Return the loaded vector of users
}

void SaveUsers(const std::vector<User>& users) { // Define function to save all user profiles to disk
    std::ofstream file("users.txt"); // Open or create the users data file for writing
    if (file.is_open()) { // Verify that the file opened successfully
        for (const auto& user : users) { // Iterate through each registered user
            file << user.username << "|" // Write username followed by delimiter
                << user.password << "|" // Write password followed by delimiter
                << user.xp << "|" // Write XP followed by delimiter
                << user.levelsCompleted << "|" // Write completed levels count followed by delimiter
                << user.streak << "|" // Write daily streak followed by delimiter
                << user.lives << "|" // Write remaining lives followed by delimiter
                << user.lastActiveDate << "|" // Write last active date followed by delimiter
                << user.weakTopic << "\n"; // Write weak topic and end the line
        }
        file.close(); // Close the output file stream
    }
}

void InitConstructorLevel(AppState& state, const ConstructorLevel& level) { // Define function to initialize a Code Constructor level
    state.blocks.clear(); // Remove any blocks from a previous level
    int numSlots = (int)level.correctLines.size(); // Determine how many target code slots are needed
    state.slotBlockIndex.assign(numSlots, -1); // Reset all slots to empty using -1

    std::vector<std::string> allTexts = level.correctLines; // Copy the required correct code lines
    for (const auto& extra : level.extraBlocks) { // Iterate over distractor code blocks
        allTexts.push_back(extra); // Add each distractor block to the pool of available blocks
    }

    std::mt19937 rng(1337 + state.selectedConstructorTopic * 31 + state.selectedConstructorLevel); // Seed a deterministic random generator per level
    std::shuffle(allTexts.begin(), allTexts.end(), rng); // Shuffle the code blocks into a randomized order

    int startX = 65; // Set starting X coordinate for the bottom block pool
    int startY = 480; // Set starting Y coordinate for the bottom block pool
    int colWidth = 520; // Set width of each draggable block column
    int colGap = 30; // Set horizontal spacing between the two block columns
    int rowHeight = 46; // Set vertical spacing between rows of blocks

    for (size_t i = 0; i < allTexts.size(); ++i) { // Loop through all shuffled block strings
        DraggableBlock b; // Create a new draggable block instance
        b.text = allTexts[i]; // Assign the code text to the block
        int col = (int)(i % 2); // Calculate the column index for a 2-column grid
        int row = (int)(i / 2); // Calculate the row index for a 2-column grid
        b.rect = { (float)(startX + col * (colWidth + colGap)), (float)(startY + row * rowHeight), (float)colWidth, 36.0f }; // Position and size the block rectangle
        b.currentSlot = -1; // Mark the block as not currently placed in any slot
        b.isDragging = false; // Initialize drag state to false
        b.dragOffset = { 0, 0 }; // Initialize mouse drag offset to zero
        state.blocks.push_back(b); // Add the configured block to application state
    }

    state.isCarMoving = false; // Reset car movement animation flag
    state.carX = 180.0f; // Reset the car to the start of the track
    state.carTargetLine = 0; // Reset the number of verified lines to zero
    state.carCrashed = false; // Clear any previous crash flag
    state.levelSuccess = false; // Clear any previous level completion flag
    state.showErrorModal = false; // Hide the error report modal
    state.firstErrorLine = -1; // Reset the index of the first detected error
    state.errorModalScrollY = 0.0f; // Reset vertical scroll position for the error modal
}

void InitVisualizerLevel(AppState& state, const VisualizerLevel& level) { // Define function to initialize a Code Visualizer level
    state.showIntroModal = true; // Display the full code overview modal at the start
    state.revealedLinesCount = 1; // Reveal only the first line of code initially
    state.explanationVisible.assign(level.steps.size(), false); // Hide explanations for all steps initially
    state.stepUnderstood.assign(level.steps.size(), false); // Mark all steps as not yet understood
    state.visualizerScrollY = 0.0f; // Reset the vertical scroll position of the visualizer
}

void UpdateTextInput(char* buffer, int maxLen) { // Define function to process keyboard typing into a character buffer
    int key = GetCharPressed(); // Retrieve the next Unicode character pressed on the keyboard
    while (key > 0) { // Process all queued character presses for this frame
        if ((key >= 32) && (key <= 125)) { // Restrict input to printable ASCII characters
            int currentLen = (int)strlen(buffer); // Measure the current string length in the buffer
            if (currentLen < maxLen - 1) { // Ensure space remains for the character and null terminator
                buffer[currentLen] = (char)key; // Append the typed character to the buffer
                buffer[currentLen + 1] = '\0'; // Terminate the C-string with a null character
            }
        }
        key = GetCharPressed(); // Fetch the next character in the input queue
    }
    if (IsKeyPressed(KEY_BACKSPACE)) { // Check if the user pressed Backspace
        int currentLen = (int)strlen(buffer); // Measure the current string length
        if (currentLen > 0) { // Ensure the buffer is not empty before deleting
            buffer[currentLen - 1] = '\0'; // Remove the last character by moving the null terminator back
        }
    }
}

int main() { // Define main application entry point
    const int screenWidth = 1200; // Set fixed window width in pixels
    const int screenHeight = 800; // Set fixed window height in pixels
    InitWindow(screenWidth, screenHeight, "C / C++ Interactive Learning Studio"); // Create and open the Raylib application window
    InitAppFont(); // Load the custom UI font
    SetTargetFPS(60); // Cap the application frame rate at 60 frames per second

    AppState state; // Instantiate the central application state object
    state.registeredUsers = LoadUsers(); // Load user accounts from file or defaults
    LanguageDatabase db = GetDatabase(state.language); // Load curriculum data for the active programming language

    Color kahootColors[4] = { // Define four vibrant colors for multiple-choice quiz buttons
        { 226, 27, 60, 255 }, // Red color for answer option 1
        { 19, 104, 206, 255 }, // Blue color for answer option 2
        { 216, 158, 0, 255 }, // Yellow color for answer option 3
        { 38, 137, 12, 255 } // Green color for answer option 4
    };

    while (!WindowShouldClose()) { // Run the main game and UI loop until the window is closed
        float dt = GetFrameTime(); // Get elapsed time in seconds since the last frame
        Vector2 mouse = GetMousePosition(); // Get the current cursor coordinates
        ScreenState frameScreen = state.currentScreen; // Snapshot the active screen state for consistent rendering during this frame

        BeginDrawing(); // Begin Raylib frame rendering
        ClearBackground(Theme::BG_DARK); // Fill the window background with the dark theme color

        if (frameScreen == ScreenState::AUTH_MENU) { // Handle rendering and logic for the initial authentication menu
            DrawCenteredText("C / C++ LEARNING STUDIO", 180, 42, Theme::TEXT_LIGHT); // Draw the main application title
            DrawCenteredText("Master programming from scratch with clear lessons, quizzes & labs", 240, 20, Theme::TEXT_MUTED); // Draw the subtitle

            float btnW = 360.0f; // Set width for authentication buttons
            float btnH = 60.0f; // Set height for authentication buttons
            float centerX = (screenWidth - btnW) / 2.0f; // Calculate centered X coordinate for the buttons

            if (DrawButton({ centerX, 360.0f, btnW, btnH }, "Login", Theme::BTN_PRIMARY, 24)) { // Draw Login button and check for clicks
                state.inputUsername[0] = '\0'; // Clear the username input buffer
                state.inputPassword[0] = '\0'; // Clear the password input buffer
                state.authErrorMsg = ""; // Clear any prior authentication error message
                state.activeInputBox = 0; // Deselect text input fields
                state.currentScreen = ScreenState::LOGIN; // Navigate to the Login screen
            }
            if (DrawButton({ centerX, 445.0f, btnW, btnH }, "Register New Account", Theme::SUCCESS_COL, 24)) { // Draw Register button and check for clicks
                state.inputUsername[0] = '\0'; // Clear the username input buffer
                state.inputPassword[0] = '\0'; // Clear the password input buffer
                state.authErrorMsg = ""; // Clear any prior authentication error message
                state.activeInputBox = 0; // Deselect text input fields
                state.currentScreen = ScreenState::REGISTER; // Navigate to the Register screen
            }
        }

        else if (frameScreen == ScreenState::LOGIN) { // Handle rendering and logic for the user login screen
            DrawCenteredText("USER LOGIN", 140, 36, Theme::TEXT_LIGHT); // Draw the login screen heading

            Rectangle userBox = { 400, 260, 400, 45 }; // Define bounds for the username input field
            Rectangle passBox = { 400, 360, 400, 45 }; // Define bounds for the password input field

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check for left mouse click to focus an input box
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1; // Focus username box if clicked
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2; // Focus password box if clicked
                else state.activeInputBox = 0; // Unfocus both boxes if clicked elsewhere
            }

            DrawCustomText("Username:", 400, 230, 20, Theme::TEXT_MUTED); // Draw label above the username field
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Choose border color based on focus state
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG); // Draw username input box background
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder); // Draw username input box border
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT); // Render typed username text
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32); // Capture keyboard input if username box is active

            DrawCustomText("Password:", 400, 330, 20, Theme::TEXT_MUTED); // Draw label above the password field
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Choose border color based on focus state
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG); // Draw password input box background
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder); // Draw password input box border
            std::string maskedPass(strlen(state.inputPassword), '*'); // Build an asterisk string matching the password length
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT); // Render masked password text
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32); // Capture keyboard input if password box is active

            if (!state.authErrorMsg.empty()) { // Check if an authentication error message should be shown
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL); // Display the error message in red
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Sign In", Theme::SUCCESS_COL, 20)) { // Draw Sign In button and handle clicks
                bool found = false; // Track whether matching credentials were found
                for (size_t i = 0; i < state.registeredUsers.size(); ++i) { // Search through all registered accounts
                    if (state.registeredUsers[i].username == state.inputUsername && // Compare entered username
                        state.registeredUsers[i].password == state.inputPassword) { // Compare entered password
                        state.currentUserIndex = (int)i; // Store the index of the authenticated user
                        SyncUserDailyStatus(state.registeredUsers[i]); // Update daily streak and lives for the user
                        SaveUsers(state.registeredUsers); // Persist updated user status to file
                        found = true; // Mark credentials as valid
                        break; // Stop searching once matched
                    }
                }
                if (found) { // Check if login succeeded
                    state.currentScreen = ScreenState::MAIN_MENU; // Navigate to the main dashboard
                }
                else { // Handle invalid login credentials
                    state.authErrorMsg = "Invalid username or password!"; // Set login failure message
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) { // Draw Back button and handle clicks
                state.currentScreen = ScreenState::AUTH_MENU; // Return to the authentication menu
            }
        }

        else if (frameScreen == ScreenState::REGISTER) { // Handle rendering and logic for the account registration screen
            DrawCenteredText("CREATE ACCOUNT", 140, 36, Theme::TEXT_LIGHT); // Draw registration screen heading

            Rectangle userBox = { 400, 260, 400, 45 }; // Define bounds for the new username input box
            Rectangle passBox = { 400, 360, 400, 45 }; // Define bounds for the new password input box

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check for mouse clicks to change active input focus
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1; // Focus username input box
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2; // Focus password input box
                else state.activeInputBox = 0; // Remove focus from both boxes
            }

            DrawCustomText("Choose Username:", 400, 230, 20, Theme::TEXT_MUTED); // Draw label for username input
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Select border color for focus feedback
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG); // Draw username box background
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder); // Draw username box outline
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT); // Draw currently entered username
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32); // Process typing when username box is focused

            DrawCustomText("Choose Password:", 400, 330, 20, Theme::TEXT_MUTED); // Draw label for password input
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Select border color for focus feedback
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG); // Draw password box background
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder); // Draw password box outline
            std::string maskedPass(strlen(state.inputPassword), '*'); // Mask the entered password with asterisks
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT); // Display the masked password
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32); // Process typing when password box is focused

            if (!state.authErrorMsg.empty()) { // Check if a validation error exists
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL); // Render the validation error text
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Register", Theme::SUCCESS_COL, 20)) { // Draw Register button and handle submission
                std::string u(state.inputUsername); // Convert username buffer to std::string
                std::string p(state.inputPassword); // Convert password buffer to std::string
                if (u.empty() || p.empty()) { // Validate that neither field is empty
                    state.authErrorMsg = "Fields cannot be empty!"; // Set empty field error message
                }
                else { // Proceed with duplicate username verification
                    bool exists = false; // Track whether the username is already taken
                    for (auto& user : state.registeredUsers) { // Check against every existing user
                        if (user.username == u) exists = true; // Flag duplicate username if matched
                    }
                    if (exists) { // Reject registration if username exists
                        state.authErrorMsg = "Username already taken!"; // Set duplicate username error message
                    }
                    else { // Create and save the new user account
                        User newUser{ u, p, 0, 0, 1, 5, GetTodayDate(), "Basics & Syntax" }; // Initialize new user with starting stats
                        state.registeredUsers.push_back(newUser); // Add new user to the user list
                        state.currentUserIndex = (int)state.registeredUsers.size() - 1; // Set the newly created user as active
                        SaveUsers(state.registeredUsers); // Save the updated user list to disk
                        state.currentScreen = ScreenState::MAIN_MENU; // Navigate to the main dashboard
                    }
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) { // Draw Back button and handle clicks
                state.currentScreen = ScreenState::AUTH_MENU; // Return to the authentication menu
            }
        }

        else if (frameScreen == ScreenState::MAIN_MENU) { // Handle rendering and logic for the main dashboard screen
            const float sidebarW = 270.0f; // Set width of the left navigation sidebar
            DrawRectangle(0, 0, (int)sidebarW, screenHeight, Theme::SIDEBAR_BG); // Draw the sidebar background panel
            DrawLine((int)sidebarW, 0, (int)sidebarW, screenHeight, Theme::BTN_SECONDARY); // Draw vertical separator line on the sidebar edge

            DrawCustomText("TARGET LANGUAGE", 22, 20, 14, Theme::TEXT_MUTED); // Draw header for language selection
            Color cCol = (state.language == TargetLang::LANG_C) ? Theme::SUCCESS_COL : Theme::PANEL_BG; // Highlight C button if C is active
            Color cppCol = (state.language == TargetLang::LANG_CPP) ? Theme::SUCCESS_COL : Theme::PANEL_BG; // Highlight C++ button if C++ is active

            if (DrawButton({ 20, 44, 110, 42 }, "C", cCol, 20)) { // Draw C language toggle button and handle clicks
                state.language = TargetLang::LANG_C; // Switch target language to C
                db = GetDatabase(state.language); // Reload curriculum database for C
            }
            if (DrawButton({ 140, 44, 110, 42 }, "C++", cppCol, 20)) { // Draw C++ language toggle button and handle clicks
                state.language = TargetLang::LANG_CPP; // Switch target language to C++
                db = GetDatabase(state.language); // Reload curriculum database for C++
            }

            DrawLine(20, 105, (int)sidebarW - 20, 105, Theme::BTN_SECONDARY); // Draw horizontal divider below language buttons

            User* currUser = nullptr; // Declare pointer to the currently logged-in user
            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) { // Verify valid active user index
                currUser = &state.registeredUsers[state.currentUserIndex]; // Point to the active user record
                DrawCustomText(currUser->username.c_str(), 22, 122, 22, Theme::TEXT_LIGHT); // Display the active username
                DrawCustomText(TextFormat("Total XP: %d XP", currUser->xp), 22, 150, 17, Theme::WARNING_COL); // Display the active user's total XP
            }

            DrawLine(20, 185, (int)sidebarW - 20, 185, Theme::BTN_SECONDARY); // Draw horizontal divider below user info
            DrawCustomText("LEARNING MODES", 22, 202, 14, Theme::TEXT_MUTED); // Draw section header for learning modes

            float navX = 20.0f; // Set left padding for navigation buttons
            float navW = sidebarW - 40.0f; // Set width for sidebar navigation buttons
            float navH = 48.0f; // Set height for sidebar navigation buttons
            float navStartY = 225.0f; // Set starting Y coordinate for the first mode button
            float navGap = 58.0f; // Set vertical spacing between mode buttons

            if (DrawButton({ navX, navStartY, navW, navH }, "Theory", Theme::BTN_PRIMARY, 20)) { // Draw Theory mode button
                state.currentScreen = ScreenState::THEORY_TOPICS; // Open the Theory topic selection screen
            }
            if (DrawButton({ navX, navStartY + navGap, navW, navH }, "Quiz", Theme::BTN_PRIMARY, 20)) { // Draw Quiz mode button
                if (currUser && currUser->lives <= 0) { // Check if the user has run out of daily lives
                    state.authErrorMsg = "No lives left today! Practice Theory or come back tomorrow."; // Display out-of-lives warning on dashboard
                }
                else { // Allow access to quizzes when lives remain
                    state.authErrorMsg = ""; // Clear any previous warning message
                    state.currentScreen = ScreenState::QUIZ_TOPICS; // Open the Quiz topic selection screen
                }
            }
            if (DrawButton({ navX, navStartY + navGap * 2, navW, navH }, "Constructor", Theme::BTN_PRIMARY, 20)) { // Draw Code Constructor mode button
                state.currentScreen = ScreenState::CONSTRUCTOR_TOPICS; // Open the Constructor topic selection screen
            }
            if (DrawButton({ navX, navStartY + navGap * 3, navW, navH }, "Visualizer", Theme::BTN_PRIMARY, 20)) { // Draw Code Visualizer mode button
                state.currentScreen = ScreenState::VISUALIZER_TOPICS; // Open the Visualizer topic selection screen
            }
            if (DrawButton({ navX, navStartY + navGap * 4, navW, navH }, "Practice", Theme::BTN_PRIMARY, 20)) { // Draw Practice coding mode button
                state.currentScreen = ScreenState::PRACTICE_TOPICS; // Open the Practice topic selection screen
            }
            if (DrawButton({ navX, navStartY + navGap * 5, navW, navH }, "Exam Mode", Theme::WARNING_COL, 20)) { // Draw Exam Mode button highlighted in amber
                state.currentScreen = ScreenState::EXAM_TOPICS; // Open the Exam topic selection screen
            }

            if (DrawButton({ 20, screenHeight - 70.0f, navW, 46 }, "Log Out", Theme::ERROR_COL, 18)) { // Draw Log Out button at bottom of sidebar
                state.currentUserIndex = -1; // Clear the active user index
                state.authErrorMsg = ""; // Clear any displayed messages
                state.currentScreen = ScreenState::AUTH_MENU; // Return to the authentication menu
            }

            float contentX = sidebarW + 30.0f; // Set starting X coordinate for the right-side dashboard area
            float contentW = screenWidth - sidebarW - 60.0f; // Calculate available width for the main dashboard area
            float cardGap = 20.0f; // Set horizontal gap between top summary cards
            float cardW = (contentW - cardGap * 2) / 3.0f; // Divide width evenly among three summary cards
            float cardH = 105.0f; // Set height for each summary card
            float cardY = 25.0f; // Set top Y position for summary cards

            int streakVal = currUser ? currUser->streak : 1; // Read active user's streak or default to 1
            int livesVal = currUser ? currUser->lives : 5; // Read active user's lives or default to 5
            std::string weakVal = currUser ? currUser->weakTopic : "Pointers & Syntax"; // Read active user's weak topic or default

            Rectangle streakCard = { contentX, cardY, cardW, cardH }; // Define bounds for the Daily Streak card
            DrawRectangleRounded(streakCard, 0.16f, 8, Theme::PANEL_BG); // Draw Daily Streak card background
            DrawRectangleRoundedLines(streakCard, 0.16f, 8, Theme::WARNING_COL); // Draw Daily Streak card border
            DrawFlameIcon(streakCard.x + 36, streakCard.y + 52, 1.35f, streakVal > 0); // Render flame icon inside the streak card
            DrawCustomText("DAILY STREAK", (int)streakCard.x + 68, (int)streakCard.y + 20, 14, Theme::TEXT_MUTED); // Draw streak card title
            DrawCustomText(TextFormat("%d %s", streakVal, (streakVal == 1 ? "Day" : "Days")), // Format streak number with singular or plural label
                (int)streakCard.x + 68, (int)streakCard.y + 44, 28, Theme::WARNING_COL); // Draw formatted streak count
            DrawCustomText("Keep learning daily!", (int)streakCard.x + 68, (int)streakCard.y + 78, 13, Theme::TEXT_MUTED); // Draw motivational caption

            Rectangle focusCard = { contentX + cardW + cardGap, cardY, cardW, cardH }; // Define bounds for the Needs Practice card
            DrawRectangleRounded(focusCard, 0.16f, 8, Theme::PANEL_BG); // Draw Needs Practice card background
            DrawRectangleRoundedLines(focusCard, 0.16f, 8, Theme::BTN_PRIMARY); // Draw Needs Practice card border
            DrawCustomText("NEEDS PRACTICE", (int)focusCard.x + 20, (int)focusCard.y + 20, 14, Theme::TEXT_MUTED); // Draw Needs Practice card title
            DrawTextFitWidth(weakVal.c_str(), (int)focusCard.x + 20, (int)focusCard.y + 46, 17, cardW - 36.0f, Theme::TEXT_LIGHT); // Draw weak topic text scaled to fit card width
            DrawCustomText("Focus on this topic next", (int)focusCard.x + 20, (int)focusCard.y + 78, 13, Theme::BTN_HOVER); // Draw practice hint caption

            Rectangle livesCard = { contentX + (cardW + cardGap) * 2, cardY, cardW, cardH }; // Define bounds for the Daily Lives card
            DrawRectangleRounded(livesCard, 0.16f, 8, Theme::PANEL_BG); // Draw Daily Lives card background
            DrawRectangleRoundedLines(livesCard, 0.16f, 8, (livesVal > 0) ? Theme::ERROR_COL : Theme::TEXT_MUTED); // Draw border colored by remaining lives
            DrawCustomText(TextFormat("DAILY LIVES (%d/5)", livesVal), (int)livesCard.x + 20, (int)livesCard.y + 20, 14, Theme::TEXT_MUTED); // Draw Daily Lives card title with count
            for (int h = 0; h < 5; ++h) { // Loop to render five heart icons
                DrawHeartIcon(livesCard.x + 34 + h * 34, livesCard.y + 58, 1.2f, h < livesVal); // Draw filled or empty heart based on remaining lives
            }
            DrawCustomText("0 score in Quiz = -1 Life", (int)livesCard.x + 20, (int)livesCard.y + 80, 13, Theme::TEXT_MUTED); // Explain how lives are lost

            if (!state.authErrorMsg.empty()) { // Check if a dashboard warning message is active
                DrawCustomText(state.authErrorMsg.c_str(), (int)contentX, 142, 17, Theme::ERROR_COL); // Display warning text below the cards
            }

            Rectangle lbPanel = { contentX, 168.0f, contentW, screenHeight - 195.0f }; // Define bounds for the leaderboard panel
            DrawRectangleRounded(lbPanel, 0.04f, 8, Theme::SIDEBAR_BG); // Draw leaderboard panel background
            DrawRectangleRoundedLines(lbPanel, 0.04f, 8, Theme::BTN_SECONDARY); // Draw leaderboard panel border

            DrawCustomText("DIAMOND LEAGUE LEADERBOARD", (int)lbPanel.x + 28, (int)lbPanel.y + 22, 28, Theme::WARNING_COL); // Draw leaderboard title
            DrawCustomText("Compete with fellow programmers. Earn XP in Theory, Quiz, Constructor & Visualizer!", // Draw leaderboard description text
                (int)lbPanel.x + 28, (int)lbPanel.y + 56, 16, Theme::TEXT_MUTED); // Position leaderboard subtitle

            std::vector<User> sortedUsers = state.registeredUsers; // Copy registered users to sort without altering original indices
            std::sort(sortedUsers.begin(), sortedUsers.end(), [](const User& a, const User& b) { // Sort users in descending order by XP
                return a.xp > b.xp; // Place higher XP user ahead of lower XP user
                });

            float rowStartY = lbPanel.y + 98.0f; // Set starting Y coordinate for the first leaderboard row
            int maxRows = std::min((int)sortedUsers.size(), 7); // Display up to 7 top users
            for (int i = 0; i < maxRows; ++i) { // Iterate through the top ranked users
                Rectangle rowRect = { lbPanel.x + 28.0f, rowStartY + i * 68.0f, lbPanel.width - 56.0f, 56.0f }; // Define bounds for the current leaderboard row

                bool isCurrentUser = (currUser && sortedUsers[i].username == currUser->username); // Check if this row represents the logged-in user
                Color rowCol = isCurrentUser ? Color{ 40, 90, 70, 255 } : Theme::PANEL_BG; // Highlight active user's row in green tint
                Color borderCol = isCurrentUser ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY; // Choose row outline color

                DrawRectangleRounded(rowRect, 0.2f, 6, rowCol); // Draw row background rectangle
                DrawRectangleRoundedLines(rowRect, 0.2f, 6, borderCol); // Draw row border

                std::string rankStr = TextFormat("#%d", i + 1); // Format 1-based rank label
                Color rankCol = Theme::TEXT_LIGHT; // Set default rank text color
                if (i == 0) rankCol = { 255, 215, 0, 255 }; // Use gold color for rank 1
                else if (i == 1) rankCol = { 192, 192, 192, 255 }; // Use silver color for rank 2
                else if (i == 2) rankCol = { 205, 127, 50, 255 }; // Use bronze color for rank 3

                DrawCustomText(rankStr.c_str(), (int)rowRect.x + 22, (int)rowRect.y + 16, 22, rankCol); // Draw rank label
                DrawCustomText(sortedUsers[i].username.c_str(), (int)rowRect.x + 85, (int)rowRect.y + 17, 20, Theme::TEXT_LIGHT); // Draw user's name

                std::string streakBadge = TextFormat("%dd streak", sortedUsers[i].streak); // Format user's daily streak badge
                DrawCustomText(streakBadge.c_str(), (int)(rowRect.x + rowRect.width - 230), (int)rowRect.y + 19, 16, Theme::TEXT_MUTED); // Draw streak badge

                std::string xpStr = TextFormat("%d XP", sortedUsers[i].xp); // Format user's XP string
                int xpWidth = (int)MeasureCustomText(xpStr.c_str(), 20); // Measure XP text width for right alignment
                DrawCustomText(xpStr.c_str(), (int)(rowRect.x + rowRect.width - xpWidth - 24), (int)rowRect.y + 17, 20, Theme::WARNING_COL); // Draw right-aligned XP text
            }
        }

        else { // Render compact top status bar for all inner learning screens
            const char* activeLangText = (state.language == TargetLang::LANG_C) ? "Active Lang: C" : "Active Lang: C++"; // Prepare label for currently selected language
            DrawRectangleRounded({ 20, 14, 195, 34 }, 0.3f, 6, Theme::PANEL_BG); // Draw language badge background
            DrawCustomText(activeLangText, 35, 22, 17, Theme::WARNING_COL); // Draw active language text

            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) { // Verify a user is logged in
                User& currUser = state.registeredUsers[state.currentUserIndex]; // Reference the active user
                std::string userBadge = TextFormat("User: %s | %d XP | %d Lives", currUser.username.c_str(), currUser.xp, currUser.lives); // Format user status summary
                DrawRectangleRounded({ 228, 14, 345, 34 }, 0.3f, 6, Theme::PANEL_BG); // Draw user badge background
                DrawTextFitWidth(userBadge.c_str(), 242, 22, 16, 318.0f, Theme::SUCCESS_COL); // Draw user status text scaled to fit
            }
        }

        if (frameScreen == ScreenState::THEORY_TOPICS) { // Handle rendering and logic for Theory topic selection
            DrawCenteredText("THEORY - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT); // Draw screen title
            DrawCenteredText("Start from Topic 1 if you are brand new to programming!", 108, 18, Theme::TEXT_MUTED); // Draw guidance subtitle

            for (size_t i = 0; i < db.theoryTopics.size(); ++i) { // Loop through all available theory topics
                int col = (int)(i % 2); // Calculate 2-column grid column index
                int row = (int)(i / 2); // Calculate 2-column grid row index
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Compute button bounds for the topic
                if (DrawButton(btnRect, db.theoryTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw topic button and check for click
                    state.selectedTopic = (int)i; // Store the selected topic index
                    state.currentCard = 0; // Start at the first flashcard of the topic
                    state.currentScreen = ScreenState::THEORY_CARDS; // Switch to the Theory flashcards screen
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back to Main Menu button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::THEORY_CARDS) { // Handle rendering and logic for Theory lesson flashcards
            const auto& topic = db.theoryTopics[state.selectedTopic]; // Get reference to the selected theory topic
            const auto& card = topic.cards[state.currentCard]; // Get reference to the current lesson card

            DrawCenteredText(topic.topicName.c_str(), 64, 24, Theme::TEXT_MUTED); // Display the active topic name

            Rectangle cardRect = { 140, 108, 920, 475 }; // Define bounds for the lesson card container
            DrawRectangleRounded(cardRect, 0.05f, 10, Theme::PANEL_BG); // Draw card background
            DrawRectangleRoundedLines(cardRect, 0.05f, 10, Theme::BTN_PRIMARY); // Draw card border

            std::string progress = TextFormat("Lesson %d of %d", state.currentCard + 1, (int)topic.cards.size()); // Format lesson progress text
            DrawCustomText(progress.c_str(), 175, 130, 18, Theme::WARNING_COL); // Render progress text
            DrawCenteredText(card.title.c_str(), 164, 29, Theme::TEXT_LIGHT); // Render the lesson card title
            DrawLine(175, 208, 1025, 208, Theme::BTN_SECONDARY); // Draw horizontal divider below card title

            DrawTextWrapped(card.content, { 175, 228, 850, 340 }, 20, Theme::TEXT_LIGHT); // Render wrapped lesson content text

            if (state.currentCard > 0) { // Show Previous button if not on the first card
                if (DrawButton({ 140, 610, 190, 50 }, "< Previous", Theme::BTN_PRIMARY, 20)) { // Draw Previous button
                    state.currentCard--; // Move back one lesson card
                }
            }
            if (state.currentCard < (int)topic.cards.size() - 1) { // Show Next button if more cards remain
                if (DrawButton({ 870, 610, 190, 50 }, "Next >", Theme::BTN_PRIMARY, 20)) { // Draw Next button
                    state.currentCard++; // Advance to the next lesson card
                }
            }
            else { // Show completion actions on the final card of the topic
                if (state.selectedTopic < (int)db.quizTopics.size()) { // Check if a matching quiz topic exists
                    if (DrawButton({ 640, 610, 210, 50 }, "Take Topic Quiz", Theme::WARNING_COL, 20)) { // Draw button to jump straight to the topic quiz
                        if (state.currentUserIndex >= 0) { // Verify a user is logged in
                            state.registeredUsers[state.currentUserIndex].xp += 20; // Award 20 XP for completing the theory topic
                            SaveUsers(state.registeredUsers); // Save updated XP to file
                        }
                        if (state.currentUserIndex >= 0 && state.registeredUsers[state.currentUserIndex].lives <= 0) { // Check if user has no lives left
                            state.authErrorMsg = "No lives left today! Come back tomorrow for Quizzes."; // Set zero-lives warning message
                            state.currentScreen = ScreenState::MAIN_MENU; // Redirect back to main menu
                        }
                        else { // Start the corresponding quiz
                            state.selectedQuizTopic = state.selectedTopic; // Select matching quiz topic index
                            state.currentQuestion = 0; // Start at the first question
                            state.score = 0; // Reset quiz score
                            state.quizFinished = false; // Clear quiz finished flag
                            state.selectedAnswer = -1; // Clear any selected answer
                            state.feedbackTimer = 0.0f; // Reset feedback delay timer
                            state.currentScreen = ScreenState::QUIZ; // Navigate to the Quiz screen
                        }
                    }
                }
                if (DrawButton({ 870, 610, 190, 50 }, "Finish Topic", Theme::SUCCESS_COL, 20)) { // Draw Finish Topic button
                    if (state.currentUserIndex >= 0) { // Verify a user is logged in
                        state.registeredUsers[state.currentUserIndex].xp += 20; // Award 20 XP for completing the topic
                        SaveUsers(state.registeredUsers); // Persist updated XP to file
                    }
                    state.currentScreen = ScreenState::THEORY_TOPICS; // Return to Theory topic selection
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) { // Draw Back to Topics button
                state.currentScreen = ScreenState::THEORY_TOPICS; // Return to Theory topic selection
            }
        }

        else if (frameScreen == ScreenState::QUIZ_TOPICS) { // Handle rendering and logic for Quiz topic selection
            DrawCenteredText("QUIZ - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT); // Draw screen heading
            DrawCenteredText("Choose a topic you studied in Theory to test your knowledge!", 108, 18, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < db.quizTopics.size(); ++i) { // Loop through all quiz topics
                int col = (int)(i % 2); // Calculate grid column index
                int row = (int)(i / 2); // Calculate grid row index
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Define bounds for quiz topic button
                if (DrawButton(btnRect, db.quizTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw quiz topic button and handle click
                    state.selectedQuizTopic = (int)i; // Store selected quiz topic index
                    state.currentQuestion = 0; // Start at question 1
                    state.score = 0; // Reset score to zero
                    state.quizFinished = false; // Mark quiz as active
                    state.selectedAnswer = -1; // Clear answer selection
                    state.feedbackTimer = 0.0f; // Reset feedback display timer
                    state.currentScreen = ScreenState::QUIZ; // Switch to the Quiz gameplay screen
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back to Main Menu button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::QUIZ) { // Handle rendering and logic for the active Quiz game
            const auto& qTopic = db.quizTopics[state.selectedQuizTopic]; // Reference the active quiz topic

            if (!state.quizFinished) { // Check if there are still questions remaining
                const auto& q = qTopic.questions[state.currentQuestion]; // Reference the current quiz question

                DrawCenteredText(qTopic.topicName.c_str(), 58, 20, Theme::TEXT_MUTED); // Draw active quiz topic name
                std::string header = TextFormat("QUESTION %d OF %d", state.currentQuestion + 1, (int)qTopic.questions.size()); // Format question counter
                DrawCenteredText(header.c_str(), 86, 22, Theme::WARNING_COL); // Draw question counter

                Rectangle qBox = { 130, 122, 940, 155 }; // Define bounds for the question prompt panel
                DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG); // Draw question box background
                DrawTextWrapped(q.question, { 165, 152, 870, 105 }, 24, Theme::TEXT_LIGHT); // Render wrapped question text

                for (int i = 0; i < 4; ++i) { // Loop through the four multiple-choice options
                    int col = i % 2; // Compute option grid column
                    int row = i / 2; // Compute option grid row
                    Rectangle optRect = { 130.0f + col * 480.0f, 308.0f + row * 145.0f, 460.0f, 125.0f }; // Define bounds for the answer option button

                    Color btnCol = kahootColors[i]; // Assign default vibrant color to the option button
                    if (state.selectedAnswer != -1) { // Override button colors after an answer is picked
                        if (i == q.correctIndex) btnCol = Theme::SUCCESS_COL; // Highlight the correct option in green
                        else if (i == state.selectedAnswer) btnCol = Theme::ERROR_COL; // Highlight an incorrect choice in red
                        else btnCol = Theme::BTN_SECONDARY; // Dim the remaining unselected options
                    }

                    if (DrawButton(optRect, q.options[i].c_str(), btnCol, 20) && state.selectedAnswer == -1) { // Handle option click if no answer chosen yet
                        state.selectedAnswer = i; // Record the selected answer index
                        if (i == q.correctIndex) { // Check if the user selected the right answer
                            state.score++; // Increment quiz score
                            if (state.currentUserIndex >= 0) { // Verify a user is logged in
                                state.registeredUsers[state.currentUserIndex].xp += 15; // Award 15 XP for a correct answer
                                SaveUsers(state.registeredUsers); // Save updated user XP
                            }
                        }
                        else if (state.currentUserIndex >= 0) { // Handle incorrect answer for logged-in user
                            state.registeredUsers[state.currentUserIndex].weakTopic = qTopic.topicName; // Mark this topic as needing practice
                            SaveUsers(state.registeredUsers); // Persist updated weak topic
                        }
                        state.feedbackTimer = 1.4f; // Set pause duration before moving to the next question
                    }
                }

                if (state.selectedAnswer != -1) { // Check if feedback is currently being shown
                    state.feedbackTimer -= dt; // Decrease feedback timer by frame delta time
                    const char* statusMsg = (state.selectedAnswer == q.correctIndex) ? "Correct! +15 XP" : "Incorrect! Review this in Theory."; // Choose feedback message
                    Color statusCol = (state.selectedAnswer == q.correctIndex) ? Theme::SUCCESS_COL : Theme::ERROR_COL; // Choose feedback text color
                    DrawCenteredText(statusMsg, 625, 28, statusCol); // Display feedback text

                    if (state.feedbackTimer <= 0.0f) { // Advance once the feedback timer expires
                        state.selectedAnswer = -1; // Reset selected answer for the next question
                        state.currentQuestion++; // Move to the next question index
                        if (state.currentQuestion >= (int)qTopic.questions.size()) { // Check if all questions have been answered
                            state.quizFinished = true; // Mark the quiz as completed
                            if (state.score == 0 && state.currentUserIndex >= 0) { // Penalize a zero score by deducting one daily life
                                User& u = state.registeredUsers[state.currentUserIndex]; // Reference the active user
                                if (u.lives > 0) u.lives -= 1; // Deduct one life if above zero
                                SaveUsers(state.registeredUsers); // Persist updated lives count
                            }
                        }
                    }
                }
            }
            else { // Render the quiz completion summary screen
                DrawCenteredText("QUIZ COMPLETED!", 160, 44, Theme::SUCCESS_COL); // Draw completion heading
                DrawCenteredText(qTopic.topicName.c_str(), 218, 24, Theme::TEXT_MUTED); // Draw finished quiz topic name
                std::string scoreStr = TextFormat("Your Final Score: %d / %d", state.score, (int)qTopic.questions.size()); // Format final score summary
                DrawCenteredText(scoreStr.c_str(), 265, 32, Theme::TEXT_LIGHT); // Draw final score

                if (state.score == 0) { // Inform the user if they lost a life due to scoring zero
                    DrawCenteredText("0 correct answers! You lost 1 Daily Life.", 320, 22, Theme::ERROR_COL); // Display life deduction notice
                }

                bool canRetry = true; // Assume user can retry the quiz
                if (state.currentUserIndex >= 0 && state.registeredUsers[state.currentUserIndex].lives <= 0) { // Check if lives reached zero
                    canRetry = false; // Disable retrying quizzes for today
                    DrawCenteredText("Out of lives for today!", 355, 20, Theme::WARNING_COL); // Display out-of-lives message
                }

                if (canRetry) { // Show Retry and Other Topics buttons when lives remain
                    if (DrawButton({ 340, 410, 240, 55 }, "Retry This Topic", Theme::BTN_PRIMARY, 20)) { // Draw Retry button
                        state.currentQuestion = 0; // Reset to first question
                        state.score = 0; // Reset score to zero
                        state.quizFinished = false; // Restart quiz gameplay
                        state.selectedAnswer = -1; // Clear answer state
                    }
                    if (DrawButton({ 620, 410, 240, 55 }, "Other Quiz Topics", Theme::SUCCESS_COL, 20)) { // Draw button to choose another quiz topic
                        state.currentScreen = ScreenState::QUIZ_TOPICS; // Return to quiz topics list
                    }
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Quiz Topics", Theme::BTN_SECONDARY, 20)) { // Draw bottom navigation button
                state.currentScreen = ScreenState::QUIZ_TOPICS; // Return to quiz topic selection
            }
        }

        else if (frameScreen == ScreenState::CONSTRUCTOR_TOPICS) { // Handle rendering and logic for Code Constructor topic selection
            DrawCenteredText("CODE CONSTRUCTOR - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT); // Draw screen heading
            DrawCenteredText("Choose a topic first, then build its lab challenges block-by-block!", 108, 18, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < db.constructorTopics.size(); ++i) { // Loop through all constructor topics
                int col = (int)(i % 2); // Calculate grid column
                int row = (int)(i / 2); // Calculate grid row
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Define bounds for topic button
                if (DrawButton(btnRect, db.constructorTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw topic button and handle click
                    state.selectedConstructorTopic = (int)i; // Store selected constructor topic index
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS; // Navigate to the levels list for this topic
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::CONSTRUCTOR_LEVELS) { // Handle rendering and logic for Code Constructor level selection
            const auto& cTopic = db.constructorTopics[state.selectedConstructorTopic]; // Reference the selected constructor topic
            DrawCenteredText(cTopic.topicName.c_str(), 68, 30, Theme::TEXT_LIGHT); // Draw selected topic heading
            DrawCenteredText("Select a Code Constructor Lab for this topic:", 112, 19, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < cTopic.levels.size(); ++i) { // Loop through all lab levels in the topic
                Rectangle btnRect = { 300.0f, 185.0f + i * 88.0f, 600.0f, 68.0f }; // Define vertical stack bounds for level button
                if (DrawButton(btnRect, cTopic.levels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 22)) { // Draw level button and handle click
                    state.selectedConstructorLevel = (int)i; // Store selected level index
                    InitConstructorLevel(state, cTopic.levels[i]); // Initialize draggable blocks and car state for the level
                    state.currentScreen = ScreenState::CONSTRUCTOR_GAME; // Switch to the constructor puzzle screen
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::CONSTRUCTOR_TOPICS; // Return to constructor topics list
            }
        }

        else if (frameScreen == ScreenState::CONSTRUCTOR_GAME) { // Handle rendering and logic for the Code Constructor puzzle game
            const auto& cTopic = db.constructorTopics[state.selectedConstructorTopic]; // Reference the active constructor topic
            const auto& level = cTopic.levels[state.selectedConstructorLevel]; // Reference the active constructor level
            int numSlots = (int)level.correctLines.size(); // Get total number of code slots in this level

            DrawCenteredText(level.levelTitle.c_str(), 58, 24, Theme::TEXT_LIGHT); // Draw level title below the status bar
            DrawCenteredText("Drag code blocks from the bottom pool into the numbered slots, then click Ready:", 86, 15, Theme::TEXT_MUTED); // Draw gameplay instructions

            DrawLine(180, 144, 1020, 144, Theme::BTN_SECONDARY); // Draw horizontal track line for the car animation
            DrawCar(state.carX, 104, state.carCrashed); // Render the car at its current horizontal position

            std::vector<Rectangle> slotRects(numSlots); // Allocate rectangle bounds for each numbered code slot
            for (int i = 0; i < numSlots; ++i) { // Compute positions for all code slots
                slotRects[i] = { 210.0f, 156.0f + i * 44.0f, 780.0f, 38.0f }; // Position slot i vertically with consistent spacing
            }

            if (state.isCarMoving && !state.carCrashed && !state.levelSuccess) { // Update car validation animation while running
                state.carX += 270.0f * dt; // Move the car rightward across the track
                float stepWidth = 780.0f / (float)numSlots; // Divide track length into equal segments per code line
                int currentInspectSlot = (int)((state.carX - 180.0f) / stepWidth); // Determine which slot the car is currently checking

                if (currentInspectSlot >= 0 && currentInspectSlot < numSlots) { // Validate the slot currently under inspection
                    int bIdx = state.slotBlockIndex[currentInspectSlot]; // Retrieve index of the block placed in this slot
                    bool slotCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[currentInspectSlot]); // Check if slot is filled with the expected line

                    if (!slotCorrect) { // Trigger crash and error report on mismatch or empty slot
                        state.carCrashed = true; // Flag that the car crashed
                        state.isCarMoving = false; // Stop car movement
                        state.firstErrorLine = currentInspectSlot; // Record the first line index that failed
                        state.showErrorModal = true; // Open the detailed error report modal
                        state.errorModalScrollY = 0.0f; // Reset error modal scroll position
                        if (state.currentUserIndex >= 0) { // Update user's weak topic if logged in
                            state.registeredUsers[state.currentUserIndex].weakTopic = cTopic.topicName; // Set current topic as weak topic
                            SaveUsers(state.registeredUsers); // Persist updated user profile
                        }
                    }
                    else { // Mark the inspected line as passed
                        state.carTargetLine = currentInspectSlot + 1; // Advance verified line count
                    }
                }
                else if (currentInspectSlot >= numSlots) { // Trigger level completion when the car passes all slots
                    state.levelSuccess = true; // Mark level as successfully solved
                    state.isCarMoving = false; // Stop car movement
                    state.carTargetLine = numSlots; // Mark all slots as verified
                    if (state.currentUserIndex >= 0) { // Reward the logged-in user
                        state.registeredUsers[state.currentUserIndex].xp += 50; // Grant 50 XP for completing the lab
                        state.registeredUsers[state.currentUserIndex].levelsCompleted += 1; // Increment completed levels counter
                        SaveUsers(state.registeredUsers); // Save progress to file
                    }
                }
            }

            for (int i = 0; i < numSlots; ++i) { // Render each target slot rectangle
                Color slotCol = Theme::PANEL_BG; // Set default neutral slot background color
                if (state.carTargetLine > i) { // Check if the car passed this slot
                    slotCol = Theme::SUCCESS_COL; // Color verified slot green
                }
                else if (state.carCrashed) { // Highlight incorrect slots when crashed
                    int bIdx = state.slotBlockIndex[i]; // Get block placed in slot i
                    bool isCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[i]); // Verify if slot content matches solution
                    if (!isCorrect) slotCol = Theme::ERROR_COL; // Color incorrect or empty slot red
                }

                DrawRectangleRounded(slotRects[i], 0.2f, 6, slotCol); // Draw slot background
                DrawRectangleRoundedLines(slotRects[i], 0.2f, 6, Theme::BTN_SECONDARY); // Draw slot outline
                DrawCustomText(TextFormat("%d.", i + 1), (int)slotRects[i].x - 36, (int)slotRects[i].y + 9, 19, Theme::TEXT_MUTED); // Draw 1-based line number beside the slot
            }

            DrawLine(65, 436, 1135, 436, Theme::BTN_SECONDARY); // Draw divider separating slots from block pool
            DrawCustomText("Available Code Blocks (drag into slots above):", 65, 448, 17, Theme::TEXT_MUTED); // Draw pool header label

            bool interactionLocked = state.isCarMoving || state.showErrorModal || state.levelSuccess; // Lock dragging during animation or modals
            if (!interactionLocked) { // Process drag-and-drop mouse interaction when unlocked
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { // Check if user pressed left mouse button to pick up a block
                    for (int i = (int)state.blocks.size() - 1; i >= 0; --i) { // Iterate top-to-bottom so topmost block is grabbed first
                        if (CheckCollisionPointRec(mouse, state.blocks[i].rect)) { // Check if cursor is over block i
                            state.blocks[i].isDragging = true; // Start dragging this block
                            state.blocks[i].dragOffset = { mouse.x - state.blocks[i].rect.x, mouse.y - state.blocks[i].rect.y }; // Save click offset relative to block corner
                            if (state.blocks[i].currentSlot != -1) { // Free the slot if block was pulled out of one
                                state.slotBlockIndex[state.blocks[i].currentSlot] = -1; // Clear slot occupancy reference
                                state.blocks[i].currentSlot = -1; // Detach block from slot
                            }
                            break; // Stop checking other blocks once one is grabbed
                        }
                    }
                }

                for (auto& b : state.blocks) { // Update positions of any currently dragged block
                    if (b.isDragging) { // Check if block b is being dragged
                        b.rect.x = mouse.x - b.dragOffset.x; // Move block X coordinate to follow cursor
                        b.rect.y = mouse.y - b.dragOffset.y; // Move block Y coordinate to follow cursor

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) { // Handle dropping the block when mouse button is released
                            b.isDragging = false; // Stop dragging state
                            for (int s = 0; s < numSlots; ++s) { // Check if dropped inside an available slot
                                if (CheckCollisionPointRec(mouse, slotRects[s]) && state.slotBlockIndex[s] == -1) { // Verify cursor is over empty slot s
                                    b.currentSlot = s; // Assign block to slot s
                                    b.rect.x = slotRects[s].x + 8.0f; // Snap block X position neatly inside slot
                                    b.rect.y = slotRects[s].y + 1.0f; // Snap block Y position neatly inside slot
                                    state.slotBlockIndex[s] = (int)(&b - &state.blocks[0]); // Store block index in the slot occupancy array
                                    break; // Stop checking remaining slots
                                }
                            }
                        }
                    }
                }
            }

            for (int pass = 0; pass < 2; ++pass) { // Draw blocks in two passes so the actively dragged block renders on top
                for (size_t i = 0; i < state.blocks.size(); ++i) { // Iterate through all code blocks
                    const auto& b = state.blocks[i]; // Reference block i
                    if ((pass == 0 && b.isDragging) || (pass == 1 && !b.isDragging)) continue; // Draw stationary blocks on pass 0 and dragged block on pass 1

                    Color blockColor = Theme::BTN_PRIMARY; // Set default block color
                    if (b.currentSlot != -1) { // Adjust color if block is currently slotted
                        if (state.carTargetLine > b.currentSlot) { // Check if slot was validated as correct
                            blockColor = Theme::SUCCESS_COL; // Highlight validated block in green
                        }
                        else if (state.carCrashed && b.text != level.correctLines[b.currentSlot]) { // Check if block caused an error
                            blockColor = Theme::ERROR_COL; // Highlight incorrect block in red
                        }
                    }

                    DrawRectangleRounded(b.rect, 0.2f, 6, blockColor); // Draw block background rectangle
                    DrawRectangleRoundedLines(b.rect, 0.2f, 6, Theme::TEXT_LIGHT); // Draw crisp light outline around block
                    DrawTextFitWidth(b.text.c_str(), (int)b.rect.x + 12, (int)b.rect.y + 9, 17, b.rect.width - 24.0f, Theme::TEXT_LIGHT); // Draw block code text fitted within bounds
                }
            }

            if (!state.isCarMoving && !state.levelSuccess && !state.showErrorModal) { // Show Ready button when idle
                if (DrawButton({ 740, 720, 220, 50 }, "Ready (Run Car)", Theme::SUCCESS_COL, 20)) { // Draw Ready button and start validation run
                    state.carX = 180.0f; // Place car at track start
                    state.carTargetLine = 0; // Reset verified line counter
                    state.carCrashed = false; // Clear crash state
                    state.isCarMoving = true; // Start car validation animation
                }
            }

            if (DrawButton({ 240, 720, 220, 50 }, "Back to Labs", Theme::BTN_SECONDARY, 20)) { // Draw Back to Labs button
                state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS; // Return to level selection for this topic
            }

            if (state.levelSuccess) { // Render victory modal when all lines pass
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 185 }); // Dim background behind modal
                Rectangle modal = { 280, 220, 640, 320 }; // Define bounds for success modal box
                DrawRectangleRounded(modal, 0.08f, 10, Theme::PANEL_BG); // Draw modal background
                DrawRectangleRoundedLines(modal, 0.08f, 10, Theme::SUCCESS_COL); // Draw green modal border

                DrawCenteredText("CONGRATULATIONS!", 265, 36, Theme::SUCCESS_COL); // Draw victory heading
                DrawCenteredText("The car passed every line! +50 XP earned.", 330, 22, Theme::TEXT_LIGHT); // Draw XP reward message

                if (DrawButton({ 340, 440, 230, 50 }, "Play Again", Theme::BTN_PRIMARY, 20)) { // Draw Play Again button
                    InitConstructorLevel(state, level); // Reset and replay the current level
                }
                if (DrawButton({ 630, 440, 230, 50 }, "Topic Labs", Theme::BTN_SECONDARY, 20)) { // Draw Topic Labs button
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS; // Return to level list
                }
            }

            if (state.showErrorModal) { // Render scrollable error report modal when the car crashes
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 195 }); // Dim background behind error modal
                Rectangle modal = { 130, 75, 940, 650 }; // Define bounds for error modal container
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG); // Draw modal background
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::ERROR_COL); // Draw red error modal outline

                DrawCenteredText("CAR CRASHED! CODE ERRORS DETECTED", 100, 28, Theme::ERROR_COL); // Draw crash heading
                std::string crashInfo = TextFormat("First breakdown at Line %d. Scroll down if there are multiple errors:", state.firstErrorLine + 1); // Format first error line indicator
                DrawCenteredText(crashInfo.c_str(), 138, 18, Theme::WARNING_COL); // Draw crash info subtitle

                std::string errorReport = ""; // Accumulate detailed error descriptions for all wrong slots
                for (int s = 0; s < numSlots; ++s) { // Inspect every slot in the level
                    int bIdx = state.slotBlockIndex[s]; // Get block placed in slot s
                    std::string userCode = (bIdx == -1) ? "[Empty Slot]" : state.blocks[bIdx].text; // Get user's placed text or empty placeholder
                    if (userCode != level.correctLines[s]) { // Check if slot s does not match expected code
                        errorReport += TextFormat("Line %d:\n  Your block: %s\n  Expected:   %s\n  Why: %s\n\n", // Append formatted error entry
                            s + 1, userCode.c_str(), level.correctLines[s].c_str(), level.slotExplanations[s].c_str()); // Supply line number, user code, expected code, and explanation
                    }
                }

                Rectangle reportViewport = { 165, 175, 870, 445 }; // Define scrollable viewport bounds inside modal
                DrawRectangleRounded(reportViewport, 0.03f, 6, Theme::BG_DARK); // Draw dark viewport background

                float contentH = MeasureTextWrappedHeight(errorReport, reportViewport.width - 36.0f, 17) + 20.0f; // Measure total height of wrapped error text
                float maxScroll = std::max(0.0f, contentH - reportViewport.height); // Compute maximum allowed scroll offset

                float wheel = GetMouseWheelMove(); // Poll mouse wheel scroll delta
                if (wheel != 0.0f) state.errorModalScrollY -= wheel * 35.0f; // Scroll vertically with mouse wheel
                if (IsKeyDown(KEY_DOWN)) state.errorModalScrollY += 260.0f * dt; // Scroll downward when Down arrow is held
                if (IsKeyDown(KEY_UP)) state.errorModalScrollY -= 260.0f * dt; // Scroll upward when Up arrow is held
                state.errorModalScrollY = std::clamp(state.errorModalScrollY, 0.0f, maxScroll); // Clamp scroll offset within valid bounds

                BeginScissorMode((int)reportViewport.x + 4, (int)reportViewport.y + 4, // Enable scissor clipping to keep text inside viewport
                    (int)reportViewport.width - 8, (int)reportViewport.height - 8); // Pass clipped width and height
                DrawTextWrapped(errorReport, // Render wrapped error report text shifted by scroll offset
                    { reportViewport.x + 18.0f, reportViewport.y + 12.0f - state.errorModalScrollY, // Set scrolled text origin
                      reportViewport.width - 36.0f, contentH }, // Set text bounding box width and height
                    17, Theme::TEXT_LIGHT); // Use font size 17 and light text color
                EndScissorMode(); // Disable scissor clipping

                if (maxScroll > 0.0f) { // Draw scrollbar thumb when content exceeds viewport height
                    float barH = std::max(36.0f, reportViewport.height * (reportViewport.height / contentH)); // Calculate proportional scrollbar height
                    float barY = reportViewport.y + (state.errorModalScrollY / maxScroll) * (reportViewport.height - barH); // Calculate scrollbar Y position
                    DrawRectangleRounded({ reportViewport.x + reportViewport.width - 10.0f, barY, 6.0f, barH }, 0.5f, 4, Theme::TEXT_MUTED); // Render scrollbar thumb
                }

                if (DrawButton({ 330, 645, 240, 50 }, "Retry Level", Theme::BTN_PRIMARY, 20)) { // Draw Retry Level button
                    InitConstructorLevel(state, level); // Reset level for another attempt
                }
                if (DrawButton({ 630, 645, 240, 50 }, "Topic Labs", Theme::BTN_SECONDARY, 20)) { // Draw Topic Labs button
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS; // Return to constructor level list
                }
            }
        }

        else if (frameScreen == ScreenState::VISUALIZER_TOPICS) { // Handle rendering and logic for Code Visualizer topic selection
            DrawCenteredText("CODE VISUALIZER - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT); // Draw screen heading
            DrawCenteredText("Choose a topic first, then inspect its lab solutions line-by-line!", 108, 18, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < db.visualizerTopics.size(); ++i) { // Loop through all visualizer topics
                int col = (int)(i % 2); // Calculate 2-column grid column
                int row = (int)(i / 2); // Calculate 2-column grid row
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Define bounds for topic button
                if (DrawButton(btnRect, db.visualizerTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw topic button and check click
                    state.selectedVisualizerTopic = (int)i; // Store selected visualizer topic index
                    state.currentScreen = ScreenState::VISUALIZER_LEVELS; // Switch to visualizer level selection
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::VISUALIZER_LEVELS) { // Handle rendering and logic for Code Visualizer level selection
            const auto& vTopic = db.visualizerTopics[state.selectedVisualizerTopic]; // Reference the active visualizer topic
            DrawCenteredText(vTopic.topicName.c_str(), 68, 30, Theme::TEXT_LIGHT); // Draw topic title
            DrawCenteredText("Select a Visualizer Lab Walkthrough for this topic:", 112, 19, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < vTopic.levels.size(); ++i) { // Loop through visualizer labs in this topic
                Rectangle btnRect = { 300.0f, 185.0f + i * 88.0f, 600.0f, 68.0f }; // Define button bounds in vertical list
                if (DrawButton(btnRect, vTopic.levels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 22)) { // Draw level button and handle click
                    state.selectedVisualizerLevel = (int)i; // Store selected visualizer level index
                    InitVisualizerLevel(state, vTopic.levels[i]); // Reset step reveal states and open intro modal
                    state.currentScreen = ScreenState::VISUALIZER_VIEW; // Switch to step-by-step visualizer view
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::VISUALIZER_TOPICS; // Return to visualizer topic selection
            }
        }

        else if (frameScreen == ScreenState::VISUALIZER_VIEW) { // Handle rendering and logic for step-by-step Code Visualizer walkthrough
            const auto& vTopic = db.visualizerTopics[state.selectedVisualizerTopic]; // Reference the selected visualizer topic
            const auto& level = vTopic.levels[state.selectedVisualizerLevel]; // Reference the active visualizer level

            DrawCenteredText(level.levelTitle.c_str(), 58, 26, Theme::TEXT_LIGHT); // Draw lab title cleanly below top status bar

            Rectangle viewport = { 75.0f, 100.0f, 1050.0f, 560.0f }; // Define scrollable code walkthrough viewport bounds
            DrawRectangleRounded(viewport, 0.03f, 8, Theme::SIDEBAR_BG); // Draw viewport background
            DrawRectangleRoundedLines(viewport, 0.03f, 8, Theme::BTN_SECONDARY); // Draw viewport border

            float totalContentH = 16.0f; // Initialize total dynamic height calculation with top padding
            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) { // Accumulate heights of all currently revealed steps
                totalContentH += 44.0f; // Add height for the code line box and vertical gap
                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) { // Account for the Explanation button when shown
                    totalContentH += 42.0f; // Add Explanation button height and spacing
                }
                if (state.explanationVisible[i]) { // Account for expanded explanation box height
                    float expTextH = MeasureTextWrappedHeight(level.steps[i].explanation, 720.0f, 17); // Measure wrapped explanation text height
                    float expBoxH = std::max(46.0f, expTextH + 20.0f); // Enforce minimum box height plus padding
                    totalContentH += expBoxH + 8.0f; // Add explanation box height and gap
                    if (!state.stepUnderstood[i]) { // Account for Got It button if step not yet confirmed
                        totalContentH += 42.0f; // Add Got It button height and gap
                    }
                }
            }
            totalContentH += 20.0f; // Add bottom padding to total scrollable height

            float maxScroll = std::max(0.0f, totalContentH - viewport.height); // Calculate maximum vertical scroll offset
            if (!state.showIntroModal) { // Allow scrolling only when intro modal is closed
                float wheel = GetMouseWheelMove(); // Poll mouse wheel delta
                if (wheel != 0.0f) state.visualizerScrollY -= wheel * 40.0f; // Scroll with mouse wheel
                if (IsKeyDown(KEY_DOWN)) state.visualizerScrollY += 260.0f * dt; // Scroll down with Down arrow key
                if (IsKeyDown(KEY_UP)) state.visualizerScrollY -= 260.0f * dt; // Scroll up with Up arrow key
            }
            state.visualizerScrollY = std::clamp(state.visualizerScrollY, 0.0f, maxScroll); // Keep scroll position within bounds

            BeginScissorMode((int)viewport.x + 4, (int)viewport.y + 4, // Clip drawing strictly to the visualizer viewport
                (int)viewport.width - 8, (int)viewport.height - 8); // Supply inner clipped width and height

            bool mouseInViewport = CheckCollisionPointRec(mouse, viewport) && !state.showIntroModal; // Gate button clicks to inside viewport when modal is hidden
            float curY = viewport.y + 16.0f - state.visualizerScrollY; // Set starting scrolled Y cursor for rendering steps

            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) { // Render each revealed code step
                Rectangle lineBox = { 105.0f, curY, 790.0f, 38.0f }; // Define bounds for the code line panel
                DrawRectangleRounded(lineBox, 0.2f, 6, Theme::PANEL_BG); // Draw code line background
                DrawRectangleRoundedLines(lineBox, 0.2f, 6, Theme::BTN_PRIMARY); // Draw code line outline
                DrawTextFitWidth(level.steps[i].codeLine.c_str(), 122, (int)curY + 9, 18, lineBox.width - 28.0f, Theme::TEXT_LIGHT); // Draw code text fitted to box width

                if (state.stepUnderstood[i]) { // Provide a toggle button for already understood steps
                    const char* toggleLabel = state.explanationVisible[i] ? "Hide Info" : "Show Info"; // Choose label based on current visibility
                    Rectangle toggleRect = { 912.0f, curY, 175.0f, 38.0f }; // Position toggle button to the right of the code box
                    if (DrawButton(toggleRect, toggleLabel, Theme::BTN_SECONDARY, 17) && mouseInViewport) { // Handle toggle button click
                        state.explanationVisible[i] = !state.explanationVisible[i]; // Flip explanation visibility for step i
                    }
                }

                curY += 44.0f; // Advance drawing Y cursor past the code line box

                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) { // Show Explanation button for the active uninspected step
                    Rectangle expBtn = { 125.0f, curY, 190.0f, 34.0f }; // Define bounds for Explanation button
                    if (DrawButton(expBtn, "Explanation", Theme::WARNING_COL, 18) && mouseInViewport) { // Handle Explanation click
                        state.explanationVisible[i] = true; // Reveal the explanation box for step i
                    }
                    curY += 42.0f; // Advance drawing Y cursor past the button
                }

                if (state.explanationVisible[i]) { // Render the explanation panel when visible
                    float expTextH = MeasureTextWrappedHeight(level.steps[i].explanation, 720.0f, 17); // Measure height needed for wrapped explanation
                    float expBoxH = std::max(46.0f, expTextH + 20.0f); // Compute dynamic box height
                    Rectangle expBox = { 125.0f, curY, 760.0f, expBoxH }; // Define bounds for explanation box
                    DrawRectangleRounded(expBox, 0.16f, 6, { 30, 58, 50, 255 }); // Draw tinted green explanation background
                    DrawRectangleRoundedLines(expBox, 0.16f, 6, Theme::SUCCESS_COL); // Draw green explanation border
                    DrawTextWrapped(level.steps[i].explanation, { 145.0f, curY + 10.0f, 720.0f, expTextH + 8.0f }, 17, Theme::TEXT_LIGHT); // Render wrapped explanation text
                    curY += expBoxH + 8.0f; // Advance drawing Y cursor past the explanation box

                    if (!state.stepUnderstood[i]) { // Show Got It button until the user acknowledges understanding
                        Rectangle gotItBtn = { 125.0f, curY, 160.0f, 34.0f }; // Define bounds for Got It button
                        if (DrawButton(gotItBtn, "Got It!", Theme::SUCCESS_COL, 18) && mouseInViewport) { // Handle Got It confirmation click
                            state.stepUnderstood[i] = true; // Mark step i as understood
                            state.explanationVisible[i] = false; // Collapse explanation to keep workspace tidy
                            if (state.revealedLinesCount < (int)level.steps.size()) { // Check if more code lines remain
                                state.revealedLinesCount++; // Reveal the next line of code
                            }
                            if (state.currentUserIndex >= 0) { // Award XP to logged-in user
                                state.registeredUsers[state.currentUserIndex].xp += 5; // Grant 5 XP per understood step
                                SaveUsers(state.registeredUsers); // Persist updated XP
                            }
                        }
                        curY += 42.0f; // Advance drawing Y cursor past Got It button
                    }
                }
            }

            EndScissorMode(); // End viewport scissor clipping

            if (maxScroll > 0.0f) { // Render scrollbar indicator if content overflows viewport
                float barH = std::max(40.0f, viewport.height * (viewport.height / totalContentH)); // Compute scrollbar thumb height
                float barY = viewport.y + (state.visualizerScrollY / maxScroll) * (viewport.height - barH); // Compute scrollbar thumb Y position
                DrawRectangleRounded({ viewport.x + viewport.width - 12.0f, barY, 7.0f, barH }, 0.5f, 4, Theme::TEXT_MUTED); // Draw scrollbar thumb
            }

            if (!state.stepUnderstood.empty() && state.stepUnderstood.back()) { // Check if the final step has been understood
                DrawCenteredText("All lines analyzed! Scroll or toggle any explanation above, or pick another lab.", 674, 18, Theme::SUCCESS_COL); // Display completion prompt
            }
            else if (maxScroll > 0.0f) { // Show scrolling hint when content is scrollable
                DrawCenteredText("Tip: Use Mouse Wheel or Up/Down keys to scroll through steps.", 674, 16, Theme::TEXT_MUTED); // Draw scrolling tip
            }

            if (DrawButton({ 480, 718, 240, 50 }, "Back to Labs", Theme::BTN_SECONDARY, 20)) { // Draw Back to Labs button
                state.currentScreen = ScreenState::VISUALIZER_LEVELS; // Return to visualizer level selection
            }

            if (state.showIntroModal) { // Render full code overview modal before starting walkthrough
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 205 }); // Dim screen behind modal
                Rectangle modal = { 180, 85, 840, 610 }; // Define bounds for intro modal
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG); // Draw modal background
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::BTN_PRIMARY); // Draw modal border

                DrawCenteredText("LAB CODE OVERVIEW", 112, 30, Theme::WARNING_COL); // Draw intro modal heading
                DrawCenteredText("Here is the complete code we will break down step-by-step:", 152, 18, Theme::TEXT_LIGHT); // Draw intro modal subtitle

                Rectangle codeBox = { 220, 190, 760, 405 }; // Define bounds for full code preview panel
                DrawRectangleRounded(codeBox, 0.04f, 8, Theme::BG_DARK); // Draw dark code preview background

                BeginScissorMode((int)codeBox.x + 4, (int)codeBox.y + 4, (int)codeBox.width - 8, (int)codeBox.height - 8); // Clip full code text inside preview box
                DrawTextWrapped(level.fullCode, { codeBox.x + 22.0f, codeBox.y + 18.0f, codeBox.width - 44.0f, codeBox.height - 30.0f }, 18, Theme::SUCCESS_COL); // Render complete lab source code
                EndScissorMode(); // End code preview clipping

                if (DrawButton({ 320, 620, 220, 50 }, "Back to Labs", Theme::ERROR_COL, 20)) { // Draw button to cancel and go back
                    state.currentScreen = ScreenState::VISUALIZER_LEVELS; // Return to visualizer level list
                }
                if (DrawButton({ 660, 620, 220, 50 }, "Let's Go!", Theme::SUCCESS_COL, 20)) { // Draw button to dismiss modal and begin walkthrough
                    state.showIntroModal = false; // Hide intro overview modal
                }
            }
        }

        else if (frameScreen == ScreenState::PRACTICE_TOPICS) { // Handle rendering and logic for Practice Mode topic selection
            DrawCenteredText("PRACTICE - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT); // Draw screen heading
            DrawCenteredText("Choose a topic to solve hands-on coding assignments!", 108, 18, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < db.practiceTopics.size(); ++i) { // Loop through all Practice topics
                int col = (int)(i % 2); // Calculate 2-column grid column index
                int row = (int)(i / 2); // Calculate 2-column grid row index
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Define bounds for the topic button
                if (DrawButton(btnRect, db.practiceTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw topic button and handle clicks
                    state.selectedPracticeTopic = (int)i; // Store selected Practice topic index
                    state.currentScreen = ScreenState::PRACTICE_TASKS; // Navigate to the tasks list for this topic
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::PRACTICE_TASKS) { // Handle rendering and logic for Practice Mode task selection
            const auto& pTopic = db.practiceTopics[state.selectedPracticeTopic]; // Reference the selected Practice topic
            DrawCenteredText(pTopic.topicName.c_str(), 68, 30, Theme::TEXT_LIGHT); // Draw selected topic heading
            DrawCenteredText("Select a coding task to practice:", 112, 19, Theme::TEXT_MUTED); // Draw subtitle

            for (size_t i = 0; i < pTopic.tasks.size(); ++i) { // Loop through all tasks in the chosen topic
                Rectangle btnRect = { 280.0f, 185.0f + i * 88.0f, 640.0f, 68.0f }; // Define vertically stacked task button bounds
                if (DrawButton(btnRect, pTopic.tasks[i].taskTitle.c_str(), Theme::BTN_PRIMARY, 22)) { // Draw task button and handle selection
                    state.selectedPracticeTask = (int)i; // Store selected task index
                    state.practiceCodeBuffer.clear(); // Clear code editor contents for the new task
                    state.practiceChecked = false; // Reset submission checked state
                    state.practiceIsCorrect = false; // Reset correctness flag
                    state.practiceFeedback.clear(); // Clear previous feedback text
                    state.practiceEditorScrollY = 0.0f; // Reset editor scroll position
                    state.practiceFeedbackScrollY = 0.0f; // Reset feedback scroll position
                    state.currentScreen = ScreenState::PRACTICE_EDITOR; // Open the Practice code editor
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::PRACTICE_TOPICS; // Return to Practice topic list
            }
        }

        else if (frameScreen == ScreenState::PRACTICE_EDITOR) { // Handle rendering and logic for the interactive Practice code editor
            const auto& pTopic = db.practiceTopics[state.selectedPracticeTopic]; // Reference active Practice topic
            const auto& task = pTopic.tasks[state.selectedPracticeTask]; // Reference active Practice coding task

            DrawCenteredText(task.taskTitle.c_str(), 56, 26, Theme::TEXT_LIGHT); // Draw task title below status bar

            Rectangle taskBox = { 80.0f, 92.0f, 1040.0f, 98.0f }; // Define bounds for task prompt container
            DrawRectangleRounded(taskBox, 0.08f, 8, Theme::PANEL_BG); // Draw task prompt background
            DrawRectangleRoundedLines(taskBox, 0.08f, 8, Theme::BTN_PRIMARY); // Draw task prompt border
            DrawCustomText("TASK:", 100, 102, 15, Theme::WARNING_COL); // Draw task label
            DrawTextWrapped(task.description, { 100.0f, 122.0f, 1000.0f, 64.0f }, 17, Theme::TEXT_LIGHT); // Render wrapped task description

            float editorH = state.practiceChecked ? 245.0f : 430.0f; // Shrink editor height after checking to make room for feedback
            Rectangle editorBox = { 80.0f, 204.0f, 1040.0f, editorH }; // Define bounds for the code editor box
            DrawRectangleRounded(editorBox, 0.04f, 8, Theme::SIDEBAR_BG); // Draw code editor background
            DrawRectangleRoundedLines(editorBox, 0.04f, 8, state.practiceChecked ? Theme::BTN_SECONDARY : Theme::SUCCESS_COL); // Highlight editor border green while editing

            if (!state.practiceChecked) { // Accept typing only before submission is checked
                UpdateCodeEditorInput(state.practiceCodeBuffer, state.backspaceHoldTimer, dt); // Update code buffer from keyboard input
            }

            int lineCount = 1; // Count total lines in the code buffer starting at 1
            for (char c : state.practiceCodeBuffer) if (c == '\n') lineCount++; // Increment count for every newline character
            float codeContentH = (float)lineCount * 24.0f + 30.0f; // Estimate total pixel height of typed code
            float maxEdScroll = std::max(0.0f, codeContentH - (editorBox.height - 20.0f)); // Compute maximum vertical editor scroll
            if (CheckCollisionPointRec(mouse, editorBox)) { // Allow wheel scrolling when cursor hovers over the editor
                float wheel = GetMouseWheelMove(); // Poll mouse wheel movement
                if (wheel != 0.0f) state.practiceEditorScrollY -= wheel * 32.0f; // Apply vertical scroll delta
            }
            state.practiceEditorScrollY = std::clamp(state.practiceEditorScrollY, 0.0f, maxEdScroll); // Clamp editor scroll within valid bounds

            BeginScissorMode((int)editorBox.x + 6, (int)editorBox.y + 6, (int)editorBox.width - 12, (int)editorBox.height - 12); // Clip code rendering inside the editor box
            DrawCodeWithCursor(state.practiceCodeBuffer, { editorBox.x + 14.0f, editorBox.y + 12.0f, editorBox.width - 28.0f, editorBox.height - 20.0f }, // Draw line-numbered source code and blinking cursor
                state.practiceEditorScrollY, 18, !state.practiceChecked); // Pass scroll offset, font size, and cursor visibility
            EndScissorMode(); // End editor scissor clipping

            if (!state.practiceChecked) { // Show typing tip and Check button while editing
                DrawCustomText("Tip: Type your code directly. Press Enter for a new line, Tab for indentation.", 82, 644, 15, Theme::TEXT_MUTED); // Display editor usage hint

                if (DrawButton({ 480.0f, 695.0f, 240.0f, 54.0f }, "Check", Theme::SUCCESS_COL, 22)) { // Draw Check button and evaluate submission on click
                    state.practiceIsCorrect = EvaluatePracticeCode(state.practiceCodeBuffer, task, state.practiceFeedback); // Validate submitted code and generate feedback
                    state.practiceChecked = true; // Lock editor and reveal result box
                    state.practiceFeedbackScrollY = 0.0f; // Reset feedback scroll position

                    if (state.practiceIsCorrect && state.currentUserIndex >= 0) { // Reward logged-in user for correct solution
                        state.registeredUsers[state.currentUserIndex].xp += 40; // Grant 40 XP for solving the Practice task
                        state.registeredUsers[state.currentUserIndex].levelsCompleted += 1; // Increment completed levels counter
                        SaveUsers(state.registeredUsers); // Persist updated user stats
                    }
                    else if (!state.practiceIsCorrect && state.currentUserIndex >= 0) { // Track weak topic if submission failed
                        state.registeredUsers[state.currentUserIndex].weakTopic = pTopic.topicName; // Update user's weak topic to this Practice topic
                        SaveUsers(state.registeredUsers); // Persist updated user profile
                    }
                }
            }
            else { // Render evaluation result panel and post-check navigation buttons
                Rectangle resBox = { 80.0f, 464.0f, 1040.0f, 220.0f }; // Define bounds for the result panel below the editor
                Color resBorder = state.practiceIsCorrect ? Theme::SUCCESS_COL : Theme::ERROR_COL; // Select green or red border based on verdict
                Color resBg = state.practiceIsCorrect ? Color{ 28, 56, 44, 255 } : Color{ 62, 30, 36, 255 }; // Select subtle tinted green or red background
                DrawRectangleRounded(resBox, 0.06f, 8, resBg); // Draw result panel background
                DrawRectangleRoundedLines(resBox, 0.06f, 8, resBorder); // Draw result panel outline

                const char* verdict = state.practiceIsCorrect ? "CORRECT! (+40 XP)" : "INCORRECT!"; // Choose verdict banner text
                DrawCustomText(verdict, 102, 476, 24, resBorder); // Draw verdict banner

                const char* subLabel = state.practiceIsCorrect ? "Program Output:" : "What is wrong:"; // Choose subheading for output or diagnostics
                DrawCustomText(subLabel, 102, 508, 17, Theme::WARNING_COL); // Draw feedback subheading

                Rectangle fbView = { 102.0f, 534.0f, 996.0f, 138.0f }; // Define scrollable viewport for feedback text
                float fbH = MeasureTextWrappedHeight(state.practiceFeedback, fbView.width - 12.0f, 17) + 16.0f; // Measure total wrapped height of feedback
                float maxFbScroll = std::max(0.0f, fbH - fbView.height); // Compute maximum vertical feedback scroll
                if (CheckCollisionPointRec(mouse, resBox)) { // Scroll feedback box when mouse hovers over it
                    float wheel = GetMouseWheelMove(); // Poll mouse wheel delta
                    if (wheel != 0.0f) state.practiceFeedbackScrollY -= wheel * 30.0f; // Shift feedback scroll offset
                }
                state.practiceFeedbackScrollY = std::clamp(state.practiceFeedbackScrollY, 0.0f, maxFbScroll); // Clamp feedback scroll offset

                BeginScissorMode((int)fbView.x, (int)fbView.y, (int)fbView.width, (int)fbView.height); // Clip feedback text to viewport
                DrawTextWrapped(state.practiceFeedback, // Render wrapped program output or error diagnostics
                    { fbView.x, fbView.y - state.practiceFeedbackScrollY, fbView.width - 12.0f, fbH }, // Position text shifted by scroll offset
                    17, Theme::TEXT_LIGHT); // Use font size 17 and light text color
                EndScissorMode(); // End feedback scissor clipping

                if (DrawButton({ 340.0f, 706.0f, 240.0f, 52.0f }, "Retry Task", Theme::BTN_PRIMARY, 20)) { // Draw Retry Task button
                    state.practiceCodeBuffer.clear(); // Clear code editor for a fresh attempt
                    state.practiceChecked = false; // Unlock editor
                    state.practiceIsCorrect = false; // Clear correctness flag
                    state.practiceFeedback.clear(); // Clear feedback text
                    state.practiceEditorScrollY = 0.0f; // Reset editor scroll position
                }
                if (DrawButton({ 620.0f, 706.0f, 240.0f, 52.0f }, "Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Main Menu button
                    state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
                }
            }
        }

        else if (frameScreen == ScreenState::EXAM_TOPICS) { // Handle rendering and logic for Exam Mode topic selection
            DrawCenteredText("EXAM MODE - SELECT A TOPIC", 68, 32, Theme::WARNING_COL); // Draw screen heading in amber
            DrawCenteredText("Answer all questions sequentially. No hints until the exam ends!", 108, 18, Theme::TEXT_MUTED); // Draw exam instructions subtitle

            for (size_t i = 0; i < db.examTopics.size(); ++i) { // Loop through all Exam topics
                int col = (int)(i % 2); // Calculate 2-column grid column index
                int row = (int)(i / 2); // Calculate 2-column grid row index
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f }; // Define topic button bounds
                if (DrawButton(btnRect, db.examTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) { // Draw topic button and initialize exam on click
                    state.selectedExamTopic = (int)i; // Store selected Exam topic index
                    state.currentExamQuestion = 0; // Start at the first exam question
                    size_t qCount = db.examTopics[i].questions.size(); // Get total number of questions in this exam
                    state.examUserChoice.assign(qCount, -1); // Initialize all multiple-choice selections to unanswered
                    state.examUserText.assign(qCount, ""); // Initialize all text responses to empty strings
                    state.examScore = 0; // Reset exam score counter
                    state.examReviewScrollY = 0.0f; // Reset review screen scroll offset
                    state.currentScreen = ScreenState::EXAM_QUESTION; // Start the exam
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Back button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::EXAM_QUESTION) { // Handle rendering and logic for each sequential Exam question
            const auto& eTopic = db.examTopics[state.selectedExamTopic]; // Reference active Exam topic
            int totalQ = (int)eTopic.questions.size(); // Get total question count for this exam
            const auto& q = eTopic.questions[state.currentExamQuestion]; // Reference current exam question

            DrawCenteredText(eTopic.topicName.c_str(), 58, 20, Theme::TEXT_MUTED); // Draw active exam topic name
            DrawCenteredText(TextFormat("EXAM QUESTION %d OF %d", state.currentExamQuestion + 1, totalQ), 86, 24, Theme::WARNING_COL); // Draw question progress indicator

            Rectangle qBox = { 130.0f, 128.0f, 940.0f, 150.0f }; // Define bounds for the question prompt box
            DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG); // Draw question box background
            DrawRectangleRoundedLines(qBox, 0.08f, 8, Theme::BTN_PRIMARY); // Draw question box border
            DrawTextWrapped(q.question, { 165.0f, 156.0f, 870.0f, 105.0f }, 23, Theme::TEXT_LIGHT); // Render wrapped question prompt

            if (q.type == ExamQuestionType::MULTIPLE_CHOICE) { // Render four selectable choices for multiple-choice questions
                DrawCenteredText("Select your answer:", 292, 17, Theme::TEXT_MUTED); // Draw selection instruction
                for (int i = 0; i < (int)q.options.size() && i < 4; ++i) { // Loop through up to four answer options
                    int col = i % 2; // Compute grid column index
                    int row = i / 2; // Compute grid row index
                    Rectangle optRect = { 130.0f + col * 480.0f, 326.0f + row * 126.0f, 460.0f, 104.0f }; // Define bounds for option button
                    bool isSelected = (state.examUserChoice[state.currentExamQuestion] == i); // Check if user selected this option
                    Color optCol = isSelected ? Theme::SUCCESS_COL : Theme::PANEL_BG; // Highlight chosen option in green

                    if (DrawButton(optRect, q.options[i].c_str(), optCol, 20)) { // Draw option button and record click
                        state.examUserChoice[state.currentExamQuestion] = i; // Save chosen option index for this question
                    }
                }
            }
            else { // Render single-line text input box for text-entry exam questions
                DrawCenteredText("Type your answer below:", 318, 19, Theme::TEXT_MUTED); // Draw typing prompt
                Rectangle inputBox = { 260.0f, 360.0f, 680.0f, 64.0f }; // Define bounds for text answer box
                DrawRectangleRounded(inputBox, 0.16f, 8, Theme::PANEL_BG); // Draw input box background
                DrawRectangleRoundedLines(inputBox, 0.16f, 8, Theme::SUCCESS_COL); // Draw active green input box border

                std::string& ansBuf = state.examUserText[state.currentExamQuestion]; // Reference current question's text answer buffer
                UpdateExamTextInput(ansBuf, state.backspaceHoldTimer, dt); // Capture keyboard typing into the answer buffer

                std::string displayTxt = ansBuf; // Copy buffer for display with optional blinking cursor
                if ((int)(GetTime() * 2.0) % 2 == 0) displayTxt += "|"; // Append blinking cursor every half second
                DrawCustomText(displayTxt.c_str(), (int)inputBox.x + 20, (int)inputBox.y + 20, 22, Theme::TEXT_LIGHT); // Render typed answer inside input box
            }

            bool isLast = (state.currentExamQuestion == totalQ - 1); // Check whether this is the final question in the exam
            const char* nextLabel = isLast ? "Finish Exam" : "Next >"; // Choose button label based on position in exam
            if (DrawButton({ 460.0f, 610.0f, 280.0f, 56.0f }, nextLabel, Theme::BTN_PRIMARY, 22)) { // Draw Next or Finish Exam button
                if (!isLast) { // Advance to the next question if more remain
                    state.currentExamQuestion++; // Increment current exam question index
                }
                else { // Grade all answers when finishing the exam
                    int correctCount = 0; // Initialize counter for correct answers
                    for (int i = 0; i < totalQ; ++i) { // Evaluate every question in the exam
                        if (IsExamAnswerCorrect(eTopic.questions[i], state.examUserChoice[i], state.examUserText[i])) { // Check if answer i is correct
                            correctCount++; // Increment correct answer tally
                        }
                    }
                    state.examScore = correctCount; // Store final exam score in state

                    if (state.currentUserIndex >= 0) { // Update logged-in user's XP and weak topic
                        state.registeredUsers[state.currentUserIndex].xp += correctCount * 20; // Award 20 XP per correct exam answer
                        if (correctCount * 2 < totalQ) { // Flag topic for practice if score is below 50%
                            state.registeredUsers[state.currentUserIndex].weakTopic = eTopic.topicName; // Update user's weak topic
                        }
                        SaveUsers(state.registeredUsers); // Persist updated user data to disk
                    }
                    state.currentScreen = ScreenState::EXAM_RESULT; // Transition to the Exam Result summary screen
                }
            }

            if (DrawButton({ 480.0f, 715.0f, 240.0f, 48.0f }, "Cancel Exam", Theme::BTN_SECONDARY, 18)) { // Draw Cancel Exam button
                state.currentScreen = ScreenState::EXAM_TOPICS; // Abort exam and return to topic list
            }
        }

        else if (frameScreen == ScreenState::EXAM_RESULT) { // Handle rendering and logic for the Exam completion summary screen
            const auto& eTopic = db.examTopics[state.selectedExamTopic]; // Reference completed Exam topic
            int totalQ = (int)eTopic.questions.size(); // Get total number of exam questions
            int pct = (totalQ > 0) ? (int)((100.0f * state.examScore) / totalQ) : 0; // Calculate integer percentage score
            std::string gradeStr = GetLetterGrade(state.examScore, totalQ); // Determine corresponding letter grade string

            DrawCenteredText("EXAM COMPLETED!", 145, 42, Theme::WARNING_COL); // Draw completion heading
            DrawCenteredText(eTopic.topicName.c_str(), 202, 22, Theme::TEXT_MUTED); // Draw exam topic name

            Rectangle scoreCard = { 300.0f, 250.0f, 600.0f, 195.0f }; // Define bounds for score summary card
            DrawRectangleRounded(scoreCard, 0.08f, 10, Theme::PANEL_BG); // Draw score card background
            DrawRectangleRoundedLines(scoreCard, 0.08f, 10, (pct >= 60) ? Theme::SUCCESS_COL : Theme::ERROR_COL); // Outline card green for passing or red for failing

            DrawCenteredText(TextFormat("Score: %d / %d (%d%%)", state.examScore, totalQ, pct), 286, 32, Theme::TEXT_LIGHT); // Display numeric and percentage score
            DrawCenteredText(TextFormat("Grade: %s", gradeStr.c_str()), 342, 30, (pct >= 60) ? Theme::SUCCESS_COL : Theme::ERROR_COL); // Display letter grade
            DrawCenteredText(TextFormat("XP Earned: +%d XP", state.examScore * 20), 394, 20, Theme::WARNING_COL); // Display earned XP reward

            if (DrawButton({ 455.0f, 495.0f, 290.0f, 56.0f }, "Result Review", Theme::BTN_PRIMARY, 21)) { // Draw Result Review button
                state.examReviewScrollY = 0.0f; // Reset review viewport scroll offset
                state.currentScreen = ScreenState::EXAM_REVIEW; // Navigate to detailed per-question review
            }
            if (DrawButton({ 455.0f, 571.0f, 290.0f, 54.0f }, "Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Main Menu button beneath Result Review
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        else if (frameScreen == ScreenState::EXAM_REVIEW) { // Handle rendering and logic for scrollable per-question Exam review
            const auto& eTopic = db.examTopics[state.selectedExamTopic]; // Reference reviewed Exam topic
            int totalQ = (int)eTopic.questions.size(); // Get total number of questions

            DrawCenteredText("EXAM RESULT REVIEW", 62, 30, Theme::TEXT_LIGHT); // Draw review heading
            DrawCenteredText(eTopic.topicName.c_str(), 98, 18, Theme::TEXT_MUTED); // Draw exam topic subtitle

            Rectangle viewport = { 90.0f, 132.0f, 1020.0f, 555.0f }; // Define scrollable review container bounds
            DrawRectangleRounded(viewport, 0.03f, 8, Theme::SIDEBAR_BG); // Draw review viewport background
            DrawRectangleRoundedLines(viewport, 0.03f, 8, Theme::BTN_SECONDARY); // Draw review viewport border

            float totalH = 16.0f; // Initialize accumulated height for all review cards
            for (int i = 0; i < totalQ; ++i) { // Sum card heights based on whether each answer was correct or wrong
                bool ok = IsExamAnswerCorrect(eTopic.questions[i], state.examUserChoice[i], state.examUserText[i]); // Check correctness of question i
                totalH += ok ? 112.0f : 142.0f; // Add taller height for wrong answers that also display the correct answer
            }
            float maxScroll = std::max(0.0f, totalH - viewport.height); // Compute maximum scroll limit
            float wheel = GetMouseWheelMove(); // Poll mouse wheel delta
            if (wheel != 0.0f) state.examReviewScrollY -= wheel * 40.0f; // Scroll review list with mouse wheel
            if (IsKeyDown(KEY_DOWN)) state.examReviewScrollY += 260.0f * dt; // Scroll down when holding Down arrow
            if (IsKeyDown(KEY_UP)) state.examReviewScrollY -= 260.0f * dt; // Scroll up when holding Up arrow
            state.examReviewScrollY = std::clamp(state.examReviewScrollY, 0.0f, maxScroll); // Clamp review scroll offset within range

            BeginScissorMode((int)viewport.x + 4, (int)viewport.y + 4, (int)viewport.width - 8, (int)viewport.height - 8); // Clip review cards strictly inside viewport
            float curY = viewport.y + 16.0f - state.examReviewScrollY; // Set starting scrolled Y coordinate for first review card

            for (int i = 0; i < totalQ; ++i) { // Render a review card for every question in the exam
                const auto& q = eTopic.questions[i]; // Reference question i
                bool ok = IsExamAnswerCorrect(q, state.examUserChoice[i], state.examUserText[i]); // Determine whether user got question i right

                std::string userAnsStr = "[No Answer]"; // Default display text if user left question blank
                if (q.type == ExamQuestionType::MULTIPLE_CHOICE) { // Format user's chosen option for multiple-choice questions
                    int idx = state.examUserChoice[i]; // Retrieve chosen option index
                    if (idx >= 0 && idx < (int)q.options.size()) userAnsStr = q.options[idx]; // Look up chosen option text if valid
                }
                else if (!state.examUserText[i].empty()) { // Use typed response for text-entry questions if non-empty
                    userAnsStr = state.examUserText[i]; // Copy user's typed text
                }

                float cardH = ok ? 98.0f : 128.0f; // Set taller card height when showing the correct answer line
                Rectangle itemCard = { viewport.x + 22.0f, curY, viewport.width - 44.0f, cardH }; // Define review card bounds
                DrawRectangleRounded(itemCard, 0.12f, 8, Theme::PANEL_BG); // Draw review card background
                DrawRectangleRoundedLines(itemCard, 0.12f, 8, ok ? Theme::SUCCESS_COL : Theme::ERROR_COL); // Outline card green or red based on accuracy

                std::string qTitle = TextFormat("Q%d: %s", i + 1, q.question.c_str()); // Format numbered question title
                DrawTextFitWidth(qTitle.c_str(), (int)itemCard.x + 16, (int)itemCard.y + 12, 18, itemCard.width - 140.0f, Theme::TEXT_LIGHT); // Draw question text fitted to card width

                const char* badge = ok ? "CORRECT" : "WRONG"; // Choose right-aligned status badge label
                DrawCustomText(badge, (int)(itemCard.x + itemCard.width - 110), (int)itemCard.y + 12, 17, ok ? Theme::SUCCESS_COL : Theme::ERROR_COL); // Draw status badge

                std::string yourLine = "Your Answer: " + userAnsStr; // Format user's submitted answer line
                DrawTextFitWidth(yourLine.c_str(), (int)itemCard.x + 16, (int)itemCard.y + 48, 17, itemCard.width - 32.0f, // Draw user's answer fitted inside card
                    ok ? Theme::SUCCESS_COL : Theme::ERROR_COL); // Color user's answer green if correct or red if wrong

                if (!ok) { // Show the canonical correct answer whenever the user missed the question
                    std::string correctLine = "Correct Answer: " + q.correctAnswerDisplay; // Format expected answer text
                    DrawTextFitWidth(correctLine.c_str(), (int)itemCard.x + 16, (int)itemCard.y + 82, 17, itemCard.width - 32.0f, Theme::WARNING_COL); // Draw expected answer in amber
                }

                curY += cardH + 14.0f; // Advance vertical drawing cursor to the next review card
            }
            EndScissorMode(); // End review viewport scissor clipping

            if (maxScroll > 0.0f) { // Draw scrollbar thumb if review cards overflow viewport
                float barH = std::max(40.0f, viewport.height * (viewport.height / totalH)); // Compute proportional scrollbar thumb height
                float barY = viewport.y + (state.examReviewScrollY / maxScroll) * (viewport.height - barH); // Compute scrollbar vertical position
                DrawRectangleRounded({ viewport.x + viewport.width - 12.0f, barY, 7.0f, barH }, 0.5f, 4, Theme::TEXT_MUTED); // Render scrollbar thumb
            }

            if (DrawButton({ 340.0f, 712.0f, 240.0f, 50.0f }, "Back to Grade", Theme::BTN_PRIMARY, 20)) { // Draw Back to Grade button
                state.currentScreen = ScreenState::EXAM_RESULT; // Return to the Exam score summary screen
            }
            if (DrawButton({ 620.0f, 712.0f, 240.0f, 50.0f }, "Main Menu", Theme::BTN_SECONDARY, 20)) { // Draw Main Menu button
                state.currentScreen = ScreenState::MAIN_MENU; // Return to main dashboard
            }
        }

        EndDrawing(); // Finish Raylib frame rendering and swap buffers
    }

    UnloadAppFont(); // Free custom loaded UI font resources
    CloseWindow(); // Close the Raylib window and OpenGL context
    return 0; // Return success status code to the operating system
}