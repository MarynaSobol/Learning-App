#pragma once
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
};

#endif // APP_TYPES_H