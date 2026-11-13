#ifndef CONTENT_DATA_H
#define CONTENT_DATA_H

#include "AppTypes.h"

struct LanguageDatabase {
    std::vector<TheoryTopic> theoryTopics;
    std::vector<QuizQuestion> quizQuestions;
    std::vector<ConstructorLevel> constructorLevels;
    std::vector<VisualizerLevel> visualizerLevels;
};

inline LanguageDatabase GetDatabase(TargetLang lang) {
    LanguageDatabase db;

    if (lang == TargetLang::LANG_C) {
        // ==================== C LANGUAGE CONTENT ====================
        db.theoryTopics = {
            {"1. Basics & Syntax", {
                {"Entry Point", "Every C program starts execution from the main() function:\n\nint main(void) {\n    return 0;\n}"},
                {"Standard I/O", "To print text to the screen in C, include <stdio.h> and use the printf() function."},
                {"Format Specifiers", "In printf, use %d for integers, %f for floats, and %s for strings.\nExample: printf(\"Age: %d\", 20);"},
                {"Comments", "Use // for single-line comments and /* ... */ for multi-line comments in C99 and later."}
            }},
            {"2. Pointers & Memory", {
                {"What is a Pointer?", "A pointer is a variable that stores the memory address of another variable.\nExample: int *p = &x;"},
                {"Dereferencing", "Use the asterisk (*) operator to access the value stored at the memory address.\nExample: *p = 10;"},
                {"Dynamic Memory", "Include <stdlib.h> to allocate memory dynamically on the heap using malloc() and free it using free()."}
            }},
            {"3. Control Flow", {
                {"If-Else Statement", "Used for conditional execution:\nif (x > 0) {\n    printf(\"Positive\");\n}"},
                {"For Loop", "Repeats a block of code a known number of times:\nfor (int i = 0; i < 5; i++) { ... }"},
                {"While Loop", "Repeats code while a condition remains true:\nwhile (condition) { ... }"}
            }}
        };

        db.quizQuestions = {
            {"Which header file is required for printf() in C?", {"<iostream>", "<stdio.h>", "<stdlib.h>", "<string.h>"}, 1},
            {"Which format specifier prints an integer in C?", {"%s", "%f", "%d", "%c"}, 2},
            {"Which operator retrieves the memory address of a variable?", {"*", "&", "->", "#"}, 1},
            {"Which function deallocates heap memory in C?", {"delete", "remove()", "free()", "dealloc()"}, 2}
        };

        db.constructorLevels = {
            {
                "Level 1: Hello World in C",
                {
                    "#include <stdio.h>",
                    "int main() {",
                    "    printf(\"Hello, C!\");",
                    "    return 0;",
                    "}"
                },
                {
                    "Header <stdio.h> must come first to provide printf().",
                    "The main() function signature starts the program execution.",
                    "printf() outputs the greeting inside the main block.",
                    "return 0 signals that the program ended successfully.",
                    "Closing brace '}' terminates the main() function."
                },
                {"cout << \"Hello\";"}
            },
            {
                "Level 2: Pointer Assignment",
                {
                    "int x = 42;",
                    "int *ptr = &x;",
                    "*ptr = 100;",
                    "printf(\"%d\", x);"
                },
                {
                    "First, declare and initialize the integer variable 'x'.",
                    "Declare pointer 'ptr' and assign it the address of 'x'.",
                    "Dereference 'ptr' to modify the value of 'x' to 100.",
                    "Print the updated value of 'x' using printf."
                },
                {"int &ptr = *x;"}
            }
        };

        db.visualizerLevels = {
            {
                "Level 1: Swapping with Pointers",
                "#include <stdio.h>\n\nint main() {\n    int a = 5;\n    int *p = &a;\n    *p = 20;\n    printf(\"%d\", a);\n    return 0;\n}",
                {
                    {"#include <stdio.h>", "Includes the Standard Input/Output library for printf()."},
                    {"int main() {", "Defines the main entry point of the C program."},
                    {"    int a = 5;", "Allocates an integer variable 'a' on the stack and sets it to 5."},
                    {"    int *p = &a;", "Creates a pointer 'p' that stores the exact memory address of 'a'."},
                    {"    *p = 20;", "Dereferences 'p', going to the memory of 'a' and changing its value to 20."},
                    {"    printf(\"%d\", a);", "Prints the value of 'a' (which is now 20) to the console."},
                    {"    return 0;", "Exits the program with status code 0 (success)."},
                    {"}", "Closes the main function scope."}
                }
            }
        };
    }
    else {
        // ==================== C++ LANGUAGE CONTENT ====================
        db.theoryTopics = {
            {"1. C++ Basics & Streams", {
                {"I/O Streams", "C++ uses <iostream> and stream objects std::cout (output) and std::cin (input)."},
                {"Namespaces", "A namespace groups identifiers. 'std::cout' accesses 'cout' inside the standard ('std') namespace."},
                {"References", "A reference (&) is an alias for an existing variable:\nint a = 5;\nint &ref = a; // ref is now an alias for a"},
                {"Auto Keyword", "Since C++11, 'auto' automatically deduces the type of a variable from its initializer."}
            }},
            {"2. Object-Oriented Programming", {
                {"Classes & Objects", "A class is a blueprint for objects. By default, members of a 'class' are private, while 'struct' members are public."},
                {"Constructors", "A special method with the same name as the class, automatically called when an object is instantiated."},
                {"Encapsulation", "Hiding internal object details using 'private:' and exposing an interface via 'public:' methods."}
            }},
            {"3. STL & Modern C++", {
                {"std::vector", "A dynamic array from <vector> that resizes automatically when elements are added via push_back()."},
                {"std::string", "A safe, flexible string class from <string> that replaces raw C-style char arrays."},
                {"Smart Pointers", "std::unique_ptr and std::shared_ptr manage dynamic memory automatically, preventing memory leaks."}
            }}
        };

        db.quizQuestions = {
            {"Which operator is used with std::cout to output data?", {">>", "<<", "::", "->"}, 1},
            {"What is the default access modifier for members of a C++ class?", {"public", "protected", "private", "global"}, 2},
            {"Which STL container acts as a dynamic array?", {"std::map", "std::list", "std::vector", "std::set"}, 2},
            {"How do you add an element to the end of a std::vector 'v'?", {"v.add(x)", "v.push_back(x)", "v.append(x)", "v.insert_end(x)"}, 1}
        };

        db.constructorLevels = {
            {
                "Level 1: Hello World in C++",
                {
                    "#include <iostream>",
                    "int main() {",
                    "    std::cout << \"Hello, C++!\" << std::endl;",
                    "    return 0;",
                    "}"
                },
                {
                    "Include <iostream> to gain access to std::cout.",
                    "Start the main() execution entry point.",
                    "Use the insertion operator (<<) to send text to std::cout.",
                    "Return 0 to indicate the program ran without errors.",
                    "Close the main() function block with '}'."
                },
                {"printf(\"Hello\");"}
            },
            {
                "Level 2: Using std::vector",
                {
                    "#include <vector>",
                    "int main() {",
                    "    std::vector<int> nums;",
                    "    nums.push_back(10);",
                    "    return 0;",
                    "}"
                },
                {
                    "Include the <vector> header to use dynamic arrays.",
                    "Open the main() function.",
                    "Instantiate an empty vector of integers named 'nums'.",
                    "Append the integer 10 to the back of the vector.",
                    "Return 0 at the end of main.",
                    "Close the main function scope."
                },
                {}
            }
        };

        db.visualizerLevels = {
            {
                "Level 1: Classes & Objects",
                "class Player {\npublic:\n    int hp = 100;\n    void heal() { hp += 20; }\n};\n\nint main() {\n    Player hero;\n    hero.heal();\n    return 0;\n}",
                {
                    {"class Player {", "Declares a new class named 'Player'."},
                    {"public:", "Access specifier making all following members accessible from outside the class."},
                    {"    int hp = 100;", "Member variable 'hp' initialized to 100 by default for every Player instance."},
                    {"    void heal() { hp += 20; }", "Member function (method) that increases the object's hp by 20."},
                    {"};", "Ends the class definition (note the mandatory semicolon in C++!)."},
                    {"int main() {", "Starts the main program execution."},
                    {"    Player hero;", "Instantiates an object named 'hero' of type 'Player'."},
                    {"    hero.heal();", "Calls the heal() method on 'hero', raising hero.hp to 120."},
                    {"    return 0;", "Exits main() successfully."},
                    {"}", "Closes the main function."}
                }
            }
        };
    }

    return db;
}

#endif // CONTENT_DATA_H