# C / C++ Interactive Learning App

An interactive desktop learning application built with **Raylib** that teaches **C** and **C++** from fundamentals to advanced topics through flashcards, quizzes, drag-and-drop code construction, and step-by-step code walkthroughs.

![Dashboard Preview](<img width="602" height="424" alt="image" src="https://github.com/user-attachments/assets/156380b2-f569-4a2b-a540-2a03d65f2d88" />)

---

## Features

- **Dual-Language Curriculum**: Switch seamlessly between **C** (9 topics) and **C++** (10 topics) from the main sidebar.
- **4 Topic-Based Learning Modes**:
  - **Theory**: Structured flashcard lessons with code examples (`+20 XP` per completed topic and quick jump to the matching quiz).
  - **Quiz**: 4-option multiple-choice quizzes (`+15 XP` per correct answer). Wrong answers update your **Needs Practice** focus topic, and finishing with a score of `0` costs 1 Daily Life.
  - **Code Constructor**: Drag-and-drop coding labs with distractor blocks. Click **Ready (Run Car)** to test your solution—an animated car drives through each line, awarding `+50 XP` on success or crashing with a scrollable line-by-line error report on failure.
  - **Code Visualizer**: Preview the full lab solution, then step through each line interactively with toggleable explanations (`+5 XP` per step).
- **Gamification & Local Persistence**:
  - User registration and login stored locally in `users.txt`.
  - **Daily Streak** tracker and **Daily Lives** (resets to `5/5` each new day).
  - **Needs Practice** recommendation card.
  - **Diamond League Leaderboard** ranking top learners by XP.

---

## Screenshots

| Theory Flashcards | Quiz Mode |
| :---: | :---: |
| ![Theory](<img width="602" height="424" alt="image" src="https://github.com/user-attachments/assets/82d9f464-b01c-47a2-b30c-5e65cdab33a3" />) | ![Quiz](<img width="602" height="424" alt="image" src="https://github.com/user-attachments/assets/1844a985-6324-4ba2-8756-c261c59ceb1d" />) |

| Code Constructor | Code Visualizer |
| :---: | :---: |
| ![Code Constructor](<img width="602" height="424" alt="image" src="https://github.com/user-attachments/assets/5b4fc333-06cd-4663-a43e-8f2103a11d81" />) | ![Code Visualizer](<img width="602" height="424" alt="image" src="https://github.com/user-attachments/assets/5cb46f45-d43d-46ca-8e04-0e108381fcdc" />) |

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
| | 10. STL Lists, Maps & Smart Pointers |

---

## Project Structure

```text
.
├── main.cpp          # Application loop, screen routing, user persistence, and game logic
├── AppTypes.h        # Enums, data models, and global AppState struct
├── ContentData.h     # Complete C & C++ curriculum (Theory, Quiz, Constructor, Visualizer)
├── UI.h              # Dark theme palette, font loading, text wrapping, buttons, and icons
├── users.txt         # Auto-generated local user database 
├── font.ttf          # (Optional) Custom TrueType font placed in the working directory
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
1. Create a new **Empty C++ Project** and add `main.cpp`, `AppTypes.h`, `ContentData.h`, and `UI.h`.
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
