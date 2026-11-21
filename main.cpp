#include "raylib.h"
#include "AppTypes.h"
#include "ContentData.h"
#include "UI.h"
#include <algorithm>
#include <random>
#include <cstring>
#include <fstream>
#include <sstream>
#include <ctime>

// Returns current date as YYYY-MM-DD
std::string GetTodayDate() {
    std::time_t t = std::time(nullptr);
    std::tm tm_now{};
#ifdef _WIN32
    localtime_s(&tm_now, &t);
#else
    localtime_r(&t, &tm_now);
#endif
    char buf[16];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d", &tm_now);
    return std::string(buf);
}

// Updates user's daily streak and resets daily lives (5 per day) on a new day
void SyncUserDailyStatus(User& user) {
    std::string today = GetTodayDate();
    if (user.lastActiveDate != today) {
        user.lives = 5;
        if (!user.lastActiveDate.empty()) {
            user.streak += 1;
        }
        else {
            user.streak = 1;
        }
        user.lastActiveDate = today;
    }
    if (user.weakTopic.empty()) {
        user.weakTopic = "Pointers & Syntax";
    }
}

std::vector<User> LoadUsers() {
    std::vector<User> users;
    std::ifstream file("users.txt");
    std::string today = GetTodayDate();

    if (!file.is_open()) {
        return {
            {"Alice_Coder", "123", 450, 5, 7, 5, today, "STL & Vectors"},
            {"Bob_CPlusPlus", "123", 320, 3, 4, 4, today, "Memory Management"},
            {"Charlie_Dev", "123", 180, 2, 2, 5, today, "Control Flow"}
        };
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string username, password, lastDate, weakTopic;
        int xp = 0, levels = 0, streak = 1, lives = 5;

        if (std::getline(ss, username, '|') &&
            std::getline(ss, password, '|') &&
            (ss >> xp) && (ss.ignore()) &&
            (ss >> levels)) {
            if (ss.peek() == '|') {
                ss.ignore();
                ss >> streak;
                if (ss.peek() == '|') {
                    ss.ignore();
                    ss >> lives;
                    if (ss.peek() == '|') {
                        ss.ignore();
                        std::getline(ss, lastDate, '|');
                        std::getline(ss, weakTopic);
                    }
                }
            }
            if (lastDate.empty()) lastDate = today;
            if (weakTopic.empty()) weakTopic = "Pointers & Syntax";
            users.push_back({ username, password, xp, levels, streak, lives, lastDate, weakTopic });
        }
    }
    file.close();

    if (users.empty()) {
        users = {
            {"Alice_Coder", "123", 450, 5, 7, 5, today, "STL & Vectors"},
            {"Bob_CPlusPlus", "123", 320, 3, 4, 4, today, "Memory Management"},
            {"Charlie_Dev", "123", 180, 2, 2, 5, today, "Control Flow"}
        };
    }
    return users;
}

void SaveUsers(const std::vector<User>& users) {
    std::ofstream file("users.txt");
    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.username << "|"
                << user.password << "|"
                << user.xp << "|"
                << user.levelsCompleted << "|"
                << user.streak << "|"
                << user.lives << "|"
                << user.lastActiveDate << "|"
                << user.weakTopic << "\n";
        }
        file.close();
    }
}

void InitConstructorLevel(AppState& state, const ConstructorLevel& level) {
    state.blocks.clear();
    int numSlots = (int)level.correctLines.size();
    state.slotBlockIndex.assign(numSlots, -1);

    std::vector<std::string> allTexts = level.correctLines;
    for (const auto& extra : level.extraBlocks) {
        allTexts.push_back(extra);
    }

    std::mt19937 rng(1337 + state.selectedConstructorTopic * 31 + state.selectedConstructorLevel);
    std::shuffle(allTexts.begin(), allTexts.end(), rng);

    // Cleanly positioned bottom pool (no overlap with slots or bottom buttons)
    int startX = 65;
    int startY = 480;
    int colWidth = 520;
    int colGap = 30;
    int rowHeight = 46;

    for (size_t i = 0; i < allTexts.size(); ++i) {
        DraggableBlock b;
        b.text = allTexts[i];
        int col = (int)(i % 2);
        int row = (int)(i / 2);
        b.rect = { (float)(startX + col * (colWidth + colGap)), (float)(startY + row * rowHeight), (float)colWidth, 36.0f };
        b.currentSlot = -1;
        b.isDragging = false;
        b.dragOffset = { 0, 0 };
        state.blocks.push_back(b);
    }

    state.isCarMoving = false;
    state.carX = 180.0f;
    state.carTargetLine = 0;
    state.carCrashed = false;
    state.levelSuccess = false;
    state.showErrorModal = false;
    state.firstErrorLine = -1;
    state.errorModalScrollY = 0.0f;
}

void InitVisualizerLevel(AppState& state, const VisualizerLevel& level) {
    state.showIntroModal = true;
    state.revealedLinesCount = 1;
    state.explanationVisible.assign(level.steps.size(), false);
    state.stepUnderstood.assign(level.steps.size(), false);
    state.visualizerScrollY = 0.0f;
}

