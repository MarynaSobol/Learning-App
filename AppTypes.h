// Purpose of AppTypes.h: Defines core enumerations, data structures for users and all learning modes (Theory, Quiz, Constructor, Visualizer, Practice, and Exam), and the central AppState struct holding runtime state for the C / C++ Learning Studio.

#ifndef APP_TYPES_H // Guard against multiple inclusions of this header file
#define APP_TYPES_H // Define the APP_TYPES_H include guard macro

#include "raylib.h" // Include Raylib types such as Rectangle and Vector2
#include <string> // Include std::string for text fields
#include <vector> // Include std::vector for dynamic collections

enum class TargetLang { // Enumerate the supported target programming languages
    LANG_C, // Select the C programming language curriculum
    LANG_CPP // Select the C++ programming language curriculum
};

enum class ScreenState { // Enumerate all navigable screens in the application
    AUTH_MENU, // Initial landing screen with Login and Register options
    LOGIN, // Existing user sign-in screen
    REGISTER, // New account creation screen
    MAIN_MENU, // Primary dashboard with sidebar, stats cards, and leaderboard
    LEADERBOARD, // Dedicated leaderboard view
    SETTINGS, // Application settings screen
    THEORY_TOPICS, // Topic selection screen for Theory lessons
    THEORY_CARDS, // Flashcard lesson viewer for a selected Theory topic
    QUIZ_TOPICS, // Topic selection screen for Quizzes
    QUIZ, // Active multiple-choice quiz gameplay screen
    CONSTRUCTOR_TOPICS, // Topic selection screen for Code Constructor
    CONSTRUCTOR_LEVELS, // Level selection screen within a Constructor topic
    CONSTRUCTOR_GAME, // Interactive drag-and-drop Code Constructor puzzle screen
    VISUALIZER_TOPICS, // Topic selection screen for Code Visualizer
    VISUALIZER_LEVELS, // Level selection screen within a Visualizer topic
    VISUALIZER_VIEW, // Step-by-step code walkthrough viewer
    PRACTICE_TOPICS, // Topic selection screen for Practice coding mode
    PRACTICE_TASKS, // Task selection screen within a Practice topic
    PRACTICE_EDITOR, // Interactive code editor screen for solving a Practice task
    EXAM_TOPICS, // Topic selection screen for Exam mode
    EXAM_QUESTION, // Active exam question screen
    EXAM_RESULT, // Exam score summary screen
    EXAM_REVIEW // Detailed answer review screen after completing an exam
};

struct User { // Store persistent profile and progress data for a registered user
    std::string username; // Unique login name of the user
    std::string password; // Account password for authentication
    int xp = 0; // Total accumulated experience points
    int levelsCompleted = 0; // Count of completed lab levels
    int streak = 1; // Consecutive active learning days
    int lives = 5; // Remaining daily lives for quiz attempts
    std::string lastActiveDate = ""; // Date string of the user's most recent login
    std::string weakTopic = "Pointers & Syntax"; // Recommended topic needing extra practice
};

struct Flashcard { // Represent a single Theory lesson card
    std::string title; // Heading displayed at the top of the flashcard
    std::string content; // Body explanation text for the lesson
};

struct TheoryTopic { // Group related Theory flashcards under one topic
    std::string topicName; // Display name of the Theory topic
    std::vector<Flashcard> cards; // Ordered list of flashcards in this topic
};

struct QuizQuestion { // Represent one multiple-choice quiz question
    std::string question; // Question prompt text
    std::string options[4]; // Array of four possible answer choices
    int correctIndex; // Zero-based index of the correct choice in options
};

struct QuizTopic { // Group multiple quiz questions under one topic
    std::string topicName; // Display name of the Quiz topic
    std::vector<QuizQuestion> questions; // Collection of questions for this topic
};

struct ConstructorLevel { // Define a single drag-and-drop Code Constructor challenge
    std::string levelTitle; // Display title of the Constructor lab
    std::vector<std::string> correctLines; // Expected code lines in exact slot order
    std::vector<std::string> slotExplanations; // Explanations shown when each corresponding slot fails
    std::vector<std::string> extraBlocks; // Distractor code blocks mixed into the block pool
};

