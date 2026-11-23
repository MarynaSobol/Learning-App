// Purpose: Defines core data structures, enumerations, and global application state for the C/C++ Learning Studio.
#ifndef APP_TYPES_H // Prevent multiple inclusions of this header file
#define APP_TYPES_H // Define the header guard macro

#include "raylib.h" // Include Raylib graphics and input library definitions
#include <string> // Include standard string class support
#include <vector> // Include standard dynamic array container support

enum class TargetLang { // Enumeration representing selectable target programming languages
    LANG_C, // Identifier for the C programming language
    LANG_CPP // Identifier for the C++ programming language
};

enum class ScreenState { // Enumeration representing all navigable UI screens
    AUTH_MENU, // Initial authentication landing screen
    LOGIN, // User login screen
    REGISTER, // New account registration screen
    MAIN_MENU, // Main dashboard screen with sidebar and leaderboard
    LEADERBOARD, // Leaderboard view state
    SETTINGS, // Application settings view state
    THEORY_TOPICS, // Topic selection screen for Theory mode
    THEORY_CARDS, // Flashcard lesson viewer screen for Theory mode
    QUIZ_TOPICS, // Topic selection screen for Quiz mode
    QUIZ, // Interactive multiple-choice quiz screen
    CONSTRUCTOR_TOPICS, // Topic selection screen for Code Constructor mode
    CONSTRUCTOR_LEVELS, // Lab level selection screen for Code Constructor mode
    CONSTRUCTOR_GAME, // Drag-and-drop Code Constructor gameplay screen
    VISUALIZER_TOPICS, // Topic selection screen for Code Visualizer mode
    VISUALIZER_LEVELS, // Lab level selection screen for Code Visualizer mode
    VISUALIZER_VIEW // Step-by-step code walkthrough screen for Visualizer mode
};

struct User { // Data structure storing profile details and progress for a user
    std::string username; // Account username for login and display
    std::string password; // Account password for authentication
    int xp = 0; // Total experience points earned by the user
    int levelsCompleted = 0; // Total number of lab levels completed
    int streak = 1; // Number of consecutive days the user has been active
    int lives = 5; // Remaining daily quiz lives with a maximum of five per day
    std::string lastActiveDate = ""; // Last active date formatted as YYYY-MM-DD
    std::string weakTopic = "Pointers & Syntax"; // Topic recommended for additional practice
};

struct Flashcard { // Data structure representing a single theory lesson card
    std::string title; // Title heading of the flashcard
    std::string content; // Detailed instructional text of the flashcard
};

struct TheoryTopic { // Data structure grouping theory flashcards under a topic
    std::string topicName; // Display name of the theory topic
    std::vector<Flashcard> cards; // Collection of flashcards belonging to this topic
};

struct QuizQuestion { // Data structure representing a single four-option quiz question
    std::string question; // Prompt text of the quiz question
    std::string options[4]; // Array holding four possible answer choices
    int correctIndex; // Zero-based index of the correct answer in options
};

struct QuizTopic { // Data structure grouping quiz questions under a topic
    std::string topicName; // Display name of the quiz topic
    std::vector<QuizQuestion> questions; // List of questions in this quiz topic
};

struct ConstructorLevel { // Data structure defining a Code Constructor lab challenge
    std::string levelTitle; // Title of the constructor lab level
    std::vector<std::string> correctLines; // Ordered lines of code forming the valid solution
    std::vector<std::string> slotExplanations; // Line-by-line explanations shown when an error occurs
    std::vector<std::string> extraBlocks; // Distractor code blocks mixed into the draggable pool
};

struct ConstructorTopic { // Data structure grouping constructor labs under a topic
    std::string topicName; // Display name of the constructor topic
    std::vector<ConstructorLevel> levels; // List of constructor lab levels in this topic
};

struct DraggableBlock { // Data structure representing a movable code block in Constructor mode
    std::string text; // Code text displayed inside the draggable block
    Rectangle rect; // Current screen position and dimensions of the block
    int currentSlot; // Index of the slot holding this block or -1 if in the pool
    bool isDragging; // Flag indicating whether the user is currently dragging this block
    Vector2 dragOffset; // Cursor offset from the top-left corner when dragging starts
};

