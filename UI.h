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

#endif // UI_H