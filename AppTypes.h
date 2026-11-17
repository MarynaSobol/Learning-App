#ifndef APP_TYPES_H // Prevent the header file from being included multiple times
#define APP_TYPES_H // Define the header guard macro

#include "raylib.h" // Include the Raylib graphics library for colors, rectangles, and vectors
#include <string>   // Include the standard string library for handling text
#include <vector>   // Include the standard vector library for dynamic lists/arrays

// Enum representing the programming languages the user can choose to study
enum class TargetLang {
    LANG_C,   // Represents the C programming language
    LANG_CPP  // Represents the C++ programming language
};

// Enum representing every possible screen or page in the application
enum class ScreenState {
    AUTH_MENU,          // Welcome screen where the user chooses to log in or register
    LOGIN,              // Screen for entering existing username and password
    REGISTER,           // Screen for creating a new user account
    MAIN_MENU,          // Main hub screen with buttons for all learning modes
    LEADERBOARD,        // Screen showing user rankings sorted by XP (Duolingo-style)
    SETTINGS,           // Screen for changing the target programming language
    THEORY_TOPICS,      // Screen displaying the list of available theory topics
    THEORY_CARDS,       // Screen showing individual study flashcards for a chosen topic
    QUIZ,               // Screen for answering multiple-choice quiz questions
    CONSTRUCTOR_LEVELS, // Screen for selecting a drag-and-drop coding puzzle level
    CONSTRUCTOR_GAME,   // Screen where the user arranges code blocks and runs the test car
    VISUALIZER_LEVELS,  // Screen for selecting a step-by-step code breakdown level
    VISUALIZER_VIEW     // Screen showing code line-by-line with interactive explanations
};

// --- USER & LEADERBOARD STRUCTURES ---
// Stores profile information and progress for a single user
struct User {
    std::string username;    // The user's login name
    std::string password;    // The user's password
    int xp = 0;              // Experience points earned by completing tasks
    int levelsCompleted = 0; // Total number of levels the user has finished
};

// --- THEORY STRUCTURES ---
// Represents a single study card inside a theory topic
struct Flashcard {
    std::string title;   // The heading or concept name on the card
    std::string content; // The detailed explanation or code example on the card
};

// Groups multiple flashcards under a single subject title
struct TheoryTopic {
    std::string topicName;        // Name of the topic (e.g., "1. Basics & Syntax")
    std::vector<Flashcard> cards; // List of flashcards belonging to this topic
};

// --- QUIZ STRUCTURES ---
// Represents a single multiple-choice question in the quiz mode
struct QuizQuestion {
    std::string question;   // The question text shown to the user
    std::string options[4]; // Array of 4 possible answers
    int correctIndex;       // Index (0 to 3) of the correct answer in the options array
};

// --- CONSTRUCTOR STRUCTURES ---
// Defines a single drag-and-drop code assembly puzzle
struct ConstructorLevel {
    std::string levelTitle;                     // Title of the puzzle level
    std::vector<std::string> correctLines;      // Lines of code in the exact right order
    std::vector<std::string> slotExplanations;  // Reasons why each line belongs in its specific spot
    std::vector<std::string> extraBlocks;       // Extra "decoy" code blocks to make the puzzle harder
};

// Represents a single movable code block on the screen in Constructor mode
struct DraggableBlock {
    std::string text;   // The line of code written on this block
    Rectangle rect;     // Position (x, y) and size (width, height) of the block on screen
    int currentSlot;    // Slot index (0 to N-1) where the block is placed, or -1 if still in the bottom pool
    bool isDragging;    // True if the user is currently holding and moving this block with the mouse
    Vector2 dragOffset; // Distance between the mouse cursor and the top-left corner of the block while dragging
};

// --- VISUALIZER STRUCTURES ---
// Represents a single line of code and its explanation in the Visualizer mode
struct CodeStep {
    std::string codeLine;    // One line of code from the example program
    std::string explanation; // Plain-English explanation of what this line of code does
};

// Defines a complete code walkthrough lesson for the Visualizer mode
struct VisualizerLevel {
    std::string levelTitle;      // Title of the visualizer lesson
    std::string fullCode;        // The complete code shown in the preview popup before starting
    std::vector<CodeStep> steps; // Step-by-step list of code lines and their explanations
};

// --- GLOBAL APPLICATION STATE ---
// Holds all active data, user progress, and UI states while the program is running
struct AppState {
    ScreenState currentScreen = ScreenState::AUTH_MENU; // Tracks which screen is currently displayed (starts at login/register choice)
    TargetLang language = TargetLang::LANG_CPP;         // Tracks whether C or C++ content is currently active (defaults to C++)

    // Users & Database of registered users
    std::vector<User> registeredUsers = { // List of all user accounts (pre-filled with 3 sample users)
        {"Alice_Coder", "123", 450, 5},   // Sample user 1 with 450 XP
        {"Bob_CPlusPlus", "123", 320, 3}, // Sample user 2 with 320 XP
        {"Charlie_Dev", "123", 180, 2}    // Sample user 3 with 180 XP
    };
    int currentUserIndex = -1; // Index of the currently logged-in user in registeredUsers (-1 means nobody is logged in)

    // Text input buffers for Login / Register screens
    char inputUsername[32] = "\0"; // Character buffer storing the username being typed
    char inputPassword[32] = "\0"; // Character buffer storing the password being typed
    std::string authErrorMsg = ""; // Error message displayed if login or registration fails

    // Auth Input Focus State (0 = none, 1 = username, 2 = password)
    int activeInputBox = 0; // Tracks which text box the user clicked on to type into

    // Theory State
    int selectedTopic = 0; // Index of the currently chosen theory topic
    int currentCard = 0;   // Index of the flashcard currently being viewed

    // Quiz State
    int currentQuestion = 0;     // Index of the active quiz question
    int score = 0;               // Number of questions answered correctly in the current quiz
    bool quizFinished = false;   // True when all questions in the quiz have been answered
    int selectedAnswer = -1;     // Index (0-3) of the answer the user clicked, or -1 if none chosen yet
    float feedbackTimer = 0.0f;  // Countdown timer (in seconds) showing "Correct!" or "Incorrect!" before moving on

    // Constructor State
    int selectedConstructorLevel = 0;   // Index of the currently chosen Constructor level
    std::vector<DraggableBlock> blocks; // All draggable code blocks currently on the screen
    std::vector<int> slotBlockIndex;    // Tracks which block index is placed in each target slot (-1 if slot is empty)
    bool isCarMoving = false;           // True while the inspection car is driving across the screen
    float carX = 0.0f;                  // Current horizontal X-coordinate of the inspection car
    int carTargetLine = 0;              // Number of code lines the car has successfully checked so far
    bool carCrashed = false;            // True if the car hit a wrong or missing code block
    bool levelSuccess = false;          // True if the car reached the end with all code blocks in the right place
    bool showErrorModal = false;        // True when the error report popup window is visible
    int firstErrorLine = -1;            // Index of the first line where the car detected a mistake

    // Visualizer State
    int selectedVisualizerLevel = 0;     // Index of the currently chosen Visualizer lesson
    bool showIntroModal = true;          // True when the initial full-code overview popup is displayed
    int revealedLinesCount = 1;          // Number of code lines currently unlocked and visible on screen
    std::vector<bool> explanationVisible; // Tracks whether the explanation box is open for each line
    std::vector<bool> stepUnderstood;     // Tracks whether the user clicked "Got It!" for each line
};

#endif // APP_TYPES_H // End of header guard