// Purpose of UI.h: Defines the application's color theme, font initialization and cleanup routines, text measurement and wrapping utilities, interactive button rendering, and custom vector icon drawing helpers (flame, heart, and car) built on top of Raylib.

#ifndef UI_H // Guard against multiple inclusions of this header file
#define UI_H // Define the UI_H include guard macro

#include "raylib.h" // Include Raylib graphics, math, and input API
#include <string> // Include std::string for text handling
#include <sstream> // Include std::istringstream for splitting text into lines and words
#include <vector> // Include std::vector for storing wrapped text lines

namespace Theme { // Group all UI color palette constants inside the Theme namespace
    const Color BG_DARK = { 24, 26, 36, 255 }; // Define primary dark background color
    const Color SIDEBAR_BG = { 30, 33, 46, 255 }; // Define slightly lighter background color for sidebars and containers
    const Color PANEL_BG = { 38, 42, 58, 255 }; // Define card and input panel fill color
    const Color BTN_PRIMARY = { 65, 115, 225, 255 }; // Define default primary button blue color
    const Color BTN_HOVER = { 90, 145, 250, 255 }; // Define lighter accent blue for hover or highlights
    const Color BTN_SECONDARY = { 60, 68, 90, 255 }; // Define muted slate color for secondary buttons and borders
    const Color SUCCESS_COL = { 46, 184, 92, 255 }; // Define green color for success states and correct answers
    const Color ERROR_COL = { 220, 65, 65, 255 }; // Define red color for errors, crashes, and lost lives
    const Color WARNING_COL = { 240, 165, 45, 255 }; // Define amber color for XP, warnings, and highlights
    const Color TEXT_LIGHT = { 240, 242, 248, 255 }; // Define near-white color for primary readable text
    const Color TEXT_MUTED = { 165, 175, 195, 255 }; // Define soft gray-blue color for labels and secondary text
}

inline Font AppFont = { 0 }; // Declare global inline font handle shared across translation units
inline const float FONT_SPACING = 1.0f; // Set constant letter spacing used for all custom text rendering

inline void InitAppFont() { // Load a crisp TrueType UI font with fallbacks to system or default fonts
    if (FileExists("font.ttf")) { // Check if a custom local font file exists in the working directory
        AppFont = LoadFontEx("font.ttf", 64, nullptr, 0); // Load local font at size 64 for clean scaling
    }
    else if (FileExists("C:/Windows/Fonts/segoeui.ttf")) { // Fallback check for Windows Segoe UI font
        AppFont = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 64, nullptr, 0); // Load Segoe UI font at size 64
    }
    else if (FileExists("C:/Windows/Fonts/arial.ttf")) { // Fallback check for Windows Arial font
        AppFont = LoadFontEx("C:/Windows/Fonts/arial.ttf", 64, nullptr, 0); // Load Arial font at size 64
    }
    else { // Handle cases where no custom or system font file is found
        AppFont = GetFontDefault(); // Fall back to Raylib's built-in bitmap font
    }
    SetTextureFilter(AppFont.texture, TEXTURE_FILTER_BILINEAR); // Apply bilinear filtering so scaled text appears smooth
}

inline void UnloadAppFont() { // Release GPU resources allocated for the custom application font
    if (AppFont.texture.id != GetFontDefault().texture.id) { // Ensure we do not unload Raylib's default built-in font
        UnloadFont(AppFont); // Free the loaded custom font from memory
    }
}

inline void DrawCustomText(const char* text, int x, int y, int fontSize, Color color) { // Render text using the loaded application font
    DrawTextEx(AppFont, text, { (float)x, (float)y }, (float)fontSize, FONT_SPACING, color); // Draw text at integer coordinates converted to floats
}

inline float MeasureCustomText(const char* text, int fontSize) { // Measure the horizontal pixel width of a text string
    return MeasureTextEx(AppFont, text, (float)fontSize, FONT_SPACING).x; // Return only the X width component from Raylib's measurement
}

inline std::vector<std::string> WrapTextLines(const std::string& text, float maxWidth, int fontSize) { // Split text into wrapped lines fitting within maxWidth
    std::vector<std::string> lines; // Store the resulting list of wrapped text lines
    std::istringstream linesStream(text); // Create an input string stream to read explicit paragraphs
    std::string paragraph; // Hold each newline-delimited paragraph

    while (std::getline(linesStream, paragraph, '\n')) { // Read text paragraph by paragraph on newline characters
        if (paragraph.empty()) { // Preserve blank lines from consecutive newlines
            lines.push_back(""); // Append an empty string to represent a vertical line break
            continue; // Move on to the next paragraph
        }
        std::istringstream wordsStream(paragraph); // Create a stream to split the paragraph into whitespace-separated words
        std::string word; // Hold each extracted word
        std::string currentLine; // Accumulate words that fit on the current line

        while (wordsStream >> word) { // Read each word from the paragraph
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word; // Form candidate line with the next word appended
            if (!currentLine.empty() && MeasureCustomText(testLine.c_str(), fontSize) > maxWidth) { // Check if adding the word exceeds maxWidth
                lines.push_back(currentLine); // Commit the completed line to the output list
                currentLine = word; // Start a new line with the overflowing word
            }
            else { // Keep the word on the current line if it fits
                currentLine = testLine; // Update the current line with the candidate text
            }
        }
        if (!currentLine.empty()) { // Check if any remaining text is left in the last line of the paragraph
            lines.push_back(currentLine); // Push the final line of the paragraph
        }
    }
    return lines; // Return the full collection of wrapped lines
}

