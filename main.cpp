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
        user.lives = 5; // Restore 5 lives each new day
        if (!user.lastActiveDate.empty()) {
            user.streak += 1; // Increment streak on new day login
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

    std::mt19937 rng(1337 + state.selectedConstructorLevel);
    std::shuffle(allTexts.begin(), allTexts.end(), rng);

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

void InitVisualizerLevel(AppState& state, const VisualizerLevel& level) {
    state.showIntroModal = true;
    state.revealedLinesCount = 1;
    state.explanationVisible.assign(level.steps.size(), false);
    state.stepUnderstood.assign(level.steps.size(), false);
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

        BeginDrawing();
        ClearBackground(Theme::BG_DARK);

        // ====================================================================
        // SCREEN 0A: AUTH MENU
        // ====================================================================
        if (state.currentScreen == ScreenState::AUTH_MENU) {
            DrawCenteredText("C / C++ LEARNING STUDIO", 180, 42, Theme::TEXT_LIGHT);
            DrawCenteredText("Master programming with interactive challenges & Duolingo-style XP", 240, 20, Theme::TEXT_MUTED);

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
        else if (state.currentScreen == ScreenState::LOGIN) {
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
        else if (state.currentScreen == ScreenState::REGISTER) {
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
        else if (state.currentScreen == ScreenState::MAIN_MENU) {
            // --- 1. LEFT SIDEBAR ---
            const float sidebarW = 270.0f;
            DrawRectangle(0, 0, (int)sidebarW, screenHeight, Theme::SIDEBAR_BG);
            DrawLine((int)sidebarW, 0, (int)sidebarW, screenHeight, Theme::BTN_SECONDARY);

            // Top of Sidebar: Language Switcher (C / C++)
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

            // User Profile Info in Sidebar
            User* currUser = nullptr;
            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) {
                currUser = &state.registeredUsers[state.currentUserIndex];
                DrawCustomText(currUser->username.c_str(), 22, 122, 22, Theme::TEXT_LIGHT);
                DrawCustomText(TextFormat("Total XP: %d XP", currUser->xp), 22, 150, 17, Theme::WARNING_COL);
            }

            DrawLine(20, 185, (int)sidebarW - 20, 185, Theme::BTN_SECONDARY);
            DrawCustomText("LEARNING MODES", 22, 202, 14, Theme::TEXT_MUTED);

            // 4 Active Mode Buttons in Sidebar
            float navX = 20.0f;
            float navW = sidebarW - 40.0f;
            float navH = 54.0f;
            float navStartY = 230.0f;
            float navGap = 68.0f;

            if (DrawButton({ navX, navStartY, navW, navH }, "Theory", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::THEORY_TOPICS;
            }
            if (DrawButton({ navX, navStartY + navGap, navW, navH }, "Quiz", Theme::BTN_PRIMARY, 22)) {
                if (currUser && currUser->lives <= 0) {
                    state.authErrorMsg = "No lives left today! Practice Theory or come back tomorrow.";
                }
                else {
                    state.authErrorMsg = "";
                    state.currentQuestion = 0;
                    state.score = 0;
                    state.quizFinished = false;
                    state.selectedAnswer = -1;
                    state.feedbackTimer = 0.0f;
                    state.currentScreen = ScreenState::QUIZ;
                }
            }
            if (DrawButton({ navX, navStartY + navGap * 2, navW, navH }, "Constructor", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::CONSTRUCTOR_LEVELS;
            }
            if (DrawButton({ navX, navStartY + navGap * 3, navW, navH }, "Visualizer", Theme::BTN_PRIMARY, 22)) {
                state.currentScreen = ScreenState::VISUALIZER_LEVELS;
            }

            // Bottom of Sidebar: Log Out button
            if (DrawButton({ 20, screenHeight - 70.0f, navW, 46 }, "Log Out", Theme::ERROR_COL, 18)) {
                state.currentUserIndex = -1;
                state.authErrorMsg = "";
                state.currentScreen = ScreenState::AUTH_MENU;
            }

            // --- 2. MAIN CONTENT AREA (RIGHT OF SIDEBAR) ---
            float contentX = sidebarW + 30.0f;
            float contentW = screenWidth - sidebarW - 60.0f; // 870 px

            // Top Row: 3 Non-Clickable Info Cards (Streak | Practice Focus | Daily Lives)
            float cardGap = 20.0f;
            float cardW = (contentW - cardGap * 2) / 3.0f; // ~276 px each
            float cardH = 105.0f;
            float cardY = 25.0f;

            int streakVal = currUser ? currUser->streak : 1;
            int livesVal = currUser ? currUser->lives : 5;
            std::string weakVal = currUser ? currUser->weakTopic : "Pointers & Syntax";

            // CARD 1: Daily Streak with Flame Icon
            Rectangle streakCard = { contentX, cardY, cardW, cardH };
            DrawRectangleRounded(streakCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(streakCard, 0.16f, 8, Theme::WARNING_COL);
            DrawFlameIcon(streakCard.x + 36, streakCard.y + 52, 1.35f, streakVal > 0);
            DrawCustomText("DAILY STREAK", (int)streakCard.x + 68, (int)streakCard.y + 20, 14, Theme::TEXT_MUTED);
            DrawCustomText(TextFormat("%d %s", streakVal, (streakVal == 1 ? "Day" : "Days")),
                (int)streakCard.x + 68, (int)streakCard.y + 44, 28, Theme::WARNING_COL);
            DrawCustomText("Keep learning daily!", (int)streakCard.x + 68, (int)streakCard.y + 78, 13, Theme::TEXT_MUTED);

            // CARD 2: Practice Focus (Recommended area to improve)
            Rectangle focusCard = { contentX + cardW + cardGap, cardY, cardW, cardH };
            DrawRectangleRounded(focusCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(focusCard, 0.16f, 8, Theme::BTN_PRIMARY);
            DrawCustomText("NEEDS PRACTICE", (int)focusCard.x + 20, (int)focusCard.y + 20, 14, Theme::TEXT_MUTED);
            DrawCustomText(weakVal.c_str(), (int)focusCard.x + 20, (int)focusCard.y + 46, 21, Theme::TEXT_LIGHT);
            DrawCustomText("Focus on this topic next", (int)focusCard.x + 20, (int)focusCard.y + 78, 13, Theme::BTN_HOVER);

            // CARD 3: Daily Lives (5 Lives per day, loses 1 on 0-score Quiz)
            Rectangle livesCard = { contentX + (cardW + cardGap) * 2, cardY, cardW, cardH };
            DrawRectangleRounded(livesCard, 0.16f, 8, Theme::PANEL_BG);
            DrawRectangleRoundedLines(livesCard, 0.16f, 8, (livesVal > 0) ? Theme::ERROR_COL : Theme::TEXT_MUTED);
            DrawCustomText(TextFormat("DAILY LIVES (%d/5)", livesVal), (int)livesCard.x + 20, (int)livesCard.y + 20, 14, Theme::TEXT_MUTED);
            for (int h = 0; h < 5; ++h) {
                DrawHeartIcon(livesCard.x + 34 + h * 34, livesCard.y + 58, 1.2f, h < livesVal);
            }
            DrawCustomText("0/4 in Quiz = -1 Life", (int)livesCard.x + 20, (int)livesCard.y + 80, 13, Theme::TEXT_MUTED);

            // Warning message if user tries to start Quiz with 0 lives
            if (!state.authErrorMsg.empty()) {
                DrawCustomText(state.authErrorMsg.c_str(), (int)contentX, 142, 17, Theme::ERROR_COL);
            }

            // --- 3. CENTER LEADERBOARD PANEL ---
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

                // Show user streak badge in leaderboard row
                std::string streakBadge = TextFormat("%dd streak", sortedUsers[i].streak);
                DrawCustomText(streakBadge.c_str(), (int)(rowRect.x + rowRect.width - 230), (int)rowRect.y + 19, 16, Theme::TEXT_MUTED);

                std::string xpStr = TextFormat("%d XP", sortedUsers[i].xp);
                int xpWidth = (int)MeasureCustomText(xpStr.c_str(), 20);
                DrawCustomText(xpStr.c_str(), (int)(rowRect.x + rowRect.width - xpWidth - 24), (int)rowRect.y + 17, 20, Theme::WARNING_COL);
            }
        }

        // ====================================================================
        // TOP STATUS BAR FOR INNER SCREENS (THEORY / QUIZ / CONSTRUCTOR / VISUALIZER)
        // ====================================================================
        else {
            const char* activeLangText = (state.language == TargetLang::LANG_C) ? "Active Lang: C" : "Active Lang: C++";
            DrawRectangleRounded({ 20, 20, 210, 36 }, 0.3f, 6, Theme::PANEL_BG);
            DrawCustomText(activeLangText, 35, 29, 18, Theme::WARNING_COL);

            if (state.currentUserIndex >= 0 && state.currentUserIndex < (int)state.registeredUsers.size()) {
                User& currUser = state.registeredUsers[state.currentUserIndex];
                std::string userBadge = TextFormat("User: %s | %d XP | %d Lives", currUser.username.c_str(), currUser.xp, currUser.lives);
                DrawRectangleRounded({ 245, 20, 340, 36 }, 0.3f, 6, Theme::PANEL_BG);
                DrawCustomText(userBadge.c_str(), 260, 29, 16, Theme::SUCCESS_COL);
            }
        }

        // ====================================================================
        // SCREEN 3: THEORY TOPIC SELECTION
        // ====================================================================
        if (state.currentScreen == ScreenState::THEORY_TOPICS) {
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

            Rectangle cardRect = { 220, 130, 760, 420 };
            DrawRectangleRounded(cardRect, 0.06f, 10, Theme::PANEL_BG);
            DrawRectangleRoundedLines(cardRect, 0.06f, 10, Theme::BTN_PRIMARY);

            std::string progress = TextFormat("Card %d of %d", state.currentCard + 1, (int)topic.cards.size());
            DrawCustomText(progress.c_str(), 250, 155, 18, Theme::WARNING_COL);
            DrawCenteredText(card.title.c_str(), 195, 32, Theme::TEXT_LIGHT);
            DrawLine(260, 245, 940, 245, Theme::BTN_SECONDARY);

            DrawTextWrapped(card.content, { 260, 275, 680, 240 }, 24, Theme::TEXT_LIGHT);

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
                    if (state.currentUserIndex >= 0) {
                        state.registeredUsers[state.currentUserIndex].xp += 20;
                        SaveUsers(state.registeredUsers);
                    }
                    state.currentScreen = ScreenState::THEORY_TOPICS;
                }
            }

            if (DrawButton({ 480, 710, 240, 50 }, "Back to Main Menu", Theme::BTN_SECONDARY, 20)) {
                state.currentScreen = ScreenState::MAIN_MENU;
            }
        }

        // ====================================================================
        // SCREEN 5: QUIZ (KAHOOT STYLE + LIVES PENALTY ON 0 SCORE)
        // ====================================================================
        else if (state.currentScreen == ScreenState::QUIZ) {
            if (!state.quizFinished) {
                const auto& q = db.quizQuestions[state.currentQuestion];

                std::string header = TextFormat("QUESTION %d OF %d", state.currentQuestion + 1, (int)db.quizQuestions.size());
                DrawCenteredText(header.c_str(), 75, 24, Theme::WARNING_COL);

                Rectangle qBox = { 150, 120, 900, 160 };
                DrawRectangleRounded(qBox, 0.08f, 8, Theme::PANEL_BG);
                DrawTextWrapped(q.question, { 180, 165, 840, 100 }, 28, Theme::TEXT_LIGHT);

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
                        if (i == q.correctIndex) {
                            state.score++;
                            if (state.currentUserIndex >= 0) {
                                state.registeredUsers[state.currentUserIndex].xp += 15;
                                SaveUsers(state.registeredUsers);
                            }
                        }
                        else if (state.currentUserIndex >= 0) {
                            // Update weakTopic based on missed question
                            state.registeredUsers[state.currentUserIndex].weakTopic =
                                (state.language == TargetLang::LANG_C) ? "C Pointers & I/O" : "C++ OOP & STL";
                            SaveUsers(state.registeredUsers);
                        }
                        state.feedbackTimer = 1.4f;
                    }
                }

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
                            // Deduct 1 life if the user scored 0 (all answers wrong)
                            if (state.score == 0 && state.currentUserIndex >= 0) {
                                User& u = state.registeredUsers[state.currentUserIndex];
                                if (u.lives > 0) {
                                    u.lives -= 1;
                                }
                                SaveUsers(state.registeredUsers);
                            }
                        }
                    }
                }
            }
            else {
                DrawCenteredText("QUIZ COMPLETED!", 170, 44, Theme::SUCCESS_COL);
                std::string scoreStr = TextFormat("Your Final Score: %d / %d", state.score, (int)db.quizQuestions.size());
                DrawCenteredText(scoreStr.c_str(), 245, 32, Theme::TEXT_LIGHT);

                if (state.score == 0) {
                    DrawCenteredText("0 correct answers! You lost 1 Daily Life.", 300, 22, Theme::ERROR_COL);
                }

                bool canRetry = true;
                if (state.currentUserIndex >= 0 && state.registeredUsers[state.currentUserIndex].lives <= 0) {
                    canRetry = false;
                    DrawCenteredText("Out of lives for today!", 340, 20, Theme::WARNING_COL);
                }

                if (canRetry) {
                    if (DrawButton({ 480, 390, 240, 55 }, "Retry Quiz", Theme::BTN_PRIMARY, 22)) {
                        state.currentQuestion = 0;
                        state.score = 0;
                        state.quizFinished = false;
                        state.selectedAnswer = -1;
                    }
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
        // SCREEN 7: CONSTRUCTOR GAME
        // ====================================================================
        else if (state.currentScreen == ScreenState::CONSTRUCTOR_GAME) {
            const auto& level = db.constructorLevels[state.selectedConstructorLevel];
            int numSlots = (int)level.correctLines.size();

            DrawCenteredText(level.levelTitle.c_str(), 25, 28, Theme::TEXT_LIGHT);
            DrawCenteredText("Drag code blocks from the bottom pool into the empty slots:", 60, 18, Theme::TEXT_MUTED);

            std::vector<Rectangle> slotRects(numSlots);
            for (int i = 0; i < numSlots; ++i) {
                slotRects[i] = { 240.0f, 135.0f + i * 48.0f, 720.0f, 40.0f };
            }

            DrawLine(200, 122, 1000, 122, Theme::BTN_SECONDARY);
            DrawCar(state.carX, 82, state.carCrashed);

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
                        if (state.currentUserIndex >= 0) {
                            state.registeredUsers[state.currentUserIndex].weakTopic = level.levelTitle;
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
                DrawCustomText(TextFormat("%d.", i + 1), (int)slotRects[i].x - 35, (int)slotRects[i].y + 10, 20, Theme::TEXT_MUTED);
            }

            DrawLine(100, 465, 1100, 465, Theme::BTN_SECONDARY);
            DrawCustomText("Available Code Blocks:", 140, 442, 18, Theme::TEXT_MUTED);

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
                if (DrawButton({ 630, 440, 230, 50 }, "Main Menu", Theme::BTN_SECONDARY, 20)) {
                    state.currentScreen = ScreenState::MAIN_MENU;
                }
            }

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

            float curY = 85.0f;
            for (int i = 0; i < state.revealedLinesCount && i < (int)level.steps.size(); ++i) {
                Rectangle lineBox = { 140.0f, curY, 720.0f, 38.0f };
                DrawRectangleRounded(lineBox, 0.2f, 6, Theme::PANEL_BG);
                DrawRectangleRoundedLines(lineBox, 0.2f, 6, Theme::BTN_PRIMARY);
                DrawCustomText(level.steps[i].codeLine.c_str(), 160, (int)curY + 9, 20, Theme::TEXT_LIGHT);

                if (state.stepUnderstood[i]) {
                    const char* toggleLabel = state.explanationVisible[i] ? "Hide Info" : "Show Info";
                    if (DrawButton({ 880.0f, curY, 160.0f, 38.0f }, toggleLabel, Theme::BTN_SECONDARY, 18)) {
                        state.explanationVisible[i] = !state.explanationVisible[i];
                    }
                }

                curY += 44.0f;

                if (!state.explanationVisible[i] && !state.stepUnderstood[i]) {
                    if (DrawButton({ 160.0f, curY, 190.0f, 36.0f }, "Explanation", Theme::WARNING_COL, 18)) {
                        state.explanationVisible[i] = true;
                    }
                    curY += 44.0f;
                }

                if (state.explanationVisible[i]) {
                    Rectangle expBox = { 160.0f, curY, 700.0f, 52.0f };
                    DrawRectangleRounded(expBox, 0.2f, 6, { 30, 58, 50, 255 });
                    DrawRectangleRoundedLines(expBox, 0.2f, 6, Theme::SUCCESS_COL);
                    DrawTextWrapped(level.steps[i].explanation, { 175.0f, curY + 8.0f, 670.0f, 40.0f }, 18, Theme::TEXT_LIGHT);
                    curY += 58.0f;

                    if (!state.stepUnderstood[i]) {
                        if (DrawButton({ 160.0f, curY, 160.0f, 36.0f }, "Got It!", Theme::SUCCESS_COL, 18)) {
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