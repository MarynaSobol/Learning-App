#ifndef UI_H
#define UI_H

#include "raylib.h"
#include <string>
#include <sstream>

namespace Theme {
    const Color BG_DARK = { 24, 26, 36, 255 };
    const Color PANEL_BG = { 38, 42, 58, 255 };
    const Color BTN_PRIMARY = { 65, 115, 225, 255 };
    const Color BTN_HOVER = { 90, 145, 250, 255 };
    const Color BTN_SECONDARY = { 60, 68, 90, 255 };
    const Color SUCCESS_COL = { 46, 184, 92, 255 };
    const Color ERROR_COL = { 220, 65, 65, 255 };
    const Color WARNING_COL = { 240, 165, 45, 255 };
    const Color TEXT_LIGHT = { 240, 242, 248, 255 };
    const Color TEXT_MUTED = { 165, 175, 195, 255 };
}

// Draws an interactive rounded button and returns true when clicked
inline bool DrawButton(Rectangle bounds, const char* text, Color baseColor = Theme::BTN_PRIMARY, int fontSize = 20) {
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, bounds);
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);

    Color drawCol = hovered ? ColorBrightness(baseColor, 0.18f) : baseColor;
    if (hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        drawCol = ColorBrightness(baseColor, -0.15f);
    }

    DrawRectangleRounded(bounds, 0.25f, 8, drawCol);
    DrawRectangleRoundedLines(bounds, 0.25f, 8, ColorBrightness(drawCol, 0.3f));

    int textW = MeasureText(text, fontSize);
    DrawText(text,
        (int)(bounds.x + (bounds.width - textW) / 2),
        (int)(bounds.y + (bounds.height - fontSize) / 2),
        fontSize, Theme::TEXT_LIGHT);

    return clicked;
}

// Draws centered text horizontally at a given Y coordinate
inline void DrawCenteredText(const char* text, int y, int fontSize, Color color = Theme::TEXT_LIGHT) {
    int w = MeasureText(text, fontSize);
    DrawText(text, (GetScreenWidth() - w) / 2, y, fontSize, color);
}

// Draws multi-line wrapped text inside a bounding rectangle
inline void DrawTextWrapped(const std::string& text, Rectangle bounds, int fontSize, Color color) {
    std::istringstream linesStream(text);
    std::string paragraph;
    float curY = bounds.y;

    while (std::getline(linesStream, paragraph, '\n')) {
        if (paragraph.empty()) {
            curY += fontSize + 6;
            continue;
        }
        std::istringstream wordsStream(paragraph);
        std::string word;
        std::string currentLine;

        while (wordsStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            if (MeasureText(testLine.c_str(), fontSize) > bounds.width) {
                DrawText(currentLine.c_str(), (int)bounds.x, (int)curY, fontSize, color);
                curY += fontSize + 6;
                currentLine = word;
            }
            else {
                currentLine = testLine;
            }
        }
        if (!currentLine.empty()) {
            DrawText(currentLine.c_str(), (int)bounds.x, (int)curY, fontSize, color);
            curY += fontSize + 6;
        }
    }
}

// Draws the animated verification car in the Constructor screen
inline void DrawCar(float x, float y, bool crashed) {
    Color bodyColor = crashed ? Theme::ERROR_COL : Theme::WARNING_COL;

    // Car lower chassis
    DrawRectangleRounded({ x, y + 12, 64, 20 }, 0.3f, 6, bodyColor);
    // Car cabin/roof
    DrawRectangleRounded({ x + 12, y, 36, 16 }, 0.3f, 6, bodyColor);
    // Windshield
    DrawRectangleRounded({ x + 32, y + 3, 13, 10 }, 0.2f, 4, SKYBLUE);

    // Wheels
    DrawCircle((int)x + 15, (int)y + 32, 8, BLACK);
    DrawCircle((int)x + 15, (int)y + 32, 4, LIGHTGRAY);
    DrawCircle((int)x + 49, (int)y + 32, 8, BLACK);
    DrawCircle((int)x + 49, (int)y + 32, 4, LIGHTGRAY);

    // Crash smoke / sparks visual indicator
    if (crashed) {
        DrawCircle((int)x + 56, (int)y + 10, 9, DARKGRAY);
        DrawCircle((int)x + 64, (int)y - 2, 12, GRAY);
        DrawText("!", (int)x + 26, (int)y - 26, 26, Theme::ERROR_COL);
    }
}

#endif // UI_H