inline float MeasureTextWrappedHeight(const std::string& text, float maxWidth, int fontSize) { // Calculate total pixel height required to render wrapped text
    std::vector<std::string> lines = WrapTextLines(text, maxWidth, fontSize); // Wrap the text using the target width and font size
    if (lines.empty()) return (float)fontSize; // Return a single line height if the text is empty
    return (float)lines.size() * (fontSize + 6) - 4.0f; // Multiply line count by line pitch and trim trailing padding
}

inline void DrawTextFitWidth(const char* text, int x, int y, int maxFontSize, float maxWidth, Color color) { // Draw single-line text shrunk if needed to stay within maxWidth
    int fs = maxFontSize; // Start testing from the requested maximum font size
    while (fs > 12 && MeasureCustomText(text, fs) > maxWidth) { // Reduce font size until the text fits or reaches minimum size 12
        fs--; // Decrement font size by 1 pixel
    }
    float yOffset = (maxFontSize - fs) * 0.45f; // Calculate slight vertical offset to keep shrunk text vertically centered
    DrawCustomText(text, x, (int)(y + yOffset), fs, color); // Render the fitted text at the adjusted position
}

inline bool DrawButton(Rectangle bounds, const char* text, Color baseColor = Theme::BTN_PRIMARY, int fontSize = 20) { // Render an interactive button and return true if clicked
    Vector2 mouse = GetMousePosition(); // Get current mouse cursor position
    bool hovered = CheckCollisionPointRec(mouse, bounds); // Check whether the cursor is inside the button rectangle
    bool clicked = hovered && IsMouseButtonReleased(MOUSE_BUTTON_LEFT); // Detect a click when left mouse button is released over the button

    Color drawCol = hovered ? ColorBrightness(baseColor, 0.18f) : baseColor; // Brighten the button fill slightly when hovered
    if (hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) { // Check if the button is actively being pressed down
        drawCol = ColorBrightness(baseColor, -0.15f); // Darken the button fill for pressed visual feedback
    }

    DrawRectangleRounded(bounds, 0.22f, 8, drawCol); // Draw rounded button background
    DrawRectangleRoundedLines(bounds, 0.22f, 8, ColorBrightness(drawCol, 0.3f)); // Draw lighter rounded outline around the button

    float paddingX = 20.0f; // Set combined horizontal inner padding for button text
    float maxTextW = bounds.width - paddingX; // Compute maximum allowable width for button label text
    Vector2 textSize = MeasureTextEx(AppFont, text, (float)fontSize, FONT_SPACING); // Measure single-line dimensions of the button label

    if (textSize.x <= maxTextW) { // Render directly centered if the label fits on a single line
        DrawTextEx(AppFont, text, // Draw centered single-line button label
            { bounds.x + (bounds.width - textSize.x) / 2.0f, bounds.y + (bounds.height - textSize.y) / 2.0f }, // Center text horizontally and vertically inside bounds
            (float)fontSize, FONT_SPACING, Theme::TEXT_LIGHT); // Apply font size, spacing, and light text color
    }
    else { // Wrap and shrink long labels so they fit neatly inside the button
        int fs = fontSize; // Start wrapping at the requested font size
        std::vector<std::string> lines = WrapTextLines(text, maxTextW, fs); // Wrap label text to the button's inner width
        float lineH = (float)(fs + 4); // Calculate height per wrapped line
        while (fs > 14 && lines.size() * lineH > bounds.height - 12.0f) { // Shrink font size if wrapped lines exceed button height
            fs -= 2; // Reduce font size by 2 pixels per step
            lines = WrapTextLines(text, maxTextW, fs); // Re-wrap text at the smaller font size
            lineH = (float)(fs + 4); // Recalculate line height
        }
        float totalH = lines.size() * lineH - 4.0f; // Compute combined height of all wrapped lines
        float startY = bounds.y + (bounds.height - totalH) / 2.0f; // Compute starting Y coordinate to vertically center the text block
        for (size_t i = 0; i < lines.size(); ++i) { // Iterate through each wrapped line
            float lw = MeasureCustomText(lines[i].c_str(), fs); // Measure width of the current line for horizontal centering
            DrawTextEx(AppFont, lines[i].c_str(), // Render the wrapped line
                { bounds.x + (bounds.width - lw) / 2.0f, startY + i * lineH }, // Center the line horizontally at its vertical row offset
                (float)fs, FONT_SPACING, Theme::TEXT_LIGHT); // Apply adjusted font size, spacing, and light text color
        }
    }

    return clicked; // Return true on the frame the button was clicked
}

