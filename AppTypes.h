#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "raylib.h"
#include <string>
#include <vector>

// Target learning language selectable in Settings
enum class TargetLang {
    LANG_C,
    LANG_CPP
};

// Application screens
enum class ScreenState {
    MAIN_MENU,
    SETTINGS,
    THEORY_TOPICS,
    THEORY_CARDS,
    QUIZ,
    CONSTRUCTOR_LEVELS,
    CONSTRUCTOR_GAME,
    VISUALIZER_LEVELS,
    VISUALIZER_VIEW
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

// --- QUIZ STRUCTURES ---
struct QuizQuestion {
    std::string question;
    std::string options[4];
    int correctIndex;
};

// --- CONSTRUCTOR STRUCTURES ---
struct ConstructorLevel {
    std::string levelTitle;
    std::vector<std::string> correctLines;
    std::vector<std::string> slotExplanations;
    std::vector<std::string> extraBlocks;
};

struct DraggableBlock {
    std::string text;
    Rectangle rect;
    int currentSlot; // -1 if in pool, 0..N-1 if placed in a slot
    bool isDragging;
    Vector2 dragOffset;
};

// --- VISUALIZER STRUCTURES ---
struct CodeStep {
    std::string codeLine;
    std::string explanation;
};

struct VisualizerLevel {
    std::string levelTitle;
    std::string fullCode;
    std::vector<CodeStep> steps;
};

// --- GLOBAL APPLICATION STATE ---
struct AppState {
    ScreenState currentScreen = ScreenState::MAIN_MENU;
    TargetLang language = TargetLang::LANG_CPP;

    // Theory State
    int selectedTopic = 0;
    int currentCard = 0;

    // Quiz State
    int currentQuestion = 0;
    int score = 0;
    bool quizFinished = false;
    int selectedAnswer = -1;
    float feedbackTimer = 0.0f;

    // Constructor State
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

    // Visualizer State
    int selectedVisualizerLevel = 0;
    bool showIntroModal = true;
    int revealedLinesCount = 1;
    std::vector<bool> explanationVisible;
    std::vector<bool> stepUnderstood;
};

#endif // APP_TYPES_H