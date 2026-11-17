#include "raylib.h"
#include "AppTypes.h"
#include "ContentData.h"
#include "UI.h"
#include <algorithm>
#include <random>

// Initializes a Constructor level by creating and shuffling draggable blocks
void InitConstructorLevel(AppState& state, const ConstructorLevel& level) {
    state.blocks.clear();
    int numSlots = (int)level.correctLines.size();
    state.slotBlockIndex.assign(numSlots, -1);

    std::vector<std::string> allTexts = level.correctLines;
    for (const auto& extra : level.extraBlocks) {
        allTexts.push_back(extra);
    }

    // Deterministic shuffle so blocks are mixed in the bottom pool
    std::mt19937 rng(1337 + state.selectedConstructorLevel);
    std::shuffle(allTexts.begin(), allTexts.end(), rng);

    // Arrange blocks in a 2-column grid at the bottom of the screen
    int startX = 140;
    int startY = 485;
    int colWidth = 460;
    int rowHeight = 46;

    for (size_t i = 0; i < allTexts.size(); ++i) {
        DraggableBlock b;
        b.text = allTexts[i];
        int col = (int)(i % 2);
        int row = (int)(i / 2);
        b.rect = { (float)(startX + col * (colWidth + 30)), (float)(startY + row * rowHeight), (float)colWidth, 38.0f };
        b.currentSlot = -1;
        b.isDragging = false;
        b.dragOffset = { 0, 0 };
        state.blocks.push_back(b);
    }

    state.isCarMoving = false;
    state.carX = 140.0f;
    state.carTargetLine = 0;
    state.carCrashed = false;
    state.levelSuccess = false;
    state.showErrorModal = false;
    state.firstErrorLine = -1;
}