inline void DrawCenteredText(const char* text, int y, int fontSize, Color color = Theme::TEXT_LIGHT) { // Draw text horizontally centered across the window
    float w = MeasureCustomText(text, fontSize); // Measure pixel width of the string
    DrawTextEx(AppFont, text, { (GetScreenWidth() - w) / 2.0f, (float)y }, (float)fontSize, FONT_SPACING, color); // Draw text centered at the given Y coordinate
}

inline void DrawTextWrapped(const std::string& text, Rectangle bounds, int fontSize, Color color) { // Render multi-line wrapped text inside a bounding rectangle
    std::vector<std::string> lines = WrapTextLines(text, bounds.width, fontSize); // Break input text into lines matching bounds width
    float curY = bounds.y; // Initialize current drawing Y position to top of bounds
    for (const auto& line : lines) { // Loop over each wrapped line
        if (!line.empty()) { // Skip drawing calls for blank spacing lines
            DrawCustomText(line.c_str(), (int)bounds.x, (int)curY, fontSize, color); // Render the line at the left edge and current Y position
        }
        curY += fontSize + 6; // Advance Y position downward by one line pitch
    }
}

inline void DrawFlameIcon(float cx, float cy, float scale, bool active) { // Render a stylized flame icon representing the user's daily streak
    Color outerCol = active ? Color{ 255, 110, 20, 255 } : Theme::BTN_SECONDARY; // Use orange for active streak or muted slate when inactive
    Color innerCol = active ? Color{ 255, 215, 50, 255 } : Theme::TEXT_MUTED; // Use bright yellow for active inner flame or muted gray

    DrawCircle((int)cx, (int)(cy + 4 * scale), 10 * scale, outerCol); // Draw rounded base of the outer flame
    DrawTriangle( // Draw pointed upper tip of the outer flame
        { cx, cy - 14 * scale }, // Top vertex of outer flame triangle
        { cx - 10 * scale, cy + 4 * scale }, // Bottom-left vertex of outer flame triangle
        { cx + 10 * scale, cy + 4 * scale }, // Bottom-right vertex of outer flame triangle
        outerCol // Fill outer triangle with outer flame color
    );
    DrawCircle((int)cx, (int)(cy + 5 * scale), 5 * scale, innerCol); // Draw rounded base of the inner flame core
    DrawTriangle( // Draw pointed upper tip of the inner flame core
        { cx, cy - 5 * scale }, // Top vertex of inner flame triangle
        { cx - 5 * scale, cy + 5 * scale }, // Bottom-left vertex of inner flame triangle
        { cx + 5 * scale, cy + 5 * scale }, // Bottom-right vertex of inner flame triangle
        innerCol // Fill inner triangle with inner flame color
    );
}

inline void DrawHeartIcon(float cx, float cy, float scale, bool filled) { // Render a stylized heart icon representing a daily life
    Color col = filled ? Theme::ERROR_COL : Theme::BTN_SECONDARY; // Use red for an active life or muted slate for a lost life
    DrawCircle((int)(cx - 4.5f * scale), (int)(cy - 2 * scale), 5 * scale, col); // Draw left upper lobe of the heart
    DrawCircle((int)(cx + 4.5f * scale), (int)(cy - 2 * scale), 5 * scale, col); // Draw right upper lobe of the heart
    DrawTriangle( // Draw pointed lower half of the heart
        { cx - 9.2f * scale, cy }, // Top-left vertex connecting to the left lobe
        { cx + 9.2f * scale, cy }, // Top-right vertex connecting to the right lobe
        { cx, cy + 10 * scale }, // Bottom point of the heart
        col // Fill lower triangle with the heart color
    );
}

inline void DrawCar(float x, float y, bool crashed) { // Render the Code Constructor validation car sprite
    Color bodyColor = crashed ? Theme::ERROR_COL : Theme::WARNING_COL; // Paint car red if crashed or amber during normal driving

    DrawRectangleRounded({ x, y + 12, 64, 20 }, 0.3f, 6, bodyColor); // Draw lower chassis body of the car
    DrawRectangleRounded({ x + 12, y, 36, 16 }, 0.3f, 6, bodyColor); // Draw upper cabin roof of the car
    DrawRectangleRounded({ x + 32, y + 3, 13, 10 }, 0.2f, 4, SKYBLUE); // Draw front windshield window

    DrawCircle((int)x + 15, (int)y + 32, 8, BLACK); // Draw rear tire
    DrawCircle((int)x + 15, (int)y + 32, 4, LIGHTGRAY); // Draw rear wheel hubcap
    DrawCircle((int)x + 49, (int)y + 32, 8, BLACK); // Draw front tire
    DrawCircle((int)x + 49, (int)y + 32, 4, LIGHTGRAY); // Draw front wheel hubcap

    if (crashed) { // Render crash visual effects when the car hits a code error
        DrawCircle((int)x + 56, (int)y + 10, 9, DARKGRAY); // Draw lower smoke puff near the hood
        DrawCircle((int)x + 64, (int)y - 2, 12, GRAY); // Draw rising upper smoke puff
        DrawCustomText("!", (int)x + 26, (int)y - 26, 26, Theme::ERROR_COL); // Draw red exclamation mark above the crashed car
    }
}

#endif // End of UI_H include guard