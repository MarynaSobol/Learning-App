#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "raylib.h"
#include <string>
#include <vector>

// Target learning language selectable in Sidebar
enum class TargetLang {
    LANG_C,
    LANG_CPP
};

// Application screens
enum class ScreenState {
    AUTH_MENU,
    LOGIN,
    REGISTER,
    MAIN_MENU,
    LEADERBOARD,
    SETTINGS,
    THEORY_TOPICS,
    THEORY_CARDS,
    QUIZ_TOPICS,
    QUIZ,
    CONSTRUCTOR_TOPICS,
    CONSTRUCTOR_LEVELS,
    CONSTRUCTOR_GAME,
    VISUALIZER_TOPICS,
    VISUALIZER_LEVELS,
    VISUALIZER_VIEW
};

// --- USER & LEADERBOARD STRUCTURES ---
struct User {
    std::string username;
    std::string password;
    int xp = 0;
    int levelsCompleted = 0;
    int streak = 1;                              // Consecutive days played
    int lives = 5;                               // Daily lives (max 5 per day)
    std::string lastActiveDate = "";             // Format: YYYY-MM-DD
    std::string weakTopic = "Pointers & Syntax"; // Area recommended for practice
};

// --- THEORY STRUCTURES ---
struct Flashcard {
    std::string title;
    std::string content;
};

struct TheoryTopic {
    std::string topicName;
    std::vector<Flashcard> cards;
};

// --- QUIZ STRUCTURES (Topic-based, matching Theory topics) ---
struct QuizQuestion {
    std::string question;
    std::string options[4];
    int correctIndex;
};

struct QuizTopic {
    std::string topicName;
    std::vector<QuizQuestion> questions;
};

// --- CONSTRUCTOR STRUCTURES (Topic-based) ---
struct ConstructorLevel {
    std::string levelTitle;
    std::vector<std::string> correctLines;
    std::vector<std::string> slotExplanations;
    std::vector<std::string> extraBlocks;
};

struct ConstructorTopic {
    std::string topicName;
    std::vector<ConstructorLevel> levels;
};

struct DraggableBlock {
    std::string text;
    Rectangle rect;
    int currentSlot;
    bool isDragging;
    Vector2 dragOffset;
};

// --- VISUALIZER STRUCTURES (Topic-based) ---
struct CodeStep {
    std::string codeLine;
    std::string explanation;
};

struct VisualizerLevel {
    std::string levelTitle;
    std::string fullCode;
    std::vector<CodeStep> steps;
};

struct VisualizerTopic {
    std::string topicName;
    std::vector<VisualizerLevel> levels;
};

// --- GLOBAL APPLICATION STATE ---
struct AppState {
    ScreenState currentScreen = ScreenState::AUTH_MENU;
    TargetLang language = TargetLang::LANG_CPP;

    std::vector<User> registeredUsers = {
        {"Alice_Coder", "123", 450, 5, 7, 5, "2026-11-18", "STL & Vectors"},
        {"Bob_CPlusPlus", "123", 320, 3, 4, 4, "2026-11-18", "Memory Management"},
        {"Charlie_Dev", "123", 180, 2, 2, 5, "2026-11-18", "Control Flow"}
    };
    int currentUserIndex = -1;

    // Text input buffers for Login / Register screens
    char inputUsername[32] = "\0";
    char inputPassword[32] = "\0";
    std::string authErrorMsg = "";
    int activeInputBox = 0;

    // Theory State
    int selectedTopic = 0;
    int currentCard = 0;

    // Quiz State
    int selectedQuizTopic = 0;
    int currentQuestion = 0;
    int score = 0;
    bool quizFinished = false;
    int selectedAnswer = -1;
    float feedbackTimer = 0.0f;

    // Constructor State
    int selectedConstructorTopic = 0;
    int selectedConstructorLevel = 0;
    std::vector<DraggableBlock> blocks;
    std::vector<int> slotBlockIndex;
    bool isCarMoving = false;
    float carX = 0.0f;
    int carTargetLine = 0;
    bool carCrashed = false;
    bool levelSuccess = false;
    bool showErrorModal = false;
    int firstErrorLine = -1;
    float errorModalScrollY = 0.0f;

    // Visualizer State
    int selectedVisualizerTopic = 0;
    int selectedVisualizerLevel = 0;
    bool showIntroModal = true;
    int revealedLinesCount = 1;
    std::vector<bool> explanationVisible;
    std::vector<bool> stepUnderstood;
    float visualizerScrollY = 0.0f;
};

#endif // APP_TYPES_H
