# C / C++ Interactive Learning App

An interactive desktop learning application built with **Raylib** that teaches **C** and **C++** from fundamentals to advanced topics through flashcards, quizzes, drag-and-drop code construction, step-by-step code walkthroughs, hands-on coding practice, and graded exams.

---

## Features

- **Dual-Language Curriculum**: Switch seamlessly between **C** (9 topics) and **C++** (10 topics) from the main sidebar.
- **6 Topic-Based Learning Modes**:
  - **Theory**: Structured flashcard lessons with code examples (`+20 XP` per completed topic and quick jump to the matching quiz).
  - **Quiz**: 4-option multiple-choice quizzes (`+15 XP` per correct answer). Wrong answers update your **Needs Practice** focus topic, and finishing with a score of `0` costs 1 Daily Life.
  - **Code Constructor**: Drag-and-drop coding labs with distractor blocks. Click **Ready (Run Car)** to test your solution—an animated car drives through each line, awarding `+50 XP` on success or crashing with a scrollable line-by-line error report on failure.
  - **Code Visualizer**: Preview the full lab solution, then step through each line interactively with toggleable explanations (`+5 XP` per step).
  - **Practice**: Hands-on multi-line code editor with line numbers, Tab indentation, and automated syntax/rule checking (validates balanced braces, parentheses, quotes, semicolons, and required code constructs). Awards `+40 XP` with expected output on success, or displays scrollable diagnostic feedback on failure.
  - **Exam Mode**: Sequential topic assessments combining multiple-choice and text-input questions with no hints until completion. Awards `+20 XP` per correct answer, assigns a percentage and letter grade (`A`–`F`), flags topics scored below `50%` as **Needs Practice**, and includes a scrollable **Result Review**.
- **Gamification, Profile & Local Persistence**:
  - User registration and login stored locally in `users.txt`.
  - **Daily Streak** tracker and **Daily Lives** (resets to `5/5` each new day).
  - **Needs Practice** recommendation card.
  - **Diamond League Leaderboard** ranking top learners by XP.
  - **Profile View**: Toggle between the Leaderboard and your Profile from the sidebar to view your XP level, leaderboard rank, completed labs/tasks, and curriculum overview, or **Edit Username** and **Delete Account** via confirmation modals.

---

## Curriculum Overview

| C Track (9 Topics) | C++ Track (10 Topics) |
| :--- | :--- |
| 1. What is C? Basics & Keywords | 1. Classes, Objects & Structs |
| 2. Input, Output & Math | 2. Constructors, Copy & Destructors |
| 3. Decisions & Loops | 3. Memory: Stack, Heap & `new`/`delete` |
| 4. Arrays & 2D Grids | 4. Singly & Doubly Linked Lists |
| 5. C-Strings & Null Terminator | 5. Inheritance & Polymorphism |
| 6. Functions & Parameters | 6. Abstract, Interface & Friends |
| 7. Pointers & Pointer Math | 7. Operator Overloading |
| 8. Heap Memory & Func Pointers | 8. Templates (Generic Code) |
| 9. Linux `fork`/`pipe` & Bit Ops | 9. STL Vectors, Iterators & Algos |
| | 10. STL Lists, Maps & Smart Ptrs |

---

## Project Structure

```text
.
├── main.cpp            # Application loop, screen routing, user persistence, and game logic
├── AppTypes.h          # Enums, data models, and global AppState struct
├── ContentData.h       # C & C++ Theory, Quiz, Constructor, and Visualizer curriculum
├── PracticeExamData.h  # C & C++ Practice Mode tasks/rules and Exam Mode questions
├── UI.h                # Dark theme palette, font loading, text wrapping, buttons, and icons
├── users.txt           # Auto-generated local user database
└── font.ttf            # (Optional) Custom TrueType font placed in the working directory
```

---

## Getting Started

### Prerequisites
- A **C++17** compatible compiler (MSVC, MinGW-w64 `g++`, or `clang++`)
- **[Raylib](https://www.raylib.com/)** installed and linked

### Build Instructions

#### Windows (MinGW / g++)
```bash
g++ main.cpp -o LearningStudio.exe -std=c++17 -O2 -lraylib -lopengl32 -lgdi32 -lwinmm
./LearningStudio.exe
```

#### Windows (Visual Studio)
1. Create a new **Empty C++ Project** and add `main.cpp`, `AppTypes.h`, `ContentData.h`, `PracticeExamData.h`, and `UI.h`.
2. Set the C++ Language Standard to **ISO C++17 Standard (`/std:c++17`)**.
3. Link `raylib` (via NuGet, vcpkg, or manual include/lib paths) along with `winmm.lib`.
4. Build and run (`Ctrl + F5`).

#### Linux (g++)
```bash
g++ main.cpp -o LearningStudio -std=c++17 -O2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./LearningStudio
```

#### macOS (clang++)
```bash
clang++ main.cpp -o LearningStudio -std=c++17 -O2 -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
./LearningStudio
```

> **Font Note**: On startup, the app checks for `font.ttf` in the working directory, then falls back to Windows system fonts (`segoeui.ttf` / `arial.ttf`), and finally Raylib's default font. On Linux/macOS, drop any `.ttf` file named `font.ttf` next to the executable for sharper typography.

---

## Demo Accounts

If `users.txt` does not exist yet, the app seeds three sample accounts you can use right away (or register your own):

| Username | Password | Starting XP |
| :--- | :---: | :---: |
| `Alice_Coder` | `123` | `450` |
| `Bob_CPlusPlus` | `123` | `320` |
| `Charlie_Dev` | `123` | `180` |
