#ifndef UI_H // Prevent the header file from being included multiple times
#define UI_H // Define the header guard macro

#include "raylib.h" // Include the Raylib graphics library for shapes, colors, and fonts
#include <string>   // Include the standard string library for text handling
#include <sstream>  // Include string stream library for text wrapping and parsing

// Namespace containing color themes used throughout the user interface
namespace Theme {
    const Color BG_DARK = { 24, 26, 36, 255 };      // Dark background color for screens
    const Color PANEL_BG = { 38, 42, 58, 255 };     // Background color for UI cards and panels
    const Color BTN_PRIMARY = { 65, 115, 225, 255 };// Primary blue color for active buttons
    const Color BTN_HOVER = { 90, 145, 250, 255 };  // Brighter blue color when hovering over buttons
    const Color BTN_SECONDARY = { 60, 68, 90, 255 };// Neutral grey-blue color for secondary buttons
    const Color SUCCESS_COL = { 46, 184, 92, 255 }; // Green color for correct states and success banners
    const Color ERROR_COL = { 220, 65, 65, 255 };   // Red color for errors, crashes, and delete actions
    const Color WARNING_COL = { 240, 165, 45, 255 };// Orange/Yellow color for warnings and highlights
    const Color TEXT_LIGHT = { 240, 242, 248, 255 };// Bright off-white color for readable primary text
    const Color TEXT_MUTED = { 165, 175, 195, 255 };// Muted grey color for secondary descriptions and subtitles
}

// Global custom font object and character spacing configuration
inline Font AppFont = { 0 };               // Holds the loaded custom font data
inline const float FONT_SPACING = 1.0f;    // Defines the default space distance between letters

// Loads "font.ttf" from the project folder, or falls back to system fonts / default font
inline void InitAppFont() {
    if (FileExists("font.ttf")) {
        AppFont = LoadFontEx("font.ttf", 64, nullptr, 0); // Load custom font file from local directory
    }
    else if (FileExists("C:/Windows/Fonts/segoeui.ttf")) {
        AppFont = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 64, nullptr, 0); // Fallback to Windows Segoe UI
    }
    else if (FileExists("C:/Windows/Fonts/arial.ttf")) {
        AppFont = LoadFontEx("C:/Windows/Fonts/arial.ttf", 64, nullptr, 0); // Fallback to Windows Arial
    }
    else {
        AppFont = GetFontDefault(); // Fallback to Raylib's built-in default font
    }

    // Enable smooth anti-aliased scaling for the custom font texture
    SetTextureFilter(AppFont.texture, TEXTURE_FILTER_BILINEAR);
}

// Unloads the custom font from memory when the application closes (if it's not the default font)
inline void UnloadAppFont() {
    if (AppFont.texture.id != GetFontDefault().texture.id) {
        UnloadFont(AppFont);
    }
}

// Drop-in replacement for Raylib's DrawText using the loaded custom font
inline void DrawCustomText(const char* text, int x, int y, int fontSize, Color color) {
    DrawTextEx(AppFont, text, { (float)x, (float)y }, (float)fontSize, FONT_SPACING, color);
}

// Drop-in replacement for Raylib's MeasureText using the loaded custom font
inline float MeasureCustomText(const char* text, int fontSize) {
    return MeasureTextEx(AppFont, text, (float)fontSize, FONT_SPACING).x;
}

