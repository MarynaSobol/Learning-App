#include "raylib.h"
#include "AppTypes.h"
#include "ContentData.h"
#include "UI.h"

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C / C++ Interactive Learning Studio");
    SetTargetFPS(60);

    AppState state;
    LanguageDatabase db = GetDatabase(state.language);

    // Kahoot-inspired distinct option colors
    Color kahootColors[4] = {
        { 226, 27, 60, 255 },  // Red
        { 19, 104, 206, 255 },  // Blue
        { 216, 158, 0, 255 },   // Yellow/Gold
        { 38, 137, 12, 255 }    // Green
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mouse = GetMousePosition();

        BeginDrawing();
        ClearBackground(Theme::BG_DARK);

        // Top Language Status Badge
        const char* activeLangText = (state.language == TargetLang::LANG_C) ? "Active Language: C" : "Active Language: C++";
        DrawRectangleRounded({ 20, 20, 230, 36 }, 0.3f, 6, Theme::PANEL_BG);
        DrawText(activeLangText, 35, 29, 18, Theme::WARNING_COL);

        // ====================================================================
        // SCREEN 1: MAIN MENU
        // ====================================================================
        if (state.currentScreen == ScreenState::MAIN_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 110, 42, Theme::TEXT_LIGHT);
            DrawCenteredText("Interactive Theory & Quiz Studio", 165, 20, Theme::TEXT_MUTED);

            // Settings button in top-right corner
            if (DrawButton({ (float)screenWidth - 170.0f, 20.0f, 150.0f, 42.0f }, "Settings", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::SETTINGS;
            }

            // 2 Centered Menu Buttons (Theory & Quiz)
            float btnW = 360.0f;
            float btnH = 64.0f;
            float centerX = (screenWidth - btnW) / 2.0f;
            float startY = 280.0f;
            float gap = 82.0f;

            if (DrawButton({ centerX, startY, btnW, btnH }, "Theory", Theme::BTN_PRIMARY, 26)) {
                state.currentScreen = ScreenState::THEORY_TOPICS;
            }
            if (DrawButton({ centerX, startY + gap, btnW, btnH }, "Quiz", Theme::BTN_PRIMARY, 26)) {
                state.currentQuestion = 0;
                state.score = 0;
                state.quizFinished = false;
                state.selectedAnswer = -1;
                state.feedbackTimer = 0.0f;
                state.currentScreen = ScreenState::QUIZ;
            }
        }

        // ====================================================================
        // SCREEN 2: SETTINGS
        // ====================================================================
        else if (state.currentScreen == ScreenState::SETTINGS) {
            DrawCenteredText("SETTINGS", 120, 38, Theme::TEXT_LIGHT);
            DrawCenteredText("Choose your target programming language:", 190, 22, Theme::TEXT_MUTED);

            Color cBtnColor = (state.language == TargetLang::LANG_C) ? Theme::SUCCESS_COL : Theme::BTN_SECONDARY;
            Color cppBtnColor = (state.language == TargetLang::LANG_CPP) ? Theme::SUCCESS_COL : Theme::BTN_SECONDARY;

            if (DrawButton({ 360, 270, 220, 80 }, "C Language", cBtnColor, 24)) {
                state.language = TargetLang::LANG_C;
                db = GetDatabase(state.language);
            }
            if (DrawButton({ 620, 270, 220, 80 }, "C++ Language", cppBtnColor, 24)) {
                state.language = TargetLang::LANG_CPP;
                db = GetDatabase(state.language);
            }

            DrawCenteredText("All theory topics and quizzes automatically adapt", 410, 18, Theme::TEXT_MUTED);
            DrawCenteredText("to your selected language.", 435, 18, Theme::TEXT_MUTED);

            if (DrawButton({ 480, 690, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 3: THEORY TOPIC SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::THEORY_TOPICS) {
            DrawCenteredText("SELECT A THEORY TOPIC", 90, 34, Theme::TEXT_LIGHT);

            for (size_t i = 0; i < db.theoryTopics.size(); ++i) {
                Rectangle btnRect = { 350.0f, 190.0f + (float)(i * 85.0f), 500.0f, 65.0f };
                if (DrawButton(btnRect, db.theoryTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 24)) {
                    state.selectedTopic = (int)i;
                    state.currentCard = 0;
                    state.currentScreen = ScreenState::THEORY_CARDS;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 4: THEORY FLASHCARDS
        // ====================================================================
        else if (state.currentScreen == ScreenState::THEORY_CARDS) {
            const auto& topic = db.theoryTopics[state.selectedTopic];
            const auto& card = topic.cards[state.currentCard];

            DrawCenteredText(topic.topicName.c_str(), 70, 28, Theme::TEXT_MUTED);

            // Flashcard Container
            Rectangle cardRect = { 220, 130, 760, 420 };
            DrawRectangleRounded(cardRect, 0.06f, 10, Theme::PANEL_BG);
            DrawRectangleRoundedLines(cardRect, 0.06f, 10, Theme::BTN_PRIMARY);

            // Card Counter & Title
            std::string progress = TextFormat("Card %d of %d", state.currentCard + 1, (int)topic.cards.size());
            DrawText(progress.c_str(), 250, 155, 18, Theme::WARNING_COL);
            DrawCenteredText(card.title.c_str(), 195, 32, Theme::TEXT_LIGHT);
            DrawLine(260, 245, 940, 245, Theme::BTN_SECONDARY);

            // Card Body Text
            DrawTextWrapped(card.content, { 260, 275, 680, 240 }, 24, Theme::TEXT_LIGHT);

            // Navigation Controls
            if (state.currentCard > 0) {
                if (DrawButton({ 220, 580, 180, 50 }, "< Previous", Theme::BTN_PRIMARY, 20)) {
                    state.currentCard--;
                }
            }
            if (state.currentCard < (int)topic.cards.size() - 1) {
                if (DrawButton({ 800, 580, 180, 50 }, "Next >", Theme::BTN_PRIMARY, 20)) {
                    state.currentCard++;
                }
            }
            else {
                if (DrawButton({ 800, 580, 180, 50 }, "Finish Topic", Theme::SUCCESS_COL, 20)) {
                    state.currentScreen = ScreenState::THEORY_TOPICS;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 5: QUIZ (KAHOOT STYLE)
        // ====================================================================
        else if (state.currentScreen == ScreenState::QUIZ) {
            if (!state.quizFinished) {
                const auto& q = db.quizQuestions[state.currentQuestion];

                std::string header = TextFormat("QUESTION %d OF %d", state.currentQuestion + 1, (int)db.quizQuestions.size());
                DrawCenteredText(header.c_str(), 75, 24, Theme::WARNING_COL);

                // Question Display Box
                Rectangle qBox = { 150, 120, 900, 160 };
                DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG);
                DrawTextWrapped(q.question, { 180, 165, 840, 100 }, 28, Theme::TEXT_LIGHT);

                // 2x2 Kahoot-style Answer Grid
                for (int i = 0; i < 4; ++i) {
                    int col = i % 2;
                    int row = i / 2;
                    Rectangle optRect = { 150.0f + (float)(col * 460.0f), 320.0f + (float)(row * 145.0f), 440.0f, 125.0f };

                    Color btnCol = kahootColors[i];
                    if (state.selectedAnswer != -1) {
                        if (i == q.correctIndex) btnCol = Theme::SUCCESS_COL;
                        else if (i == state.selectedAnswer) btnCol = Theme::ERROR_COL;
                        else btnCol = Theme::BTN_SECONDARY;
                    }

                    if (DrawButton(optRect, q.options[i].c_str(), btnCol, 24) && state.selectedAnswer == -1) {
                        state.selectedAnswer = i;
                        if (i == q.correctIndex) state.score++;
                        state.feedbackTimer = 1.4f;
                    }
                }

                // Advance after brief feedback pause
                if (state.selectedAnswer != -1) {
                    state.feedbackTimer -= dt;
                    const char* statusMsg = (state.selectedAnswer == q.correctIndex) ? "Correct!" : "Incorrect!";
                    Color statusCol = (state.selectedAnswer == q.correctIndex) ? Theme::SUCCESS_COL : Theme::ERROR_COL;
                    DrawCenteredText(statusMsg, 630, 30, statusCol);

                    if (state.feedbackTimer <= 0.0f) {
                        state.selectedAnswer = -1;
                        state.currentQuestion++;
                        if (state.currentQuestion >= (int)db.quizQuestions.size()) {
                            state.quizFinished = true;
                        }
                    }
                }
            }
            else {
                // Quiz Summary View
                DrawCenteredText("QUIZ COMPLETED!", 180, 44, Theme::SUCCESS_COL);
                std::string scoreStr = TextFormat("Your Final Score: %d / %d", state.score, (int)db.quizQuestions.size());
                DrawCenteredText(scoreStr.c_str(), 260, 32, Theme::TEXT_LIGHT);

                if (DrawButton({ 480, 380, 240, 55 }, "Retry Quiz", Theme::BTN_PRIMARY, 22)) {
                    state.currentQuestion = 0;
                    state.score = 0;
                    state.quizFinished = false;
                    state.selectedAnswer = -1;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}