struct CodeStep { // Data structure pairing one code line with its explanation
    std::string codeLine; // Single line or snippet of code to display
    std::string explanation; // Plain-English explanation of what this code line does
};

struct VisualizerLevel { // Data structure defining a step-by-step Visualizer walkthrough
    std::string levelTitle; // Title of the visualizer lab walkthrough
    std::string fullCode; // Complete source code shown in the intro overview modal
    std::vector<CodeStep> steps; // Sequential list of code steps and explanations
};

struct VisualizerTopic { // Data structure grouping visualizer labs under a topic
    std::string topicName; // Display name of the visualizer topic
    std::vector<VisualizerLevel> levels; // List of visualizer walkthrough levels in this topic
};

struct AppState { // Central structure tracking runtime UI and gameplay state
    ScreenState currentScreen = ScreenState::AUTH_MENU; // Currently active application screen
    TargetLang language = TargetLang::LANG_CPP; // Currently selected programming language

    std::vector<User> registeredUsers = { // List of registered user accounts with default demo profiles
        {"Alice_Coder", "123", 450, 5, 7, 5, "2026-11-18", "STL & Vectors"}, // Default demo user profile for Alice
        {"Bob_CPlusPlus", "123", 320, 3, 4, 4, "2026-11-18", "Memory Management"}, // Default demo user profile for Bob
        {"Charlie_Dev", "123", 180, 2, 2, 5, "2026-11-18", "Control Flow"} // Default demo user profile for Charlie
    };
    int currentUserIndex = -1; // Index of the logged-in user or -1 if logged out

    char inputUsername[32] = "\0"; // Input text buffer for username entry
    char inputPassword[32] = "\0"; // Input text buffer for password entry
    std::string authErrorMsg = ""; // Error or status message displayed on auth and menu screens
    int activeInputBox = 0; // Tracks which text input box currently has focus

    int selectedTopic = 0; // Index of the currently selected theory topic
    int currentCard = 0; // Index of the currently viewed flashcard

    int selectedQuizTopic = 0; // Index of the currently selected quiz topic
    int currentQuestion = 0; // Index of the active quiz question
    int score = 0; // Number of correct answers in the current quiz session
    bool quizFinished = false; // Flag indicating whether all quiz questions are answered
    int selectedAnswer = -1; // Index of the answer chosen for the current question or -1
    float feedbackTimer = 0.0f; // Countdown timer for displaying answer feedback

    int selectedConstructorTopic = 0; // Index of the currently selected constructor topic
    int selectedConstructorLevel = 0; // Index of the currently selected constructor level
    std::vector<DraggableBlock> blocks; // All draggable code blocks active in the current level
    std::vector<int> slotBlockIndex; // Maps each target slot to the index of its placed block
    bool isCarMoving = false; // Flag indicating whether the validation car is animating
    float carX = 0.0f; // Current horizontal screen coordinate of the validation car
    int carTargetLine = 0; // Number of code slots successfully validated by the car
    bool carCrashed = false; // Flag indicating whether the car hit an incorrect slot
    bool levelSuccess = false; // Flag indicating whether the level was completed cleanly
    bool showErrorModal = false; // Flag controlling visibility of the error report modal
    int firstErrorLine = -1; // Zero-based index of the first slot containing an error
    float errorModalScrollY = 0.0f; // Vertical scroll offset inside the error report modal

    int selectedVisualizerTopic = 0; // Index of the currently selected visualizer topic
    int selectedVisualizerLevel = 0; // Index of the currently selected visualizer level
    bool showIntroModal = true; // Flag controlling visibility of the full-code intro modal
    int revealedLinesCount = 1; // Number of code lines currently revealed in Visualizer mode
    std::vector<bool> explanationVisible; // Tracks whether each step's explanation box is expanded
    std::vector<bool> stepUnderstood; // Tracks whether the user has confirmed understanding each step
    float visualizerScrollY = 0.0f; // Vertical scroll offset inside the visualizer viewport
};

#endif // End of APP_TYPES_H header guard