// Draws an interactive rounded button and returns true if the user clicks it with the left mouse button
inline bool DrawButton(Rectangle bounds, const char* text, Color baseColor = Theme::BTN_PRIMARY, int fontSize = 20) {
    Vector2 mouse = GetMousePosition();                          // Get the current mouse coordinates on screen
    bool hovered = CheckCollisionPointRec(mouse, bounds);        // Check if the mouse is hovering over the button bounds
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT); // Check if button was clicked and released

    // Adjust button color brightness depending on whether it is hovered or being clicked down
    Color drawCol = hovered ? ColorBrightness(baseColor, 0.18f) : baseColor;
    if (hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        drawCol = ColorBrightness(baseColor, -0.15f);
    }

    DrawRectangleRounded(bounds, 0.25f, 8, drawCol);                 // Draw the button body with rounded corners
    DrawRectangleRoundedLines(bounds, 0.25f, 8, ColorBrightness(drawCol, 0.3f)); // Draw the button outline border

    // Measure text size to center it perfectly inside the button rectangle
    Vector2 textSize = MeasureTextEx(AppFont, text, (float)fontSize, FONT_SPACING);
    DrawTextEx(AppFont, text,
        { bounds.x + (bounds.width - textSize.x) / 2.0f, bounds.y + (bounds.height - textSize.y) / 2.0f },
        (float)fontSize, FONT_SPACING, Theme::TEXT_LIGHT);

    return clicked; // Return true if the button was clicked
}

// Draws text horizontally centered at a given Y coordinate on the screen
inline void DrawCenteredText(const char* text, int y, int fontSize, Color color = Theme::TEXT_LIGHT) {
    float w = MeasureCustomText(text, fontSize); // Measure text width using custom font
    DrawTextEx(AppFont, text, { (GetScreenWidth() - w) / 2.0f, (float)y }, (float)fontSize, FONT_SPACING, color);
}

// Draws multi-line text wrapped automatically to fit inside a specified bounding rectangle
inline void DrawTextWrapped(const std::string& text, Rectangle bounds, int fontSize, Color color) {
    std::istringstream linesStream(text); // Stream to process text paragraph by paragraph
    std::string paragraph;
    float curY = bounds.y;                 // Track the current vertical Y position for drawing lines

    while (std::getline(linesStream, paragraph, '\n')) {
        if (paragraph.empty()) {
            curY += fontSize + 6; // Add empty line spacing if paragraph is empty
            continue;
        }
        std::istringstream wordsStream(paragraph); // Stream to process words in the paragraph
        std::string word;
        std::string currentLine;

        while (wordsStream >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            // Check if the line exceeds the width of the bounding box; wrap to next line if it does
            if (MeasureCustomText(testLine.c_str(), fontSize) > bounds.width) {
                DrawCustomText(currentLine.c_str(), (int)bounds.x, (int)curY, fontSize, color);
                curY += fontSize + 6;
                currentLine = word;
            }
            else {
                currentLine = testLine;
            }
        }
        if (!currentLine.empty()) {
            DrawCustomText(currentLine.c_str(), (int)bounds.x, (int)curY, fontSize, color);
            curY += fontSize + 6;
        }
    }
}

// Draws the animated verification car used in the Constructor code evaluation screen
inline void DrawCar(float x, float y, bool crashed) {
    Color bodyColor = crashed ? Theme::ERROR_COL : Theme::WARNING_COL; // Red if crashed, orange if running normally

    // Draw the lower chassis/body of the car
    DrawRectangleRounded({ x, y + 12, 64, 20 }, 0.3f, 6, bodyColor);
    // Draw the upper cabin/roof of the car
    DrawRectangleRounded({ x + 12, y, 36, 16 }, 0.3f, 6, bodyColor);
    // Draw the car's front windshield glass window
    DrawRectangleRounded({ x + 32, y + 3, 13, 10 }, 0.2f, 4, SKYBLUE);

    // Draw the front and rear wheels of the car
    DrawCircle((int)x + 15, (int)y + 32, 8, BLACK);
    DrawCircle((int)x + 15, (int)y + 32, 4, LIGHTGRAY);
    DrawCircle((int)x + 49, (int)y + 32, 8, BLACK);
    DrawCircle((int)x + 49, (int)y + 32, 4, LIGHTGRAY);

    // Draw smoke effects, sparks, and an exclamation mark if the car has crashed into a bug
    if (crashed) {
        DrawCircle((int)x + 56, (int)y + 10, 9, DARKGRAY);
        DrawCircle((int)x + 64, (int)y - 2, 12, GRAY);
        DrawCustomText("!", (int)x + 26, (int)y - 26, 26, Theme::ERROR_COL);
    }
}

#endif // UI_H // End of header guard