void UpdateTextInput(char* buffer, int maxLen) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125)) {
            int currentLen = (int)strlen(buffer);
            if (currentLen < maxLen - 1) {
                buffer[currentLen] = (char)key;
                buffer[currentLen + 1] = '\0';
            }
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        int currentLen = (int)strlen(buffer);
        if (currentLen > 0) {
            buffer[currentLen - 1] = '\0';
        }
    }
}

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C / C++ Interactive Learning Studio");
    InitAppFont();
    SetTargetFPS(60);

    AppState state;
    state.registeredUsers = LoadUsers();
    LanguageDatabase db = GetDatabase(state.language);

    Color kahootColors[4] = {
        { 226, 27, 60, 255 },
        { 19, 104, 206, 255 },
        { 216, 158, 0, 255 },
        { 38, 137, 12, 255 }
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mouse = GetMousePosition();
        ScreenState frameScreen = state.currentScreen;

        BeginDrawing();
        ClearBackground(Theme::BG_DARK);

        // ====================================================================
        // SCREEN 0A: AUTH MENU
        // ====================================================================
        if (frameScreen == ScreenState::AUTH_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 180, 42, Theme::TEXT_LIGHT);
            DrawCenteredText("Master programming from scratch with clear lessons, quizzes & labs", 240, 20, Theme::TEXT_MUTED);

            float btnW = 360.0f;
            float btnH = 60.0f;
            float centerX = (screenWidth - btnW) / 2.0f;

            if (DrawButton({ centerX, 360.0f, btnW, btnH }, "Login", Theme::BTN_PRIMARY, 24)) {
                state.inputUsername[0] = '\0';
                state.inputPassword[0] = '\0';
                state.authErrorMsg = "";
                state.activeInputBox = 0;
                state.currentScreen = ScreenState::LOGIN;
            }
            if (DrawButton({ centerX, 445.0f, btnW, btnH }, "Register New Account", Theme::SUCCESS_COL, 24)) {
                state.inputUsername[0] = '\0';
                state.inputPassword[0] = '\0';
                state.authErrorMsg = "";
                state.activeInputBox = 0;
                state.currentScreen = ScreenState::REGISTER;
            }
        }

        // ====================================================================
        // SCREEN 0B: LOGIN SCREEN
        // ====================================================================
        else if (frameScreen == ScreenState::LOGIN) {
            DrawCenteredText("USER LOGIN", 140, 36, Theme::TEXT_LIGHT);

            Rectangle userBox = { 400, 260, 400, 45 };
            Rectangle passBox = { 400, 360, 400, 45 };

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1;
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2;
                else state.activeInputBox = 0;
            }

            DrawCustomText("Username:", 400, 230, 20, Theme::TEXT_MUTED);
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY;
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG);
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder);
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT);
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32);

            DrawCustomText("Password:", 400, 330, 20, Theme::TEXT_MUTED);
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY;
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG);
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder);
            std::string maskedPass(strlen(state.inputPassword), '*');
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT);
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32);

            if (!state.authErrorMsg.empty()) {
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL);
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Sign In", Theme::SUCCESS_COL, 20)) {
                bool found = false;
                for (size_t i = 0; i < state.registeredUsers.size(); ++i) {
                    if (state.registeredUsers[i].username == state.inputUsername &&
                        state.registeredUsers[i].password == state.inputPassword) {
                        state.currentUserIndex = (int)i;
                        SyncUserDailyStatus(state.registeredUsers[i]);
                        SaveUsers(state.registeredUsers);
                        found = true;
                        break;
                    }
                }
                if (found) {
                    state.currentScreen = ScreenState::MAIN_MENU;
                }
                else {
                    state.authErrorMsg = "Invalid username or password!";
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::AUTH_MENU;
            }
        }

        // ====================================================================
        // SCREEN 0C: REGISTER SCREEN
        // ====================================================================
        else if (frameScreen == ScreenState::REGISTER) {
            DrawCenteredText("CREATE ACCOUNT", 140, 36, Theme::TEXT_LIGHT);

            Rectangle userBox = { 400, 260, 400, 45 };
            Rectangle passBox = { 400, 360, 400, 45 };

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, userBox)) state.activeInputBox = 1;
                else if (CheckCollisionPointRec(mouse, passBox)) state.activeInputBox = 2;
                else state.activeInputBox = 0;
            }

            DrawCustomText("Choose Username:", 400, 230, 20, Theme::TEXT_MUTED);
            Color userBorder = (state.activeInputBox == 1) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY;
            DrawRectangleRounded(userBox, 0.2f, 6, Theme::PANEL_BG);
            DrawRectangleRoundedLines(userBox, 0.2f, 6, userBorder);
            DrawCustomText(state.inputUsername, 415, 272, 20, Theme::TEXT_LIGHT);
            if (state.activeInputBox == 1) UpdateTextInput(state.inputUsername, 32);

            DrawCustomText("Choose Password:", 400, 330, 20, Theme::TEXT_MUTED);
            Color passBorder = (state.activeInputBox == 2) ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY;
            DrawRectangleRounded(passBox, 0.2f, 6, Theme::PANEL_BG);
            DrawRectangleRoundedLines(passBox, 0.2f, 6, passBorder);
            std::string maskedPass(strlen(state.inputPassword), '*');
            DrawCustomText(maskedPass.c_str(), 415, 372, 20, Theme::TEXT_LIGHT);
            if (state.activeInputBox == 2) UpdateTextInput(state.inputPassword, 32);

            if (!state.authErrorMsg.empty()) {
                DrawCenteredText(state.authErrorMsg.c_str(), 430, 18, Theme::ERROR_COL);
            }

            if (DrawButton({ 400, 540, 185, 50 }, "Register", Theme::SUCCESS_COL, 20)) {
                std::string u(state.inputUsername);
                std::string p(state.inputPassword);
                if (u.empty() || p.empty()) {
                    state.authErrorMsg = "Fields cannot be empty!";
                }
                else {
                    bool exists = false;
                    for (auto& user : state.registeredUsers) {
                        if (user.username == u) exists = true;
                    }
                    if (exists) {
                        state.authErrorMsg = "Username already taken!";
                    }
                    else {
                        User newUser{ u, p, 0, 0, 1, 5, GetTodayDate(), "Basics & Syntax" };
                        state.registeredUsers.push_back(newUser);
                        state.currentUserIndex = (int)state.registeredUsers.size() - 1;
                        SaveUsers(state.registeredUsers);
                        state.currentScreen = ScreenState::MAIN_MENU;
                    }
                }
            }

            if (DrawButton({ 615, 540, 185, 50 }, "Back", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::AUTH_MENU;
            }
        }

        // ====================================================================
        // SCREEN 1: MAIN MENU (SIDEBAR + TOP 3 INFO CARDS + CENTER LEADERBOARD)
        // ====================================================================
        else if (frameScreen == ScreenState::MAIN_MENU) {
            const float sidebarW = 270.0f;
            DrawRectangle(0, 0, (int)sidebarW, screenHeight, Theme::SIDEBAR_BG);
            DrawLine((int)sidebarW, 0, (int)sidebarW, screenHeight, Theme::BTN_SECONDARY);

            // Language Switcher
            DrawCustomText("TARGET LANGUAGE", 22, 20, 14, Theme::TEXT_MUTED);
            Color cCol = (state.language == TargetLang::LANG_C) ? Theme::SUCCESS_COL : Theme::PANEL_BG;
            Color cppCol = (state.language == TargetLang::LANG_CPP) ? Theme::SUCCESS_COL : Theme::PANEL_BG;

            if (DrawButton({ 20, 44, 110, 42 }, "C", cCol, 20)) {
                state.language = TargetLang::LANG_C;
                db = GetDatabase(state.language);
            }
            if (DrawButton({ 140, 44, 110, 42 }, "C++", cppCol, 20)) {
                state.language = TargetLang::LANG_CPP;
                db = GetDatabase(state.language);
            }

            DrawLine(20, 105, (int)sidebarW - 20, 105, Theme::BTN_SECONDARY);

            User* currUser = nullptr;
            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) {
                currUser = &state.registeredUsers[state.currentUserIndex];
                DrawCustomText(currUser->username.c_str(), 22, 122, 22, Theme::TEXT_LIGHT);
                DrawCustomText(TextFormat("Total XP: %d XP", currUser->xp), 22, 150, 17, Theme::WARNING_COL);
            }

            DrawLine(20, 185, (int)sidebarW - 20, 185, Theme::BTN_SECONDARY);
            DrawCustomText("LEARNING MODES", 22, 202, 14, Theme::TEXT_MUTED);

            float navX = 20.0f;
            float navW = sidebarW - 40.0f;
            float navH = 54.0f;
            float navStartY = 230.0f;
            float navGap = 68.0f;

            // ALL 4 MODES OPEN THEIR TOPIC SELECTION SCREEN FIRST
            if (DrawButton({ navX, navStartY, navW, navH }, "Theory", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::THEORY_TOPICS;
            }
            if (DrawButton({ navX, navStartY + navGap, navW, navH }, "Quiz", Theme::BTN_PRIMARY, 22)) {
                if (currUser && currUser->lives <= 0) {
                    state.authErrorMsg = "No lives left today! Practice Theory or come back tomorrow.";
                }
                else {
                    state.authErrorMsg = "";
                    state.currentScreen = ScreenState::QUIZ_TOPICS;
                }
            }
            if (DrawButton({ navX, navStartY + navGap * 2, navW, navH }, "Constructor", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::CONSTRUCTOR_TOPICS;
            }
            if (DrawButton({ navX, navStartY + navGap * 3, navW, navH }, "Visualizer", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::VISUALIZER_TOPICS;
            }

            if (DrawButton({ 20, screenHeight - 70.0f, navW, 46 }, "Log Out", Theme::ERROR_COL, 18)) {
                state.currentUserIndex = -1;
                state.authErrorMsg = "";
                state.currentScreen = ScreenState::AUTH_MENU;
            }

            // Main Content Area
            float contentX = sidebarW + 30.0f;
            float contentW = screenWidth - sidebarW - 60.0f;
            float cardGap = 20.0f;
            float cardW = (contentW - cardGap * 2) / 3.0f;
            float cardH = 105.0f;
            float cardY = 25.0f;

            int streakVal = currUser ? currUser->streak : 1;
            int livesVal = currUser ? currUser->lives : 5;
            std::string weakVal = currUser ? currUser->weakTopic : "Pointers & Syntax";

            // CARD 1: Daily Streak
            Rectangle streakCard = { contentX, cardY, cardW, cardH };
            DrawRectangleRounded(streakCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(streakCard, 0.16f, 8, Theme::WARNING_COL);
            DrawFlameIcon(streakCard.x + 36, streakCard.y + 52, 1.35f, streakVal > 0);
            DrawCustomText("DAILY STREAK", (int)streakCard.x + 68, (int)streakCard.y + 20, 14, Theme::TEXT_MUTED);
            DrawCustomText(TextFormat("%d %s", streakVal, (streakVal == 1 ? "Day" : "Days")),
                (int)streakCard.x + 68, (int)streakCard.y + 44, 28, Theme::WARNING_COL);
            DrawCustomText("Keep learning daily!", (int)streakCard.x + 68, (int)streakCard.y + 78, 13, Theme::TEXT_MUTED);

            // CARD 2: Needs Practice
            Rectangle focusCard = { contentX + cardW + cardGap, cardY, cardW, cardH };
            DrawRectangleRounded(focusCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(focusCard, 0.16f, 8, Theme::BTN_PRIMARY);
            DrawCustomText("NEEDS PRACTICE", (int)focusCard.x + 20, (int)focusCard.y + 20, 14, Theme::TEXT_MUTED);
            DrawTextFitWidth(weakVal.c_str(), (int)focusCard.x + 20, (int)focusCard.y + 46, 17, cardW - 36.0f, Theme::TEXT_LIGHT);
            DrawCustomText("Focus on this topic next", (int)focusCard.x + 20, (int)focusCard.y + 78, 13, Theme::BTN_HOVER);

            // CARD 3: Daily Lives
            Rectangle livesCard = { contentX + (cardW + cardGap) * 2, cardY, cardW, cardH };
            DrawRectangleRounded(livesCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(livesCard, 0.16f, 8, (livesVal > 0) ? Theme::ERROR_COL : Theme::TEXT_MUTED);
            DrawCustomText(TextFormat("DAILY LIVES (%d/5)", livesVal), (int)livesCard.x + 20, (int)livesCard.y + 20, 14, Theme::TEXT_MUTED);
            for (int h = 0; h < 5; ++h) {
                DrawHeartIcon(livesCard.x + 34 + h * 34, livesCard.y + 58, 1.2f, h < livesVal);
            }
            DrawCustomText("0 score in Quiz = -1 Life", (int)livesCard.x + 20, (int)livesCard.y + 80, 13, Theme::TEXT_MUTED);

            if (!state.authErrorMsg.empty()) {
                DrawCustomText(state.authErrorMsg.c_str(), (int)contentX, 142, 17, Theme::ERROR_COL);
            }

            // Center Leaderboard Panel
            Rectangle lbPanel = { contentX, 168.0f, contentW, screenHeight - 195.0f };
            DrawRectangleRounded(lbPanel, 0.04f, 8, Theme::SIDEBAR_BG);
            DrawRectangleRoundedLines(lbPanel, 0.04f, 8, Theme::BTN_SECONDARY);

            DrawCustomText("DIAMOND LEAGUE LEADERBOARD", (int)lbPanel.x + 28, (int)lbPanel.y + 22, 28, Theme::WARNING_COL);
            DrawCustomText("Compete with fellow programmers. Earn XP in Theory, Quiz, Constructor & Visualizer!",
                (int)lbPanel.x + 28, (int)lbPanel.y + 56, 16, Theme::TEXT_MUTED);

            std::vector<User> sortedUsers = state.registeredUsers;
            std::sort(sortedUsers.begin(), sortedUsers.end(), [](const User& a, const User& b) {
                return a.xp > b.xp;
                });

            float rowStartY = lbPanel.y + 98.0f;
            int maxRows = std::min((int)sortedUsers.size(), 7);
            for (int i = 0; i < maxRows; ++i) {
                Rectangle rowRect = { lbPanel.x + 28.0f, rowStartY + i * 68.0f, lbPanel.width - 56.0f, 56.0f };

                bool isCurrentUser = (currUser && sortedUsers[i].username == currUser->username);
                Color rowCol = isCurrentUser ? Color{ 40, 90, 70, 255 } : Theme::PANEL_BG;
                Color borderCol = isCurrentUser ? Theme::SUCCESS_COL : Theme::BTN_PRIMARY;

                DrawRectangleRounded(rowRect, 0.2f, 6, rowCol);
                DrawRectangleRoundedLines(rowRect, 0.2f, 6, borderCol);

                std::string rankStr = TextFormat("#%d", i + 1);
                Color rankCol = Theme::TEXT_LIGHT;
                if (i == 0) rankCol = { 255, 215, 0, 255 };
                else if (i == 1) rankCol = { 192, 192, 192, 255 };
                else if (i == 2) rankCol = { 205, 127, 50, 255 };

                DrawCustomText(rankStr.c_str(), (int)rowRect.x + 22, (int)rowRect.y + 16, 22, rankCol);
                DrawCustomText(sortedUsers[i].username.c_str(), (int)rowRect.x + 85, (int)rowRect.y + 17, 20, Theme::TEXT_LIGHT);

                std::string streakBadge = TextFormat("%dd streak", sortedUsers[i].streak);
                DrawCustomText(streakBadge.c_str(), (int)(rowRect.x + rowRect.width - 230), (int)rowRect.y + 19, 16, Theme::TEXT_MUTED);

                std::string xpStr = TextFormat("%d XP", sortedUsers[i].xp);
                int xpWidth = (int)MeasureCustomText(xpStr.c_str(), 20);
                DrawCustomText(xpStr.c_str(), (int)(rowRect.x + rowRect.width - xpWidth - 24), (int)rowRect.y + 17, 20, Theme::WARNING_COL);
            }
        }

        // ====================================================================
        // TOP STATUS BAR FOR INNER SCREENS (COMPACT: y = 14..48)
        // ====================================================================
        else {
            const char* activeLangText = (state.language == TargetLang::LANG_C) ? "Active Lang: C" : "Active Lang: C++";
            DrawRectangleRounded({ 20, 14, 195, 34 }, 0.3f, 6, Theme::PANEL_BG);
            DrawCustomText(activeLangText, 35, 22, 17, Theme::WARNING_COL);

            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) {
                User& currUser = state.registeredUsers[state.currentUserIndex];
                std::string userBadge = TextFormat("User: %s | %d XP | %d Lives", currUser.username.c_str(), currUser.xp, currUser.lives);
                DrawRectangleRounded({ 228, 14, 345, 34 }, 0.3f, 6, Theme::PANEL_BG);
                DrawTextFitWidth(userBadge.c_str(), 242, 22, 16, 318.0f, Theme::SUCCESS_COL);
            }
        }

        // ====================================================================
        // SCREEN 3: THEORY TOPIC SELECTION
        // ====================================================================
        if (frameScreen == ScreenState::THEORY_TOPICS) {
            DrawCenteredText("THEORY - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT);
            DrawCenteredText("Start from Topic 1 if you are brand new to programming!", 108, 18, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.theoryTopics.size(); ++i) {
                int col = (int)(i % 2);
                int row = (int)(i / 2);
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f };
                if (DrawButton(btnRect, db.theoryTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) {
                    state.selectedTopic = (int)i;
                    state.currentCard = 0;
                    state.currentScreen = ScreenState::THEORY_CARDS;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 4: THEORY FLASHCARDS
        // ====================================================================
        else if (frameScreen == ScreenState::THEORY_CARDS) {
            const auto& topic = db.theoryTopics[state.selectedTopic];
            const auto& card = topic.cards[state.currentCard];

            DrawCenteredText(topic.topicName.c_str(), 64, 24, Theme::TEXT_MUTED);

            Rectangle cardRect = { 140, 108, 920, 475 };
            DrawRectangleRounded(cardRect, 0.05f, 10, Theme::PANEL_BG);
            DrawRectangleRoundedLines(cardRect, 0.05f, 10, Theme::BTN_PRIMARY);

            std::string progress = TextFormat("Lesson %d of %d", state.currentCard + 1, (int)topic.cards.size());
            DrawCustomText(progress.c_str(), 175, 130, 18, Theme::WARNING_COL);
            DrawCenteredText(card.title.c_str(), 164, 29, Theme::TEXT_LIGHT);
            DrawLine(175, 208, 1025, 208, Theme::BTN_SECONDARY);

            DrawTextWrapped(card.content, { 175, 228, 850, 340 }, 20, Theme::TEXT_LIGHT);

            if (state.currentCard > 0) {
                if (DrawButton({ 140, 610, 190, 50 }, "< Previous", Theme::BTN_PRIMARY, 20)) {
                    state.currentCard--;
                }
            }
            if (state.currentCard < (int)topic.cards.size() - 1) {
                if (DrawButton({ 870, 610, 190, 50 }, "Next >", Theme::BTN_PRIMARY, 20)) {
                    state.currentCard++;
                }
            }
            else {
                if (state.selectedTopic < (int)db.quizTopics.size()) {
                    if (DrawButton({ 640, 610, 210, 50 }, "Take Topic Quiz", Theme::WARNING_COL, 20)) {
                        if (state.currentUserIndex >= 0) {
                            state.registeredUsers[state.currentUserIndex].xp += 20;
                            SaveUsers(state.registeredUsers);
                        }
                        if (state.currentUserIndex >= 0 && state.registeredUsers[state.currentUserIndex].lives <= 0) {
                            state.authErrorMsg = "No lives left today! Come back tomorrow for Quizzes.";
                            state.currentScreen = ScreenState::MAIN_MENU;
                        }
                        else {
                            state.selectedQuizTopic = state.selectedTopic;
                            state.currentQuestion = 0;
                            state.score = 0;
                            state.quizFinished = false;
                            state.selectedAnswer = -1;
                            state.feedbackTimer = 0.0f;
                            state.currentScreen = ScreenState::QUIZ;
                        }
                    }
                }
                if (DrawButton({ 870, 610, 190, 50 }, "Finish Topic", Theme::SUCCESS_COL, 20)) {
                    if (state.currentUserIndex >= 0) {
                        state.registeredUsers[state.currentUserIndex].xp += 20;
                        SaveUsers(state.registeredUsers);
                    }
                    state.currentScreen = ScreenState::THEORY_TOPICS;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::THEORY_TOPICS;
            }
        }

        // ====================================================================
        // SCREEN 5A: QUIZ TOPIC SELECTION
        // ====================================================================
        else if (frameScreen == ScreenState::QUIZ_TOPICS) {
            DrawCenteredText("QUIZ - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT);
            DrawCenteredText("Choose a topic you studied in Theory to test your knowledge!", 108, 18, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.quizTopics.size(); ++i) {
                int col = (int)(i % 2);
                int row = (int)(i / 2);
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f };
                if (DrawButton(btnRect, db.quizTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) {
                    state.selectedQuizTopic = (int)i;
                    state.currentQuestion = 0;
                    state.score = 0;
                    state.quizFinished = false;
                    state.selectedAnswer = -1;
                    state.feedbackTimer = 0.0f;
                    state.currentScreen = ScreenState::QUIZ;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 5B: QUIZ GAME
        // ====================================================================
        else if (frameScreen == ScreenState::QUIZ) {
            const auto& qTopic = db.quizTopics[state.selectedQuizTopic];

            if (!state.quizFinished) {
                const auto& q = qTopic.questions[state.currentQuestion];

                DrawCenteredText(qTopic.topicName.c_str(), 58, 20, Theme::TEXT_MUTED);
                std::string header = TextFormat("QUESTION %d OF %d", state.currentQuestion + 1, (int)qTopic.questions.size());
                DrawCenteredText(header.c_str(), 86, 22, Theme::WARNING_COL);

                Rectangle qBox = { 130, 122, 940, 155 };
                DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG);
                DrawTextWrapped(q.question, { 165, 152, 870, 105 }, 24, Theme::TEXT_LIGHT);

                for (int i = 0; i < 4; ++i) {
                    int col = i % 2;
                    int row = i / 2;
                    Rectangle optRect = { 130.0f + col * 480.0f, 308.0f + row * 145.0f, 460.0f, 125.0f };

                    Color btnCol = kahootColors[i];
                    if (state.selectedAnswer != -1) {
                        if (i == q.correctIndex) btnCol = Theme::SUCCESS_COL;
                        else if (i == state.selectedAnswer) btnCol = Theme::ERROR_COL;
                        else btnCol = Theme::BTN_SECONDARY;
                    }

                    if (DrawButton(optRect, q.options[i].c_str(), btnCol, 20) && state.selectedAnswer == -1) {
                        state.selectedAnswer = i;
                        if (i == q.correctIndex) {
                            state.score++;
                            if (state.currentUserIndex >= 0) {
                                state.registeredUsers[state.currentUserIndex].xp += 15;
                                SaveUsers(state.registeredUsers);
                            }
                        }
                        else if (state.currentUserIndex >= 0) {
                            state.registeredUsers[state.currentUserIndex].weakTopic = qTopic.topicName;
                            SaveUsers(state.registeredUsers);
                        }
                        state.feedbackTimer = 1.4f;
                    }
                }

                if (state.selectedAnswer != -1) {
                    state.feedbackTimer -= dt;
                    const char* statusMsg = (state.selectedAnswer == q.correctIndex) ? "Correct! +15 XP" : "Incorrect! Review this in Theory.";
                    Color statusCol = (state.selectedAnswer == q.correctIndex) ? Theme::SUCCESS_COL : Theme::ERROR_COL;
                    DrawCenteredText(statusMsg, 625, 28, statusCol);

                    if (state.feedbackTimer <= 0.0f) {
                        state.selectedAnswer = -1;
                        state.currentQuestion++;
                        if (state.currentQuestion >= (int)qTopic.questions.size()) {
                            state.quizFinished = true;
                            if (state.score == 0 && state.currentUserIndex >= 0) {
                                User& u = state.registeredUsers[state.currentUserIndex];
                                if (u.lives > 0) u.lives -= 1;
                                SaveUsers(state.registeredUsers);
                            }
                        }
                    }
                }
            }
            else {
                DrawCenteredText("QUIZ COMPLETED!", 160, 44, Theme::SUCCESS_COL);
                DrawCenteredText(qTopic.topicName.c_str(), 218, 24, Theme::TEXT_MUTED);
                std::string scoreStr = TextFormat("Your Final Score: %d / %d", state.score, (int)qTopic.questions.size());
                DrawCenteredText(scoreStr.c_str(), 265, 32, Theme::TEXT_LIGHT);

                if (state.score == 0) {
                    DrawCenteredText("0 correct answers! You lost 1 Daily Life.", 320, 22, Theme::ERROR_COL);
                }

                bool canRetry = true;
                if (state.currentUserIndex >= 0 && state.registeredUsers[state.currentUserIndex].lives <= 0) {
                    canRetry = false;
                    DrawCenteredText("Out of lives for today!", 355, 20, Theme::WARNING_COL);
                }

                if (canRetry) {
                    if (DrawButton({ 340, 410, 240, 55 }, "Retry This Topic", Theme::BTN_PRIMARY, 20)) {
                        state.currentQuestion = 0;
                        state.score = 0;
                        state.quizFinished = false;
                        state.selectedAnswer = -1;
                    }
                    if (DrawButton({ 620, 410, 240, 55 }, "Other Quiz Topics", Theme::SUCCESS_COL, 20)) {
                        state.currentScreen = ScreenState::QUIZ_TOPICS;
                    }
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Quiz Topics", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::QUIZ_TOPICS;
            }
        }

        // ====================================================================
        // SCREEN 6A: CONSTRUCTOR TOPIC SELECTION
        // ====================================================================
        else if (frameScreen == ScreenState::CONSTRUCTOR_TOPICS) {
            DrawCenteredText("CODE CONSTRUCTOR - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT);
            DrawCenteredText("Choose a topic first, then build its lab challenges block-by-block!", 108, 18, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.constructorTopics.size(); ++i) {
                int col = (int)(i % 2);
                int row = (int)(i / 2);
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f };
                if (DrawButton(btnRect, db.constructorTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) {
                    state.selectedConstructorTopic = (int)i;
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 6B: CONSTRUCTOR LEVEL SELECTION INSIDE CHOSEN TOPIC
        // ====================================================================
        else if (frameScreen == ScreenState::CONSTRUCTOR_LEVELS) {
            const auto& cTopic = db.constructorTopics[state.selectedConstructorTopic];
            DrawCenteredText(cTopic.topicName.c_str(), 68, 30, Theme::TEXT_LIGHT);
            DrawCenteredText("Select a Code Constructor Lab for this topic:", 112, 19, Theme::TEXT_MUTED);

            for (size_t i = 0; i < cTopic.levels.size(); ++i) {
                Rectangle btnRect = { 300.0f, 185.0f + i * 88.0f, 600.0f, 68.0f };
                if (DrawButton(btnRect, cTopic.levels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 22)) {
                    state.selectedConstructorLevel = (int)i;
                    InitConstructorLevel(state, cTopic.levels[i]);
                    state.currentScreen = ScreenState::CONSTRUCTOR_GAME;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::CONSTRUCTOR_TOPICS;
            }
        }

        // ====================================================================
        // SCREEN 7: CONSTRUCTOR GAME (CLEAN NON-OVERLAPPING LAYOUT)
        // ====================================================================
        else if (frameScreen == ScreenState::CONSTRUCTOR_GAME) {
            const auto& cTopic = db.constructorTopics[state.selectedConstructorTopic];
            const auto& level = cTopic.levels[state.selectedConstructorLevel];
            int numSlots = (int)level.correctLines.size();

            // Title placed cleanly BELOW the top status bar (y = 58)
            DrawCenteredText(level.levelTitle.c_str(), 58, 24, Theme::TEXT_LIGHT);
            DrawCenteredText("Drag code blocks from the bottom pool into the numbered slots, then click Ready:", 86, 15, Theme::TEXT_MUTED);

            // Car track (y = 144)
            DrawLine(180, 144, 1020, 144, Theme::BTN_SECONDARY);
            DrawCar(state.carX, 104, state.carCrashed);

            // Slots (start y = 156, spacing = 44 -> max 6 slots ends at y = 416)
            std::vector<Rectangle> slotRects(numSlots);
            for (int i = 0; i < numSlots; ++i) {
                slotRects[i] = { 210.0f, 156.0f + i * 44.0f, 780.0f, 38.0f };
            }

            if (state.isCarMoving && !state.carCrashed && !state.levelSuccess) {
                state.carX += 270.0f * dt;
                float stepWidth = 780.0f / (float)numSlots;
                int currentInspectSlot = (int)((state.carX - 180.0f) / stepWidth);

                if (currentInspectSlot >= 0 && currentInspectSlot < numSlots) {
                    int bIdx = state.slotBlockIndex[currentInspectSlot];
                    bool slotCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[currentInspectSlot]);

                    if (!slotCorrect) {
                        state.carCrashed = true;
                        state.isCarMoving = false;
                        state.firstErrorLine = currentInspectSlot;
                        state.showErrorModal = true;
                        state.errorModalScrollY = 0.0f;
                        if (state.currentUserIndex >= 0) {
                            state.registeredUsers[state.currentUserIndex].weakTopic = cTopic.topicName;
                            SaveUsers(state.registeredUsers);
                        }
                    }
                    else {
                        state.carTargetLine = currentInspectSlot + 1;
                    }
                }
                else if (currentInspectSlot >= numSlots) {
                    state.levelSuccess = true;
                    state.isCarMoving = false;
                    state.carTargetLine = numSlots;
                    if (state.currentUserIndex >= 0) {
                        state.registeredUsers[state.currentUserIndex].xp += 50;
                        state.registeredUsers[state.currentUserIndex].levelsCompleted += 1;
                        SaveUsers(state.registeredUsers);
                    }
                }
            }

            for (int i = 0; i < numSlots; ++i) {
                Color slotCol = Theme::PANEL_BG;
                if (state.carTargetLine > i) {
                    slotCol = Theme::SUCCESS_COL;
                }
                else if (state.carCrashed) {
                    int bIdx = state.slotBlockIndex[i];
                    bool isCorrect = (bIdx != -1 && state.blocks[bIdx].text == level.correctLines[i]);
                    if (!isCorrect) slotCol = Theme::ERROR_COL;
                }

                DrawRectangleRounded(slotRects[i], 0.2f, 6, slotCol);
                DrawRectangleRoundedLines(slotRects[i], 0.2f, 6, Theme::BTN_SECONDARY);
                DrawCustomText(TextFormat("%d.", i + 1), (int)slotRects[i].x - 36, (int)slotRects[i].y + 9, 19, Theme::TEXT_MUTED);
            }

            // Divider & Pool header (y = 436..474)
            DrawLine(65, 436, 1135, 436, Theme::BTN_SECONDARY);
            DrawCustomText("Available Code Blocks (drag into slots above):", 65, 448, 17, Theme::TEXT_MUTED);

            bool interactionLocked = state.isCarMoving || state.showErrorModal || state.levelSuccess;
            if (!interactionLocked) {
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

                for (auto& b : state.blocks) {
                    if (b.isDragging) {
                        b.rect.x = mouse.x - b.dragOffset.x;
                        b.rect.y = mouse.y - b.dragOffset.y;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                            b.isDragging = false;
                            for (int s = 0; s < numSlots; ++s) {
                                if (CheckCollisionPointRec(mouse, slotRects[s]) && state.slotBlockIndex[s] == -1) {
                                    b.currentSlot = s;
                                    b.rect.x = slotRects[s].x + 8.0f;
                                    b.rect.y = slotRects[s].y + 1.0f;
                                    state.slotBlockIndex[s] = (int)(&b - &state.blocks[0]);
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            // Draw non-dragging blocks first, then dragging block on top
            for (int pass = 0; pass < 2; ++pass) {
                for (size_t i = 0; i < state.blocks.size(); ++i) {
                    const auto& b = state.blocks[i];
                    if ((pass == 0 && b.isDragging) || (pass == 1 && !b.isDragging)) continue;

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
                    DrawTextFitWidth(b.text.c_str(), (int)b.rect.x + 12, (int)b.rect.y + 9, 17, b.rect.width - 24.0f, Theme::TEXT_LIGHT);
                }
            }

            // Bottom action buttons (y = 720)
            if (!state.isCarMoving && !state.levelSuccess && !state.showErrorModal) {
                if (DrawButton({ 740, 720, 220, 50 }, "Ready (Run Car)", Theme::SUCCESS_COL, 20)) {
                    state.carX = 180.0f;
                    state.carTargetLine = 0;
                    state.carCrashed = false;
                    state.isCarMoving = true;
                }
            }

            if (DrawButton({ 240, 720, 220, 50 }, "Back to Labs", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
            }

            // Success Modal
            if (state.levelSuccess) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 185 });
                Rectangle modal = { 280, 220, 640, 320 };
                DrawRectangleRounded(modal, 0.08f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.08f, 10, Theme::SUCCESS_COL);

                DrawCenteredText("CONGRATULATIONS!", 265, 36, Theme::SUCCESS_COL);
                DrawCenteredText("The car passed every line! +50 XP earned.", 330, 22, Theme::TEXT_LIGHT);

                if (DrawButton({ 340, 440, 230, 50 }, "Play Again", Theme::BTN_PRIMARY, 20)) {
                    InitConstructorLevel(state, level);
                }
                if (DrawButton({ 630, 440, 230, 50 }, "Topic Labs", Theme::BTN_SECONDARY, 20)) {
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
                }
            }

            // Error Modal (WITH SCISSOR CLIPPING & MOUSE WHEEL SCROLLING)
            if (state.showErrorModal) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 195 });
                Rectangle modal = { 130, 75, 940, 650 };
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::ERROR_COL);

                DrawCenteredText("CAR CRASHED! CODE ERRORS DETECTED", 100, 28, Theme::ERROR_COL);
                std::string crashInfo = TextFormat("First breakdown at Line %d. Scroll down if there are multiple errors:", state.firstErrorLine + 1);
                DrawCenteredText(crashInfo.c_str(), 138, 18, Theme::WARNING_COL);

                std::string errorReport = "";
                for (int s = 0; s < numSlots; ++s) {
                    int bIdx = state.slotBlockIndex[s];
                    std::string userCode = (bIdx == -1) ? "[Empty Slot]" : state.blocks[bIdx].text;
                    if (userCode != level.correctLines[s]) {
                        errorReport += TextFormat("Line %d:\n  Your block: %s\n  Expected:   %s\n  Why: %s\n\n",
                            s + 1, userCode.c_str(), level.correctLines[s].c_str(), level.slotExplanations[s].c_str());
                    }
                }

                Rectangle reportViewport = { 165, 175, 870, 445 };
                DrawRectangleRounded(reportViewport, 0.03f, 6, Theme::BG_DARK);

                float contentH = MeasureTextWrappedHeight(errorReport, reportViewport.width - 36.0f, 17) + 20.0f;
                float maxScroll = std::max(0.0f, contentH - reportViewport.height);

                float wheel = GetMouseWheelMove();
                if (wheel != 0.0f) state.errorModalScrollY -= wheel * 35.0f;
                if (IsKeyDown(KEY_DOWN)) state.errorModalScrollY += 260.0f * dt;
                if (IsKeyDown(KEY_UP)) state.errorModalScrollY -= 260.0f * dt;
                state.errorModalScrollY = std::clamp(state.errorModalScrollY, 0.0f, maxScroll);

                BeginScissorMode((int)reportViewport.x + 4, (int)reportViewport.y + 4,
                    (int)reportViewport.width - 8, (int)reportViewport.height - 8);
                DrawTextWrapped(errorReport,
                    { reportViewport.x + 18.0f, reportViewport.y + 12.0f - state.errorModalScrollY,
                      reportViewport.width - 36.0f, contentH },
                    17, Theme::TEXT_LIGHT);
                EndScissorMode();

                // Scrollbar indicator if needed
                if (maxScroll > 0.0f) {
                    float barH = std::max(36.0f, reportViewport.height * (reportViewport.height / contentH));
                    float barY = reportViewport.y + (state.errorModalScrollY / maxScroll) * (reportViewport.height - barH);
                    DrawRectangleRounded({ reportViewport.x + reportViewport.width - 10.0f, barY, 6.0f, barH }, 0.5f, 4, Theme::TEXT_MUTED);
                }

                if (DrawButton({ 330, 645, 240, 50 }, "Retry Level", Theme::BTN_PRIMARY, 20)) {
                    InitConstructorLevel(state, level);
                }
                if (DrawButton({ 630, 645, 240, 50 }, "Topic Labs", Theme::BTN_SECONDARY, 20)) {
                    state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
                }
            }
        }

        // ====================================================================
        // SCREEN 8A: VISUALIZER TOPIC SELECTION
        // ====================================================================
        else if (frameScreen == ScreenState::VISUALIZER_TOPICS) {
            DrawCenteredText("CODE VISUALIZER - SELECT A TOPIC", 68, 32, Theme::TEXT_LIGHT);
            DrawCenteredText("Choose a topic first, then inspect its lab solutions line-by-line!", 108, 18, Theme::TEXT_MUTED);

            for (size_t i = 0; i < db.visualizerTopics.size(); ++i) {
                int col = (int)(i % 2);
                int row = (int)(i / 2);
                Rectangle btnRect = { 70.0f + col * 540.0f, 152.0f + row * 82.0f, 520.0f, 64.0f };
                if (DrawButton(btnRect, db.visualizerTopics[i].topicName.c_str(), Theme::BTN_PRIMARY, 20)) {
                    state.selectedVisualizerTopic = (int)i;
                    state.currentScreen = ScreenState::VISUALIZER_LEVELS;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 8B: VISUALIZER LEVEL SELECTION INSIDE CHOSEN TOPIC
        // ====================================================================
        else if (frameScreen == ScreenState::VISUALIZER_LEVELS) {
            const auto& vTopic = db.visualizerTopics[state.selectedVisualizerTopic];
            DrawCenteredText(vTopic.topicName.c_str(), 68, 30, Theme::TEXT_LIGHT);
            DrawCenteredText("Select a Visualizer Lab Walkthrough for this topic:", 112, 19, Theme::TEXT_MUTED);

            for (size_t i = 0; i < vTopic.levels.size(); ++i) {
                Rectangle btnRect = { 300.0f, 185.0f + i * 88.0f, 600.0f, 68.0f };
                if (DrawButton(btnRect, vTopic.levels[i].levelTitle.c_str(), Theme::BTN_PRIMARY, 22)) {
                    state.selectedVisualizerLevel = (int)i;
                    InitVisualizerLevel(state, vTopic.levels[i]);
                    state.currentScreen = ScreenState::VISUALIZER_VIEW;
                }
            }

            if (DrawButton({ 480, 715, 240, 50 }, "Back to Topics", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::VISUALIZER_TOPICS;
            }
        }

        // ====================================================================
        // SCREEN 9: VISUALIZER STEP-BY-STEP VIEW (SCROLLABLE + DYNAMIC BOXES)
        // ====================================================================
        else if (frameScreen == ScreenState::VISUALIZER_VIEW) {
            const auto& vTopic = db.visualizerTopics[state.selectedVisualizerTopic];
            const auto& level = vTopic.levels[state.selectedVisualizerLevel];

            // Title cleanly BELOW the top status bar (y = 58)
            DrawCenteredText(level.levelTitle.c_str(), 58, 26, Theme::TEXT_LIGHT);

            Rectangle viewport = { 75.0f, 100.0f, 1050.0f, 560.0f };
            DrawRectangleRounded(viewport, 0.03f, 8, Theme::SIDEBAR_BG);
            DrawRectangleRoundedLines(viewport, 0.03f, 8, Theme::BTN_SECONDARY);

            // Compute total dynamic content height first so scrolling clamps accurately
            float totalContentH = 16.0f;
            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) {
                totalContentH += 44.0f; // code line box + gap
                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) {
                    totalContentH += 42.0f; // Explanation button
                }
                if (state.explanationVisible[i]) {
                    float expTextH = MeasureTextWrappedHeight(level.steps[i].explanation, 720.0f, 17);
                    float expBoxH = std::max(46.0f, expTextH + 20.0f);
                    totalContentH += expBoxH + 8.0f;
                    if (!state.stepUnderstood[i]) {
                        totalContentH += 42.0f; // Got It! button
                    }
                }
            }
            totalContentH += 20.0f;

            float maxScroll = std::max(0.0f, totalContentH - viewport.height);
            if (!state.showIntroModal) {
                float wheel = GetMouseWheelMove();
                if (wheel != 0.0f) state.visualizerScrollY -= wheel * 40.0f;
                if (IsKeyDown(KEY_DOWN)) state.visualizerScrollY += 260.0f * dt;
                if (IsKeyDown(KEY_UP)) state.visualizerScrollY -= 260.0f * dt;
            }
            state.visualizerScrollY = std::clamp(state.visualizerScrollY, 0.0f, maxScroll);

            // Clipped scrollable drawing of all revealed lines & dynamic explanation boxes
            BeginScissorMode((int)viewport.x + 4, (int)viewport.y + 4,
                (int)viewport.width - 8, (int)viewport.height - 8);

            bool mouseInViewport = CheckCollisionPointRec(mouse, viewport) && !state.showIntroModal;
            float curY = viewport.y + 16.0f - state.visualizerScrollY;

            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) {
                Rectangle lineBox = { 105.0f, curY, 790.0f, 38.0f };
                DrawRectangleRounded(lineBox, 0.2f, 6, Theme::PANEL_BG);
                DrawRectangleRoundedLines(lineBox, 0.2f, 6, Theme::BTN_PRIMARY);
                DrawTextFitWidth(level.steps[i].codeLine.c_str(), 122, (int)curY + 9, 18, lineBox.width - 28.0f, Theme::TEXT_LIGHT);

                if (state.stepUnderstood[i]) {
                    const char* toggleLabel = state.explanationVisible[i] ? "Hide Info" : "Show Info";
                    Rectangle toggleRect = { 912.0f, curY, 175.0f, 38.0f };
                    if (DrawButton(toggleRect, toggleLabel, Theme::BTN_SECONDARY, 17) && mouseInViewport) {
                        state.explanationVisible[i] = !state.explanationVisible[i];
                    }
                }

                curY += 44.0f;

                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) {
                    Rectangle expBtn = { 125.0f, curY, 190.0f, 34.0f };
                    if (DrawButton(expBtn, "Explanation", Theme::WARNING_COL, 18) && mouseInViewport) {
                        state.explanationVisible[i] = true;
                    }
                    curY += 42.0f;
                }

                if (state.explanationVisible[i]) {
                    float expTextH = MeasureTextWrappedHeight(level.steps[i].explanation, 720.0f, 17);
                    float expBoxH = std::max(46.0f, expTextH + 20.0f);
                    Rectangle expBox = { 125.0f, curY, 760.0f, expBoxH };
                    DrawRectangleRounded(expBox, 0.16f, 6, { 30, 58, 50, 255 });
                    DrawRectangleRoundedLines(expBox, 0.16f, 6, Theme::SUCCESS_COL);
                    DrawTextWrapped(level.steps[i].explanation, { 145.0f, curY + 10.0f, 720.0f, expTextH + 8.0f }, 17, Theme::TEXT_LIGHT);
                    curY += expBoxH + 8.0f;

                    if (!state.stepUnderstood[i]) {
                        Rectangle gotItBtn = { 125.0f, curY, 160.0f, 34.0f };
                        if (DrawButton(gotItBtn, "Got It!", Theme::SUCCESS_COL, 18) && mouseInViewport) {
                            state.stepUnderstood[i] = true;
                            state.explanationVisible[i] = false;
                            if (state.revealedLinesCount < (int)level.steps.size()) {
                                state.revealedLinesCount++;
                            }
                            if (state.currentUserIndex >= 0) {
                                state.registeredUsers[state.currentUserIndex].xp += 5;
                                SaveUsers(state.registeredUsers);
                            }
                        }
                        curY += 42.0f;
                    }
                }
            }

            EndScissorMode();

            // Draw scrollbar if content overflows viewport
            if (maxScroll > 0.0f) {
                float barH = std::max(40.0f, viewport.height * (viewport.height / totalContentH));
                float barY = viewport.y + (state.visualizerScrollY / maxScroll) * (viewport.height - barH);
                DrawRectangleRounded({ viewport.x + viewport.width - 12.0f, barY, 7.0f, barH }, 0.5f, 4, Theme::TEXT_MUTED);
            }

            if (!state.stepUnderstood.empty() && state.stepUnderstood.back()) {
                DrawCenteredText("All lines analyzed! Scroll or toggle any explanation above, or pick another lab.", 674, 18, Theme::SUCCESS_COL);
            }
            else if (maxScroll > 0.0f) {
                DrawCenteredText("Tip: Use Mouse Wheel or Up/Down keys to scroll through steps.", 674, 16, Theme::TEXT_MUTED);
            }

            if (DrawButton({ 480, 718, 240, 50 }, "Back to Labs", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::VISUALIZER_LEVELS;
            }

            // Intro Modal
            if (state.showIntroModal) {
                DrawRectangle(0, 0, screenWidth, screenHeight, { 0, 0, 0, 205 });
                Rectangle modal = { 180, 85, 840, 610 };
                DrawRectangleRounded(modal, 0.05f, 10, Theme::PANEL_BG);
                DrawRectangleRoundedLines(modal, 0.05f, 10, Theme::BTN_PRIMARY);

                DrawCenteredText("LAB CODE OVERVIEW", 112, 30, Theme::WARNING_COL);
                DrawCenteredText("Here is the complete code we will break down step-by-step:", 152, 18, Theme::TEXT_LIGHT);

                Rectangle codeBox = { 220, 190, 760, 405 };
                DrawRectangleRounded(codeBox, 0.04f, 8, Theme::BG_DARK);

                BeginScissorMode((int)codeBox.x + 4, (int)codeBox.y + 4, (int)codeBox.width - 8, (int)codeBox.height - 8);
                DrawTextWrapped(level.fullCode, { codeBox.x + 22.0f, codeBox.y + 18.0f, codeBox.width - 44.0f, codeBox.height - 30.0f }, 18, Theme::SUCCESS_COL);
                EndScissorMode();

                if (DrawButton({ 320, 620, 220, 50 }, "Back to Labs", Theme::ERROR_COL, 20)) {
                    state.currentScreen = ScreenState::VISUALIZER_LEVELS;
                }
                if (DrawButton({ 660, 620, 220, 50 }, "Let's Go!", Theme::SUCCESS_COL, 20)) {
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