struct ConstructorTopic { // Group Code Constructor levels under one topic
    std::string topicName; // Display name of the Constructor topic
    std::vector<ConstructorLevel> levels; // List of Constructor labs belonging to this topic
};

struct DraggableBlock {
    std::string text;
    Rectangle rect = { 0, 0, 0, 0 };
    int currentSlot = -1;
    bool isDragging = false;
    Vector2 dragOffset = { 0, 0 };
};

struct CodeStep { // Pair a single line of code with its walkthrough explanation
    std::string codeLine; // Individual line of source code displayed in the Visualizer
    std::string explanation; // Plain-language explanation of what the line does
};

struct VisualizerLevel { // Define a single line-by-line Code Visualizer walkthrough
    std::string levelTitle; // Display title of the Visualizer lab
    std::string fullCode; // Complete source code shown in the intro overview modal
    std::vector<CodeStep> steps; // Sequential breakdown of code lines and explanations
};

struct VisualizerTopic { // Group Code Visualizer walkthroughs under one topic
    std::string topicName; // Display name of the Visualizer topic
    std::vector<VisualizerLevel> levels; // List of Visualizer levels in this topic
};

struct PracticeRule { // Define a validation rule checked against user-written Practice code
    std::string requiredToken; // Required code snippet matched while ignoring extra whitespace
    std::string errorFeedback; // Diagnostic message shown when the required token is missing
};

struct PracticeTask { // Define a single hands-on coding task in Practice mode
    std::string taskTitle; // Display title of the practice coding challenge
    std::string description; // Instructions explaining what the user must implement
    std::string expectedOutput; // Expected program output shown as a target reference
    std::vector<PracticeRule> rules; // List of validation rules used to grade submitted code
};

struct PracticeTopic { // Group Practice coding tasks under one topic
    std::string topicName; // Display name of the Practice topic
    std::vector<PracticeTask> tasks; // List of coding tasks belonging to this topic
};

enum class ExamQuestionType { // Identify the interaction format of an exam question
    MULTIPLE_CHOICE, // Question answered by selecting one of four options
    TEXT_INPUT // Question answered by typing a text response
};

struct ExamQuestion { // Represent a single question inside Exam mode
    ExamQuestionType type; // Format of the exam question
    std::string question; // Question prompt displayed to the user
    std::vector<std::string> options; // Four answer choices used when type is MULTIPLE_CHOICE
    int correctOptionIndex = 0; // Zero-based index of the correct option for MULTIPLE_CHOICE
    std::vector<std::string> acceptedAnswers; // List of valid accepted strings for TEXT_INPUT
    std::string correctAnswerDisplay; // Formatted canonical answer shown during exam review
};

struct ExamTopic { // Group exam questions under one subject topic
    std::string topicName; // Display name of the Exam topic
    std::vector<ExamQuestion> questions; // Ordered list of questions in this exam
};

struct AppState { // Hold all active runtime state across every screen and mode
    ScreenState currentScreen = ScreenState::AUTH_MENU; // Currently active screen being rendered
    TargetLang language = TargetLang::LANG_CPP; // Currently selected programming language curriculum

    std::vector<User> registeredUsers = { // In-memory list of user accounts initialized with default profiles
        {"Alice_Coder", "123", 450, 5, 7, 5, "2026-11-18", "STL & Vectors"}, // Default sample user Alice_Coder
        {"Bob_CPlusPlus", "123", 320, 3, 4, 4, "2026-11-18", "Memory Management"}, // Default sample user Bob_CPlusPlus
        {"Charlie_Dev", "123", 180, 2, 2, 5, "2026-11-18", "Control Flow"} // Default sample user Charlie_Dev
    };
    int currentUserIndex = -1; // Index of the signed-in user in registeredUsers or -1 when logged out

    char inputUsername[32] = "\0"; // Text buffer for entering a username on Login or Register screens
    char inputPassword[32] = "\0"; // Text buffer for entering a password on Login or Register screens
    std::string authErrorMsg = ""; // Error or warning message shown on auth and dashboard screens
    int activeInputBox = 0; // Focused input field selector (0 none, 1 username, 2 password)