// Initializes a Visualizer level
void InitVisualizerLevel(AppState& state, const VisualizerLevel& level) {
    state.showIntroModal = true;
    state.revealedLinesCount = 1;
    state.explanationVisible.assign(level.steps.size(), false);
    state.stepUnderstood.assign(level.steps.size(), false);
}

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C / C++ Interactive Learning Studio");
    InitAppFont();
    SetTargetFPS(60);

    AppState state;
    LanguageDatabase db = GetDatabase(state.language);

    // Kahoot-inspired distinct option colors
    Color kahootColors[4] = {
        { 226, 27, 60, 255 },   // Red
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
        DrawCustomText(activeLangText, 35, 29, 18, Theme::WARNING_COL);

        // ====================================================================
        // SCREEN 1: MAIN MENU
        // ====================================================================
        if (state.currentScreen == ScreenState::MAIN_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 110, 42, Theme::TEXT_LIGHT);
            DrawCenteredText("Interactive Theory, Quiz, Code Constructor & Visualizer", 165, 20, Theme::TEXT_MUTED);

            // Settings button in top-right corner
            if (DrawButton({ screenWidth - 170.0f, 20.0f, 150.0f, 42.0f }, "Settings", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::SETTINGS;
            }

            // 4 Centered Menu Buttons
            float btnW = 360.0f;
            float btnH = 64.0f;
            float centerX = (screenWidth - btnW) / 2.0f;
            float startY = 250.0f;
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
            if (DrawButton({ centerX, startY + gap * 2, btnW, btnH }, "Constructor", Theme::BTN_PRIMARY, 26)) {
                state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
            }
            if (DrawButton({ centerX, startY + gap * 3, btnW, btnH }, "Visualizer", Theme::BTN_PRIMARY, 26)) {
                state.currentScreen = ScreenState::VISUALIZER_LEVELS;
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

            DrawCenteredText("All theory topics, quizzes, constructor puzzles, and visualizer code", 410, 18, Theme::TEXT_MUTED);
            DrawCenteredText("automatically adapt to your selected language.", 435, 18, Theme::TEXT_MUTED);

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
                Rectangle btnRect = { 350.0f, 190.0f + i * 85.0f, 500.0f, 65.0f };
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
            DrawCustomText(progress.c_str(), 250, 155, 18, Theme::WARNING_COL);
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
                    Rectangle optRect = { 150.0f + col * 460.0f, 320.0f + row * 145.0f, 440.0f, 125.0f };

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

        // ====================================================================
        // SCREEN 6: CONSTRUCTOR LEVEL SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::CONSTRUCTOR_LEVELS) {
            DrawCenteredText("CODE CONSTRUCTOR - SELECT LEVEL", 90, 34, Theme::TEXT_LIGHT);
            DrawCenteredText("Drag and drop code blocks into the right order and watch the car inspect your code!", 140, 20, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.constructorLevels.size(); ++i) {
                Rectangle btnRect = { 350.0f, 220.0f + i * 85.0f, 500.0f, 65.0f };
                if (DrawButton(btnRect, db.constructorLevels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 24)) {
                    state.selectedConstructorLevel = (int)i;
                    InitConstructorLevel(state, db.constructorLevels[i]);
                    state.currentScreen = ScreenState::CONSTRUCTOR_GAME;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 7: CONSTRUCTOR GAME (DRAG & DROP + CAR ANIMATION)
        // ====================================================================
        else if (state.currentScreen == ScreenState::CONSTRUCTOR_GAME) {
            const auto& level = db.constructorLevels[state.selectedConstructorLevel];
            int numSlots = (int)level.correctLines.size();

            DrawCenteredText(level.levelTitle.c_str(), 25, 28, Theme::TEXT_LIGHT);
            DrawCenteredText("Drag code blocks from the bottom pool into the empty slots:", 60, 18, Theme::TEXT_MUTED);

            // Define Target Empty Slots in the middle of the screen
            std::vector<Rectangle> slotRects(numSlots);
            for (int i = 0; i < numSlots; ++i) {
                slotRects[i] = { 240.0f, 135.0f + i * 48.0f, 720.0f, 40.0f };
            }

            // Car Track & Animated Car above the code slots
            DrawLine(200, 122, 1000, 122, Theme::BTN_SECONDARY);
            DrawCar(state.carX, 82, state.carCrashed);

            // Update Car Inspection Animation
            if (state.isCarMoving && !state.carCrashed && !state.levelSuccess) {
                state.carX += 260.0f * dt;
                float stepWidth = 720.0f / (float)numSlots;
                int currentInspectSlot = (int)((state.carX - 200.0f) / stepWidth);

                if (currentInspectSlot >= 0 && currentInspectSlot < numSlots) {
                    int bIdx = state.slotBlockIndex[currentInspectSlot];
                    bool slotCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[currentInspectSlot]);

                    if (!slotCorrect) {
                        state.carCrashed = true;
                        state.isCarMoving = false;
                        state.firstErrorLine = currentInspectSlot;
                        state.showErrorModal = true;
                    }
                    else {
                        state.carTargetLine = currentInspectSlot + 1;
                    }
                }
                else if (currentInspectSlot >= numSlots) {
                    state.levelSuccess = true;
                    state.isCarMoving = false;
                    state.carTargetLine = numSlots;
                }
            }

            // Draw Target Slots
            for (int i = 0; i < numSlots; ++i) {
                Color slotCol = Theme::PANEL_BG;
                if (state.carTargetLine > i) {
                    slotCol = Theme::SUCCESS_COL; // Line verified by car -> turns green!
                }
                else if (state.carCrashed) {
                    int bIdx = state.slotBlockIndex[i];
                    bool isCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[i]);
                    if (!isCorrect) slotCol = Theme::ERROR_COL; // All wrong blocks turn red on crash!
                }

                DrawRectangleRounded(slotRects[i], 0.2f, 6, slotCol);
                DrawRectangleRoundedLines(slotRects[i], 0.2f, 6, Theme::BTN_SECONDARY);
                DrawCustomText(TextFormat("%d.", i + 1), (int)slotRects[i].x - 35, (int)slotRects[i].y + 10, 20, Theme::TEXT_MUTED);
            }

            // Bottom Pool Divider
            DrawLine(100, 465, 1100, 465, Theme::BTN_SECONDARY);
            DrawCustomText("Available Code Blocks:", 140, 442, 18, Theme::TEXT_MUTED);

            // Handle Drag & Drop Interaction (disabled while car is driving or modal is open)
            bool interactionLocked = state.isCarMoving || state.showErrorModal || state.levelSuccess;
            if (!interactionLocked) {
                // Start dragging on click
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    for (int i = (int)state.blocks.size() - 1; i >= 0; --i) {
                        if (CheckCollisionPointRec(mouse, state.blocks[i].rect)) {
                            state.blocks[i].isDragging = true;
                            state.blocks[i].dragOffset = { mouse.x - state.blocks[i].rect.x, mouse.y - state.blocks[i].rect.y };
                            if (state.blocks[i].currentSlot != -1) {
                                state.slotBlockIndex[state.blocks[i].currentSlot] = -1;
                                state.blocks[i].currentSlot = -1;
                            }
                            break;
                        }
                    }
                }

                // Update dragged block position
                for (auto& b : state.blocks) {
                    if (b.isDragging) {
                        b.rect.x = mouse.x - b.dragOffset.x;
                        b.rect.y = mouse.y - b.dragOffset.y;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            b.isDragging = false;
                            // Snap into slot if dropped over an empty slot
                            for (int s = 0; s < numSlots; ++s) {
                                if (CheckCollisionPointRec(mouse, slotRects[s]) && state.slotBlockIndex[s] == -1) {
                                    b.currentSlot = s;
                                    b.rect.x = slotRects[s].x + 10;
                                    b.rect.y = slotRects[s].y + 1;
                                    state.slotBlockIndex[s] = (int)(&b - &state.blocks[0]);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Draw Draggable Code Blocks
            for (size_t i = 0; i < state.blocks.size(); ++i) {
                const auto& b = state.blocks[i];
                Color blockColor = Theme::BTN_PRIMARY;

                if (b.currentSlot != -1) {
                    if (state.carTargetLine > b.currentSlot) {
                        blockColor = Theme::SUCCESS_COL;
                    }
                    else if (state.carCrashed && b.text != level.correctLines[b.currentSlot]) {
                        blockColor = Theme::ERROR_COL;
                    }
                }

                DrawRectangleRounded(b.rect, 0.2f, 6, blockColor);
                DrawRectangleRoundedLines(b.rect, 0.2f, 6, Theme::TEXT_LIGHT);
                DrawCustomText(b.text.c_str(), (int)b.rect.x + 14, (int)b.rect.y + 9, 20, Theme::TEXT_LIGHT);
            }

            // Bottom Action Buttons
            if (!state.isCarMoving && !state.levelSuccess && !state.showErrorModal) {
                if (DrawButton({ 740, 720, 220, 50 }, "Ready (Run Car)", Theme::SUCCESS_COL, 20)) {
                    state.carX = 200.0f;
                    state.carTargetLine = 0;
                    state.carCrashed = false;
                    state.isCarMoving = true;
                }
            }

            if (DrawButton({ 240, 720, 220, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }

            // Success Banner
            if (state.levelSuccess) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 185 });
                Rectangle modal = { 280, 220, 640, 320 };
                DrawRectangleRounded(modal, 0.08f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.08f, 10, Theme::SUCCESS_COL);

                DrawCenteredText("CONGRATULATIONS!", 265, 36, Theme::SUCCESS_COL);
                DrawCenteredText("The car passed every line! Your code is 100% correct.", 330, 22, Theme::TEXT_LIGHT);

                if (DrawButton({ 340, 440, 230, 50 }, "Play Again", Theme::BTN_PRIMARY, 20)) {
                    InitConstructorLevel(state, level);
                }
                if (DrawButton({ 630, 440, 230, 50 }, "Main Menu", Theme::BTN_SECONDARY, 20)) {
                    state.currentScreen = ScreenState::MAIN_MENU;
                }
            }

            // Crash / Error Modal with detailed explanations for every mistake
            if (state.showErrorModal) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 195 });
                Rectangle modal = { 140, 90, 920, 620 };
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::ERROR_COL);

                DrawCenteredText("CAR CRASHED! CODE ERRORS DETECTED", 115, 30, Theme::ERROR_COL);
                std::string crashInfo = TextFormat("First breakdown occurred at Line %d. Review your mistakes below:", state.firstErrorLine + 1);
                DrawCenteredText(crashInfo.c_str(), 155, 20, Theme::WARNING_COL);

                std::string errorReport = "";
                for (int s = 0; s < numSlots; ++s) {
                    int bIdx = state.slotBlockIndex[s];
                    std::string userCode = (bIdx == -1) ? "[Empty Slot]" : state.blocks[bIdx].text;
                    if (userCode != level.correctLines[s]) {
                        errorReport += TextFormat("Line %d:\n  Your block: %s\n  Correct block: %s\n  Why: %s\n\n",
                            s + 1, userCode.c_str(), level.correctLines[s].c_str(), level.slotExplanations[s].c_str());
                    }
                }

                DrawTextWrapped(errorReport, { 180, 195, 840, 410 }, 18, Theme::TEXT_LIGHT);

                if (DrawButton({ 330, 635, 240, 50 }, "Retry Level", Theme::BTN_PRIMARY, 20)) {
                    InitConstructorLevel(state, level);
                }
                if (DrawButton({ 630, 635, 240, 50 }, "Main Menu", Theme::BTN_SECONDARY, 20)) {
                    state.currentScreen = ScreenState::MAIN_MENU;
                }
            }
        }

        // ====================================================================
        // SCREEN 8: VISUALIZER LEVEL SELECTION
        // ====================================================================
        else if (state.currentScreen == ScreenState::VISUALIZER_LEVELS) {
            DrawCenteredText("CODE VISUALIZER - SELECT LEVEL", 90, 34, Theme::TEXT_LIGHT);
            DrawCenteredText("Inspect code execution line-by-line with interactive explanations", 140, 20, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.visualizerLevels.size(); ++i) {
                Rectangle btnRect = { 350.0f, 220.0f + i * 85.0f, 500.0f, 65.0f };
                if (DrawButton(btnRect, db.visualizerLevels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 24)) {
                    state.selectedVisualizerLevel = (int)i;
                    InitVisualizerLevel(state, db.visualizerLevels[i]);
                    state.currentScreen = ScreenState::VISUALIZER_VIEW;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 9: VISUALIZER STEP-BY-STEP VIEW
        // ====================================================================
        else if (state.currentScreen == ScreenState::VISUALIZER_VIEW) {
            const auto& level = db.visualizerLevels[state.selectedVisualizerLevel];
            DrawCenteredText(level.levelTitle.c_str(), 30, 30, Theme::TEXT_LIGHT);

            // Step-by-step interactive code view
            float curY = 85.0f;
            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) {
                // Code Line Box
                Rectangle lineBox = { 140.0f, curY, 720.0f, 38.0f };
                DrawRectangleRounded(lineBox, 0.2f, 6, Theme::PANEL_BG);
                DrawRectangleRoundedLines(lineBox, 0.2f, 6, Theme::BTN_PRIMARY);
                DrawCustomText(level.steps[i].codeLine.c_str(), 160, (int)curY + 9, 20, Theme::TEXT_LIGHT);

                // Toggle button if already understood (allows collapsing/expanding at will)
                if (state.stepUnderstood[i]) {
                    const char* toggleLabel = state.explanationVisible[i] ? "Hide Info" : "Show Info";
                    if (DrawButton({ 880.0f, curY, 160.0f, 38.0f }, toggleLabel, Theme::BTN_SECONDARY, 18)) {
                        state.explanationVisible[i] = !state.explanationVisible[i];
                    }
                }

                curY += 44.0f;

                // If user hasn't clicked "Explanation" yet for the active line
                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) {
                    if (DrawButton({ 160.0f, curY, 190.0f, 36.0f }, "Explanation", Theme::WARNING_COL, 18)) {
                        state.explanationVisible[i] = true;
                    }
                    curY += 44.0f;
                }

                // Explanation Panel
                if (state.explanationVisible[i]) {
                    Rectangle expBox = { 160.0f, curY, 700.0f, 52.0f };
                    DrawRectangleRounded(expBox, 0.2f, 6, { 30, 58, 50, 255 });
                    DrawRectangleRoundedLines(expBox, 0.2f, 6, Theme::SUCCESS_COL);
                    DrawTextWrapped(level.steps[i].explanation, { 175.0f, curY + 8.0f, 670.0f, 40.0f }, 18, Theme::TEXT_LIGHT);
                    curY += 58.0f;

                    // "Got It" button under the explanation unlocks the next line of code
                    if (!state.stepUnderstood[i]) {
                        if (DrawButton({ 160.0f, curY, 160.0f, 36.0f }, "Got It!", Theme::SUCCESS_COL, 18)) {
                            state.stepUnderstood[i] = true;
                            state.explanationVisible[i] = false; // Auto-collapse to keep view clean (can be re-opened via Show Info)
                            if (state.revealedLinesCount < (int)level.steps.size()) {
                                state.revealedLinesCount++;
                            }
                        }
                        curY += 44.0f;
                    }
                }
            }

            if (state.stepUnderstood.back()) {
                DrawCenteredText("All lines analyzed! You can toggle any explanation above or return to the menu.", 675, 20, Theme::SUCCESS_COL);
            }

            if (DrawButton({ 480, 720, 240, 50 }, "Exit to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }

            // Intro Notification Modal showing the full chapter code first
            if (state.showIntroModal) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 205 });
                Rectangle modal = { 220, 110, 760, 560 };
                DrawRectangleRounded(modal, 0.06f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.06f, 10, Theme::BTN_PRIMARY);

                DrawCenteredText("CODE OVERVIEW", 140, 32, Theme::WARNING_COL);
                DrawCenteredText("Here is the complete code we are going to break down now:", 185, 20, Theme::TEXT_LIGHT);

                Rectangle codeBox = { 270, 225, 660, 330 };
                DrawRectangleRounded(codeBox, 0.05f, 8, Theme::BG_DARK);
                DrawTextWrapped(level.fullCode, { 295, 245, 610, 290 }, 22, Theme::SUCCESS_COL);

                if (DrawButton({ 320, 585, 220, 50 }, "Cancel (Main Menu)", Theme::ERROR_COL, 20)) {
                    state.currentScreen = ScreenState::MAIN_MENU;
                }
                if (DrawButton({ 660, 585, 220, 50 }, "Let's Go!", Theme::SUCCESS_COL, 20)) {
                    state.showIntroModal = false;
                }
            }
        }

        EndDrawing();
    }

    UnloadAppFont();
    CloseWindow();
    return 0;
}