    int selectedTopic = 0; // Index of the active Theory topic
    int currentCard = 0; // Index of the currently viewed flashcard within the Theory topic

    int selectedQuizTopic = 0; // Index of the active Quiz topic
    int currentQuestion = 0; // Index of the current question in the active quiz
    int score = 0; // Number of correctly answered questions in the current quiz attempt
    bool quizFinished = false; // Flag indicating whether all quiz questions have been completed
    int selectedAnswer = -1; // Index of the user's chosen quiz option or -1 if unanswered
    float feedbackTimer = 0.0f; // Countdown timer in seconds before advancing to the next quiz question

    int selectedConstructorTopic = 0; // Index of the active Code Constructor topic
    int selectedConstructorLevel = 0; // Index of the active Code Constructor level
    std::vector<DraggableBlock> blocks; // Active draggable code blocks for the current Constructor level
    std::vector<int> slotBlockIndex; // Mapping from each slot index to its occupying block index or -1
    bool isCarMoving = false; // Flag indicating whether the validation car animation is running
    float carX = 0.0f; // Current horizontal pixel coordinate of the validation car
    int carTargetLine = 0; // Count of consecutive code slots verified as correct by the car
    bool carCrashed = false; // Flag indicating whether the car stopped due to an error
    bool levelSuccess = false; // Flag indicating whether the user solved the Constructor level
    bool showErrorModal = false; // Flag controlling visibility of the Constructor crash report modal
    int firstErrorLine = -1; // Zero-based index of the first slot where the car crashed
    float errorModalScrollY = 0.0f; // Vertical scroll offset inside the Constructor error report modal

    int selectedVisualizerTopic = 0; // Index of the active Code Visualizer topic
    int selectedVisualizerLevel = 0; // Index of the active Code Visualizer level
    bool showIntroModal = true; // Flag controlling visibility of the Visualizer full-code overview modal
    int revealedLinesCount = 1; // Number of code lines currently revealed in the Visualizer
    std::vector<bool> explanationVisible; // Per-step flags tracking whether each explanation box is expanded
    std::vector<bool> stepUnderstood; // Per-step flags tracking whether the user confirmed understanding
    float visualizerScrollY = 0.0f; // Vertical scroll offset inside the Visualizer walkthrough viewport

    int selectedPracticeTopic = 0; // Index of the active Practice topic
    int selectedPracticeTask = 0; // Index of the active Practice coding task
    std::string practiceCodeBuffer = ""; // Multi-line source code typed by the user in the Practice editor
    bool practiceChecked = false; // Flag indicating whether the user has submitted code for checking
    bool practiceIsCorrect = false; // Flag indicating whether the checked Practice code passed all rules
    std::string practiceFeedback = ""; // Diagnostic feedback generated after checking Practice code
    float practiceEditorScrollY = 0.0f; // Vertical scroll offset for the Practice code editor area
    float practiceFeedbackScrollY = 0.0f; // Vertical scroll offset for the Practice feedback panel
    float backspaceHoldTimer = 0.0f; // Timer used to trigger smooth repeated deletion when holding Backspace

    int selectedExamTopic = 0; // Index of the active Exam topic
    int currentExamQuestion = 0; // Index of the currently displayed question in the Exam
    std::vector<int> examUserChoice; // Recorded multiple-choice option indices for each exam question
    std::vector<std::string> examUserText; // Recorded text input responses for each exam question
    int examScore = 0; // Total number of correct answers achieved in the current exam
    float examReviewScrollY = 0.0f; // Vertical scroll offset inside the Exam review screen

    // --- Profile View Runtime State ---
    bool showProfileView = false;          // Toggles right-side dashboard between Leaderboard and Profile
    bool showEditNameModal = false;        // Controls visibility of the Edit Username modal
    bool showDeleteAccountModal = false;   // Controls visibility of the Delete Account confirmation modal
    char editNameBuffer[32] = "\0";        // Input buffer for editing the username
    std::string profileErrorMsg = "";      // Validation error message inside the Edit Username modal
    float profileDetailsScrollY = 0.0f;    // Vertical scroll offset inside the Learning Progress details box
};

#endif // End of APP_TYPES_H include guard