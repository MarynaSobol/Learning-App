// Purpose: Stores and returns all structured Theory, Quiz, Constructor, and Visualizer curriculum content for C and C++.
#ifndef CONTENT_DATA_H // Prevent multiple inclusions of this header file
#define CONTENT_DATA_H // Define the header guard macro

#include "AppTypes.h" // Include application data structure definitions
#include "PracticeExamData.h"

struct LanguageDatabase { // Structure holding all learning contents for a specific programming language
    std::vector<TheoryTopic> theoryTopics; // List of theory topics and flashcards
    std::vector<QuizTopic> quizTopics; // List of quiz topics and questions
    std::vector<ConstructorTopic> constructorTopics; // List of code constructor topics and levels
    std::vector<VisualizerTopic> visualizerTopics; // List of code visualizer topics and walkthroughs
    std::vector<PracticeTopic> practiceTopics; // Added for Practice Mode
    std::vector<ExamTopic> examTopics;         // Added for Exam Mode
};

inline LanguageDatabase GetDatabase(TargetLang lang) { // Builds and returns the curriculum database for the selected language
    LanguageDatabase db; // Create an empty database instance to populate

    if (lang == TargetLang::LANG_C) { // Check if the active target language is C
        db.theoryTopics = { // Populate all C theory topics and flashcards
            {"1. What is C? Basics & Keywords", { // Topic 1 covering C fundamentals and keywords
                {"What is Programming & C?", "A computer only understands 1s and 0s. A programming language like C lets humans write step-by-step instructions in readable English-like words.\n\nC is a 'compiled' language: a tool called a compiler translates your C code into fast machine code before the program runs."}, // Flashcard introducing programming and compilation
                {"Your First C Program", "Every C program starts running inside a special block called main():\n\n#include <stdio.h>\nint main() {\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n- #include <stdio.h> loads input/output tools.\n- Every command ends with a semicolon (;).\n- return 0 tells the computer 'everything worked!'"}, // Flashcard showing a basic Hello World program
                {"Reserved Words (32 Keywords)", "C reserves 32 special words that you cannot use as your own names, plus the 'sizeof' operator:\n- Data Types (9): int, char, float, double, void, short, long, signed, unsigned\n- Control Flow (11): if, else, switch, case, default, for, while, do, break, continue, return"}, // Flashcard listing data type and control flow keywords
                {"More Reserved Words", "The remaining 12 reserved words control where data lives and how it behaves:\n- Storage (4): auto, register, static, extern\n- Custom Types (4): struct, union, enum, typedef\n- Qualifiers (4): const, volatile, restrict (C99), _Atomic (C11)"}, // Flashcard listing storage, custom type, and qualifier keywords
                {"Variables: Labeled Boxes", "Think of a variable as a labeled storage box in memory. In C, you MUST say what type of data fits in the box before using it:\n- int age = 25;       (whole numbers)\n- float price = 19.99f; (decimals)\n- double pi = 3.14159;  (more precise decimals)\n- char grade = 'A';   (1 character in single quotes)\nWarning: An uninitialized variable holds random leftover garbage!"}, // Flashcard explaining variable declarations and primitive types
                {"Constants (const)", "If a value should NEVER change while the program runs, lock the box with the keyword 'const':\n\nconst double TAX_RATE = 0.08;\nconst int MAX_SCORE = 100;\n\nTrying to change TAX_RATE later causes a compile error, protecting you from mistakes."} // Flashcard explaining read-only constants
            }},
            {"2. Input, Output & Math", { // Topic 2 covering console I/O and arithmetic
                {"Printing with printf()", "printf() displays text on the screen. To insert variable values into text, use placeholders called Format Specifiers:\n- %d for integers (int)\n- %f for decimals (%.2f shows 2 decimal places)\n- %c for a single character\n- %s for text (strings)\nExample: printf(\"Age: %d\\n\", age); (\\n starts a new line)"}, // Flashcard explaining printf and format specifiers
                {"Reading Input: scanf()", "scanf() reads what the user types on the keyboard:\n\nint age;\nscanf(\"%d\", &age);\n\nNotice the '&' before age! It means 'address of'—it tells scanf the exact memory box where the typed number should be stored.\nTip: Put a space before %c (\" %c\") to skip leftover Enter keys."}, // Flashcard explaining keyboard input via scanf
                {"Visual Studio & scanf_s()", "In Visual Studio, standard scanf() may trigger a security warning. Use scanf_s() instead and check if it returned 1 (meaning 1 valid item was read):\n\nif (scanf_s(\"%d\", &age) != 1) {\n    printf(\"Invalid input!\\n\");\n    return 1;\n}"}, // Flashcard explaining secure input with scanf_s
                {"Arithmetic & Modulo (%)", "C supports basic math: + (add), - (subtract), * (multiply), / (divide), and % (remainder/modulo).\n\nExample: 10 % 3 is 1 (because 3 fits into 10 three times with 1 left over).\nShortcuts:\n- count++ adds 1; count-- subtracts 1\n- num += 5 means num = num + 5"}, // Flashcard explaining math operators and modulo
                {"Integer Division vs Casting", "When you divide two integers in C, the decimal part is chopped off (truncated):\nint r = 7 / 2; // Result is 3, NOT 3.5!\n\nTo keep the decimal, convert (cast) one number to float first:\nfloat exact = (float)7 / 2; // Result is 3.50\nCasting float to int also chops off decimals: (int)3.9 becomes 3."} // Flashcard explaining truncation and explicit casting
            }},
            {"3. Decisions & Loops", { // Topic 3 covering conditionals and iteration
                {"Making Decisions (if / else)", "An 'if' statement lets your program choose what to do based on a true/false question:\n\nif (score >= 90) {\n    printf(\"Grade: A\\n\");\n} else if (score >= 80) {\n    printf(\"Grade: B\\n\");\n} else {\n    printf(\"Keep practicing!\\n\");\n}"}, // Flashcard explaining if, else if, and else branches
                {"Logical Operators (&&, ||)", "Combine multiple conditions inside an if statement:\n- && (AND): BOTH sides must be true.\n  if (age >= 18 && age <= 65)\n- || (OR): AT LEAST ONE side must be true.\n  if (age < 18 || age > 65)\n- ! (NOT): flips true to false and false to true."}, // Flashcard explaining boolean logical operators
                {"The switch Statement", "When checking one variable against many exact values, 'switch' is cleaner than many else-ifs:\n\nswitch(grade) {\n    case 'A': case 'a': printf(\"Excellent!\"); break;\n    default: printf(\"Invalid grade\");\n}\nAlways put 'break;' at the end of a case so it doesn't fall into the next one!"}, // Flashcard explaining switch-case branching
                {"while vs do-while Loops", "Loops repeat code automatically.\n- while: checks the condition FIRST. If false, it runs 0 times.\n- do-while: runs the code FIRST, then checks the condition—so it ALWAYS runs at least once!\n\ndo { number--; } while (number != 0);"}, // Flashcard contrasting pre-test and post-test loops
                {"for Loops, break & continue", "A 'for' loop puts start, condition, and step on one line:\nfor (int i = 1; i <= 10; i++) { ... }\n\n- break: immediately stops and exits the loop.\n- continue: skips the rest of THIS turn and jumps to the next turn.\nYou can also put one loop inside another (nested loops) to print grids or triangles!"} // Flashcard explaining for loops, break, continue, and nesting
            }},
            {"4. Arrays & 2D Grids", { // Topic 4 covering one-dimensional and two-dimensional arrays
                {"What is an Array?", "Instead of creating 50 separate variables, an array stores a row of same-type boxes side-by-side in memory:\n\nint numbers[5] = {10, 20, 30, 40, 50};\n\nIMPORTANT: Computers start counting at 0!\n- numbers[0] is 10 (the 1st item)\n- numbers[4] is 50 (the 5th and last item)"}, // Flashcard introducing zero-indexed arrays
                {"Finding Array Length", "If you omit the size, C counts the items for you:\nint values[] = {45, 12, 89, 34};\n\nTo calculate how many items are in an array:\nint size = sizeof(values) / sizeof(values[0]);\n(Total bytes of the whole array divided by bytes of 1 item.)"}, // Flashcard explaining sizeof array length calculation
                {"Shifting, Inserting & Rotating", "Arrays have fixed slots, so inserting or deleting requires shifting neighbors:\n- Insert at index: shift elements from the end RIGHT by 1 to open a gap.\n- Delete at index: shift elements after the index LEFT by 1 to close the gap.\n- Rotate Left: save arr[0], shift all left, put saved item at the end."}, // Flashcard explaining array shifting algorithms
                {"2D Arrays (Grids & Matrices)", "A 2D array is like a spreadsheet table with rows and columns:\n\nint matrix[3][4] = {\n    {1, 2, 3, 4},\n    {5, 6, 7, 8},\n    {9, 10, 11, 12}\n};\nAccess with matrix[row][col]. Use a nested for-loop (outer = row, inner = col) to visit every cell."} // Flashcard introducing two-dimensional arrays
            }},
            {"5. C-Strings & Null Terminator", { // Topic 5 covering null-terminated character arrays
                {"What a String Really Is", "C has NO built-in 'string' type! A string in C is simply an array of characters (char) that ends with a special invisible marker called the null terminator: '\\0'.\n\nWhen you write: char name[20] = \"Alice\";\nMemory actually holds: 'A', 'l', 'i', 'c', 'e', '\\0'."}, // Flashcard explaining how strings are stored as char arrays
                {"Why '\\0' Is Critical", "C does NOT store the length of a string anywhere. Functions like printf(\"%s\", name) start at index 0 and print letter by letter until they hit '\\0'.\n\nIf you build a char array manually and forget '\\0', printf keeps reading into random garbage memory!"}, // Flashcard highlighting the importance of the null terminator
                {"Manual String Operations", "Because '\\0' marks the end, you can find length or count words using a simple loop:\n\nint len = 0;\nwhile (name[len] != '\\0') {\n    len++;\n}\nWhen the loop stops, len equals the exact number of characters."} // Flashcard showing manual string length calculation
            }},
            {"6. Functions & Parameters", { // Topic 6 covering functions and argument passing
                {"Why Use Functions?", "A function is a mini-program with a name that performs one specific job—like a recipe you can reuse anytime:\n\nint add(int a, int b) {\n    return a + b;\n}\nUse 'void' as the return type if the function only does an action (like printing) and returns nothing."}, // Flashcard introducing function definitions and void
                {"Pass by Value (Photocopy)", "When you pass a normal variable (int, float, char) to a function, C sends a PHOTOCOPY of its value.\n\nvoid tryChange(int x) { x = 999; }\nEven though x becomes 999 inside the function, the original variable in main() is completely untouched!"}, // Flashcard explaining pass-by-value semantics
                {"Passing Arrays to Functions", "Arrays are an exception! C does NOT copy arrays—the function works directly on the caller's ORIGINAL array.\n\nvoid doubleAll(int arr[], int size)\nAny change made to arr[i] inside the function permanently changes the original array in main()."}, // Flashcard explaining how arrays decay when passed to functions
                {"Pass by Pointer (Changing Originals)", "How can a C function modify a caller's normal int variable? Pass its memory ADDRESS (&x) to a pointer parameter (int *ptr):\n\nvoid setFive(int *ptr) { *ptr = 5; }\n// Call it from main():\nsetFive(&myNum); // myNum is now 5!"} // Flashcard explaining pass-by-pointer to mutate caller variables
            }},
            {"7. Pointers & Pointer Math", { // Topic 7 covering pointers and pointer arithmetic
                {"Memory Like Numbered Lockers", "Imagine computer RAM as a hallway of numbered lockers. Every variable lives in a locker with a unique address (like 1000, 1004, 1008).\n\nA POINTER is simply a variable that stores a locker NUMBER (memory address) instead of the actual data."}, // Flashcard introducing memory addresses and pointers
                {"The Twin Operators: & and *", "Two symbols control pointers:\n1. & (Address-of): asks 'what locker number is this variable in?'\n   int *ptr = &i;\n2. * (Dereference): 'open the locker at this address and look inside!'\n   *ptr = 10; // Changes i to 10!"}, // Flashcard explaining address-of and dereference operators
                {"Double Pointers (int**)", "Since a pointer is also a variable in a locker, another pointer can store ITS address!\n\nint *ptr = &i;\nint **pptr = &ptr;\n- *pptr gives you ptr (address of i).\n- **pptr opens two doors and gives you the value of i!"}, // Flashcard explaining multi-level pointer indirection
                {"Pointer Arithmetic (No arr[i]!)", "Pointers can walk through arrays without using brackets arr[i]:\n- ptr + 1 jumps to the NEXT element (auto-adjusting by sizeof(type)!).\n- ptr - 1 jumps to the PREVIOUS element.\n- *(arr + i) is identical to arr[i].\nInvalid: You CANNOT multiply (ptr * 2) or divide (ptr / 2) pointers!"}, // Flashcard explaining valid and invalid pointer arithmetic
                {"Null Pointers (Safety)", "An uninitialized pointer points to a random dangerous address! Always set empty pointers to NULL (or nullptr in C++):\n\nint *ptr = NULL;\nif (ptr == NULL) { /* Safe: don't dereference! */ }"} // Flashcard explaining NULL pointer initialization and checks
            }},
            {"8. Heap Memory & Func Pointers", { // Topic 8 covering dynamic allocation and function pointers
                {"Stack vs Heap Memory", "Normal variables live on the STACK and are automatically erased when their function ends.\n\nThe HEAP is a big shared memory pool where YOU decide when memory is created and when it is freed—perfect for data whose size is only known at runtime."}, // Flashcard contrasting stack and heap memory lifetimes
                {"malloc() and free() in C", "Include <stdlib.h> to request heap memory:\n\nint *ptr = (int*)malloc(sizeof(int));\nif (ptr == NULL) return 1; // Always check!\n*ptr = 42;\nfree(ptr); // Return memory when done!\nptr = NULL; // Good safety habit"}, // Flashcard explaining malloc, NULL checks, and free
                {"Dynamic Arrays & Leaks", "To allocate an array of 'n' ints on the heap:\nint *arr = (int*)malloc(n * sizeof(int));\n\nRules:\n- Always free() every malloc() or you cause a MEMORY LEAK.\n- Never use memory after calling free()!"}, // Flashcard explaining dynamic arrays and memory leak prevention
                {"Global Shadowing & Function Ptrs", "1. Global variables are visible everywhere, but a local variable with the SAME name hides (shadows) the global one inside that function.\n2. Function Pointers store a function's address so you can call it indirectly:\nint (*fn)(int, int) = Add;\nint ans = fn(20, 10); // No arithmetic allowed on fn!"} // Flashcard explaining variable shadowing and function pointers
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Topic 9 covering POSIX processes, pipes, and bitwise operations
                {"Linux fork(): Cloning a Process", "fork() (<unistd.h>) clones your running program into two separate processes: Parent and Child.\nCheck its return value to know who you are:\n- -1: Error (failed)\n-  0: You are the CHILD\n- >0: You are the PARENT (value is Child's PID)\nCalling fork() 3 times creates 2^3 = 8 processes!"}, // Flashcard explaining process creation with fork
                {"Linux pipe(): Process Tunnel", "A pipe is a one-way water pipe between processes:\nint fd[2]; pipe(fd);\n- fd[0] = READ end, fd[1] = WRITE end.\nCall pipe() BEFORE fork(). Always close() unused ends—if the write end stays open, the reader waits forever! For two-way chat, use TWO pipes."}, // Flashcard explaining inter-process pipes
                {"Bitwise Operators (&, |, ^, ~)", "Bit manipulation edits individual 1s and 0s directly!\nFor A = 60 (0011 1100) and B = 13 (0000 1101):\n- A & B (AND): 1 only if BOTH are 1 -> 0000 1100 (12)\n- A | B (OR):  1 if EITHER is 1 -> 0011 1101 (61)\n- A ^ B (XOR): 1 if ONLY ONE is 1 -> 0011 0001 (49)\n- ~A (NOT):    flips every bit -> 1100 0011"}, // Flashcard explaining bitwise logical operators
                {"Bit Shifts & Struct Bit Fields", "Shifts move bits left/right (filling gaps with 0):\n- x << 1 is a super-fast multiply by 2.\n- x >> 1 is a super-fast divide by 2.\nBit Fields pack tiny flags tightly into structs using ': bits':\nstruct packed { unsigned int flag : 1; unsigned int type : 4; };"} // Flashcard explaining bit shifting and struct bit fields
            }}
        };

        db.quizTopics = { // Populate all C quiz topics and questions
            {"1. What is C? Basics & Keywords", { // Quiz questions for Topic 1
                {"Where does every C program start executing?", {"start()", "main()", "begin()", "printf()"}, 1}, // Question on entry point function
                {"What does 'return 0;' at the end of main() tell the computer?", {"There was an error", "The program ran successfully", "Print the number 0", "Restart the program"}, 1}, // Question on exit status code
                {"How many reserved keywords does standard C have (excluding sizeof)?", {"16", "24", "32", "50"}, 2}, // Question on C keyword count
                {"What happens if you declare 'int x;' without giving it a starting value?", {"It is always 0", "It holds random leftover memory (garbage)", "It causes a compile error", "It becomes -1"}, 1}, // Question on uninitialized variables
                {"Which keyword locks a variable so its value can never be changed?", {"static", "lock", "const", "fixed"}, 2} // Question on const qualifier
            }},
            {"2. Input, Output & Math", { // Quiz questions for Topic 2
                {"Which printf() format specifier prints a whole number (int)?", {"%f", "%c", "%d", "%s"}, 2}, // Question on integer format specifier
                {"Why do we write '&age' in scanf(\"%d\", &age)?", {"To multiply age", "To give scanf the memory address of age", "To check if age is positive", "To print age"}, 1}, // Question on address-of operator in scanf
                {"What is the result of integer division (7 / 2) in C?", {"3.5", "4", "3", "0"}, 2}, // Question on integer division truncation
                {"What does the modulo operator (10 % 3) return?", {"3", "1", "0.33", "30"}, 1}, // Question on remainder operator
                {"How do you force 7 / 2 to give 3.5 instead of 3?", {"7 // 2", "(float)7 / 2", "int(7 / 2)", "7 % 2"}, 1} // Question on float type casting
            }},
            {"3. Decisions & Loops", { // Quiz questions for Topic 3
                {"Which logical operator means BOTH conditions must be true?", {"||", "&&", "==", "!="}, 1}, // Question on logical AND operator
                {"Why do we put 'break;' at the end of each case in a switch statement?", {"To exit the whole program", "To stop execution from falling into the next case", "To restart the switch", "It is optional decoration"}, 1}, // Question on switch fallthrough prevention
                {"What is the key difference between a 'while' loop and a 'do-while' loop?", {"do-while is faster", "do-while always executes its body at least once", "while cannot use integers", "do-while never stops"}, 1}, // Question on do-while execution guarantee
                {"What does the 'continue;' statement do inside a loop?", {"Exits the loop completely", "Skips the rest of the current turn and starts the next turn", "Pauses for 1 second", "Ends main()"}, 1} // Question on continue statement behavior
            }},
            {"4. Arrays & 2D Grids", { // Quiz questions for Topic 4
                {"In 'int arr[5] = {10, 20, 30, 40, 50};', what is the index of 10?", {"1", "0", "5", "-1"}, 1}, // Question on zero-based array indexing
                {"How do you calculate the number of elements in a C array 'values'?", {"values.length()", "sizeof(values)", "sizeof(values) / sizeof(values[0])", "count(values)"}, 2}, // Question on sizeof array element count
                {"To delete an element from the middle of an array, which way do you shift the remaining items?", {"Shift them LEFT to close the gap", "Shift them RIGHT", "Reverse the array", "Multiply by 2"}, 0}, // Question on left shifting during deletion
                {"In a 2D array 'matrix[3][4]', what do 3 and 4 represent?", {"3 columns and 4 rows", "3 rows and 4 columns", "Values 3 and 4", "3 arrays of 4 bytes"}, 1} // Question on row-major 2D array dimensions
            }},
            {"5. C-Strings & Null Terminator", { // Quiz questions for Topic 5
                {"How are strings represented in the C language?", {"A built-in String class", "A char array ending with '\\0'", "An int array", "A float pointer"}, 1}, // Question on C-string representation
                {"What happens if you forget '\\0' at the end of a manual char array and call printf(\"%s\")?", {"C adds it at runtime", "printf keeps reading into random memory", "Nothing prints", "The compiler fixes it"}, 1}, // Question on missing null terminator bug
                {"How does a manual loop find the length of a C-string?", {"Counts until it hits a space", "Counts while str[len] != '\\0'", "Reads str[-1]", "Checks str.size()"}, 1}, // Question on manual string length loop
                {"How many array slots does \"Alice\" actually occupy in memory (including the null terminator)?", {"5", "6", "4", "10"}, 1} // Question on string byte size including null byte
            }},
            {"6. Functions & Parameters", { // Quiz questions for Topic 6
                {"What return type do you use for a function that does not return any value?", {"null", "empty", "void", "int"}, 2}, // Question on void return type
                {"In C, when you pass an ordinary int to a function (pass by value), what does the function get?", {"The original variable", "A photocopy (copy) of the value", "A memory address", "Nothing"}, 1}, // Question on pass by value
                {"When you pass an array to a C function and modify arr[0], what happens in main()?", {"The original array in main() is modified too", "Nothing changes in main()", "The program crashes", "It creates a second array"}, 0}, // Question on array reference semantics
                {"How can a C function modify a caller's ordinary int variable?", {"Return void", "Pass its address (&x) to a pointer parameter (int *p)", "Use const int", "Use printf"}, 1} // Question on modifying caller variables via pointers
            }},
            {"7. Pointers & Pointer Math", { // Quiz questions for Topic 7
                {"What does a pointer variable actually store?", {"A copy of the value", "The memory address of another variable", "The name of the variable", "Always 0"}, 1}, // Question on what a pointer stores
                {"Which operator dereferences a pointer to access or change the value it points to?", {"&", "*", "%", "->"}, 1}, // Question on dereference operator
                {"If 'int *p = &i;' and 'int **pp = &p;', what does '**pp = 7;' do?", {"Changes p to 7", "Changes i to 7", "Changes the address of pp", "Causes a syntax error"}, 1}, // Question on double pointer dereferencing
                {"Which pointer arithmetic expression is identical to arr[2]?", {"*(arr + 2)", "*arr + 2", "&arr + 2", "arr * 2"}, 0}, // Question on pointer-offset equivalence
                {"Which operation is INVALID on pointers in C?", {"ptr++", "ptr + 3", "ptr * 2", "ptr - 1"}, 2} // Question on illegal pointer multiplication
            }},
            {"8. Heap Memory & Func Pointers", { // Quiz questions for Topic 8
                {"Which function allocates memory on the heap in C?", {"new", "malloc()", "alloc_heap()", "create()"}, 1}, // Question on heap allocation function
                {"What should you ALWAYS check immediately after calling malloc()?", {"If ptr == NULL", "If ptr == 0xFF", "If free() ran", "If sizeof > 0"}, 0}, // Question on checking malloc return value
                {"What happens if you allocate memory with malloc() and never call free()?", {"It frees automatically in 1 second", "A memory leak occurs", "The CPU runs faster", "Compile error"}, 1}, // Question on memory leaks
                {"If a function declares a local variable with the same name as a global variable, what happens?", {"Compile error", "The local variable shadows the global one inside that function", "The global variable is deleted", "Both change together"}, 1}, // Question on variable shadowing
                {"Which operation is NOT allowed on a function pointer?", {"Assigning a function address", "Calling the function through it", "Pointer arithmetic (fn + 1)", "Passing it to another function"}, 2} // Question on function pointer restrictions
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Quiz questions for Topic 9
                {"What value does fork() return inside the CHILD process?", {"-1", "0", "Child's PID (>0)", "Parent's PID"}, 1}, // Question on fork return value in child
                {"If a program calls fork() 3 times in a row, how many total processes exist?", {"3", "6", "8", "9"}, 2}, // Question on exponential process count from fork
                {"Why must processes close unused pipe ends after fork()?", {"To save disk space", "So the reader doesn't wait forever for more data", "Because pipes only work in C++", "To double the speed"}, 1}, // Question on closing unused pipe file descriptors
                {"Given A = 0011 1100 (60) and B = 0000 1101 (13), what is A & B (AND)?", {"0000 1100 (12)", "0011 1101 (61)", "0011 0001 (49)", "1100 0011"}, 0}, // Question on bitwise AND calculation
                {"Shifting an unsigned integer left by 1 (x << 1) is a fast equivalent to:", {"Adding 1", "Multiplying by 2", "Dividing by 2", "Flipping all bits"}, 1} // Question on left bit shift multiplication
            }}
        };

        db.constructorTopics = { // Populate all C code constructor topics and levels
            {"1. What is C? Basics & Keywords", { // Constructor levels for Topic 1
                { // Begin first level definition
                    "Lab: Your First C Program", // Level title
                    { // Ordered correct lines of code
                        "#include <stdio.h>", // Line 1: header include
                        "int main() {", // Line 2: main entry point
                        "    printf(\"Hello, World!\\n\");", // Line 3: print greeting
                        "    return 0;", // Line 4: return exit status
                        "}" // Line 5: closing brace
                    },
                    { // Explanations for each slot
                        "Include <stdio.h> so printf() is available.", // Explanation for line 1
                        "Every C program starts execution inside main().", // Explanation for line 2
                        "Print Hello, World! followed by a newline.", // Explanation for line 3
                        "Return 0 to tell the OS everything succeeded.", // Explanation for line 4
                        "Close the main() function block." // Explanation for line 5
                    },
                    {"cout << \"Hello\";"} // Distractor C++ block
                },
                { // Begin second level definition
                    "Lab: Variables & const Lock", // Level title
                    { // Ordered correct lines of code
                        "const int MAX_SCORE = 100;", // Line 1: constant declaration
                        "int age = 25;", // Line 2: integer variable
                        "float price = 19.99f;", // Line 3: float variable
                        "printf(\"Age: %d\\n\", age);", // Line 4: formatted print
                        "return 0;" // Line 5: return exit status
                    },
                    { // Explanations for each slot
                        "Lock MAX_SCORE as a constant so it cannot change.", // Explanation for line 1
                        "Declare an integer variable age set to 25.", // Explanation for line 2
                        "Declare a float variable price with 'f' suffix.", // Explanation for line 3
                        "Print age using the %d integer format specifier.", // Explanation for line 4
                        "Exit main() with status 0." // Explanation for line 5
                    },
                    {"MAX_SCORE = 200;"} // Distractor invalid mutation block
                }
            }},
            {"2. Input, Output & Math", { // Constructor levels for Topic 2
                { // Begin first level definition
                    "Lab: Safe Casting & Division", // Level title
                    { // Ordered correct lines of code
                        "int a = 7, b = 2;", // Line 1: declare integers
                        "float exact = (float)a / b;", // Line 2: cast and divide
                        "printf(\"Result: %.2f\\n\", exact);", // Line 3: print float
                        "return 0;" // Line 4: return zero
                    },
                    { // Explanations for each slot
                        "Declare two integer variables a and b.", // Explanation for line 1
                        "Cast 'a' to float so 7/2 gives 3.5 instead of 3.", // Explanation for line 2
                        "Print the float formatted to 2 decimal places.", // Explanation for line 3
                        "Return 0 at the end of main." // Explanation for line 4
                    },
                    {"float exact = a / b;"} // Distractor truncated integer division block
                },
                { // Begin second level definition
                    "Lab 1 Q1: Leap Year Check", // Level title
                    { // Ordered correct lines of code
                        "bool isLeapYear(int year) {", // Line 1: function header
                        "    if (year % 400 == 0) return true;", // Line 2: divisible by 400 check
                        "    if (year % 100 == 0) return false;", // Line 3: century non-leap check
                        "    if (year % 4 == 0) return true;", // Line 4: divisible by 4 check
                        "    return false;", // Line 5: default non-leap return
                        "}" // Line 6: closing brace
                    },
                    { // Explanations for each slot
                        "Define isLeapYear taking an integer year.", // Explanation for line 1
                        "Years divisible by 400 (e.g., 1600) are leap years.", // Explanation for line 2
                        "Other centuries divisible by 100 (e.g., 1700) are NOT.", // Explanation for line 3
                        "Remaining years divisible by 4 (e.g., 2016) are leap years.", // Explanation for line 4
                        "All other years return false.", // Explanation for line 5
                        "Close the function." // Explanation for line 6
                    },
                    {"if (year % 2 == 0) return true;"} // Distractor incorrect modulo check
                }
            }},
            {"3. Decisions & Loops", { // Constructor levels for Topic 3
                { // Begin first level definition
                    "Lab 1 Q2: Reverse Number Digits", // Level title
                    { // Ordered correct lines of code
                        "int rev = 0;", // Line 1: initialize accumulator
                        "while (n > 0) {", // Line 2: loop while digits remain
                        "    int digit = n % 10;", // Line 3: extract last digit
                        "    rev = rev * 10 + digit;", // Line 4: append digit to reversed number
                        "    n = n / 10;", // Line 5: drop last digit
                        "}" // Line 6: closing brace
                    },
                    { // Explanations for each slot
                        "Initialize reversed accumulator rev to 0.", // Explanation for line 1
                        "Loop while digits remain in n.", // Explanation for line 2
                        "Extract the last digit using modulo 10.", // Explanation for line 3
                        "Shift rev left by 10 and add the extracted digit.", // Explanation for line 4
                        "Remove the last digit via integer division by 10.", // Explanation for line 5
                        "Close the while loop." // Explanation for line 6
                    },
                    {"n = n % 10;"} // Distractor overwriting n with remainder
                },
                { // Begin second level definition
                    "Lab 1 Q3: Prime Number Check", // Level title
                    { // Ordered correct lines of code
                        "if (n <= 1) return false;", // Line 1: exclude numbers <= 1
                        "for (int i = 2; i * i <= n; i++) {", // Line 2: loop divisors up to sqrt(n)
                        "    if (n % i == 0) {", // Line 3: test divisibility
                        "        return false;", // Line 4: factor found so not prime
                        "    }", // Line 5: close if block
                        "} return true;" // Line 6: close loop and return true
                    },
                    { // Explanations for each slot
                        "Numbers <= 1 are never prime.", // Explanation for line 1
                        "Check candidate divisors up to sqrt(n).", // Explanation for line 2
                        "Test if i divides n with zero remainder.", // Explanation for line 3
                        "Found a factor, so n is NOT prime.", // Explanation for line 4
                        "Close the if block.", // Explanation for line 5
                        "No factors found: close loop and return true." // Explanation for line 6
                    },
                    {"if (n % 2 == 0) return true;"} // Distractor faulty even check
                },
                { // Begin third level definition
                    "Lab 1 Q6: Right-Angled Triangle", // Level title
                    { // Ordered correct lines of code
                        "for (int row = 1; row <= 4; row++) {", // Line 1: outer row loop
                        "    for (int col = 1; col <= row; col++) {", // Line 2: inner column loop
                        "        printf(\"A\");", // Line 3: print character A
                        "    }", // Line 4: close inner loop
                        "    printf(\"\\n\");", // Line 5: print newline
                        "}" // Line 6: close outer loop
                    },
                    { // Explanations for each slot
                        "Outer loop iterates over rows 1 through 4.", // Explanation for line 1
                        "Inner loop prints 'row' A's on the current row.", // Explanation for line 2
                        "Print one 'A' character.", // Explanation for line 3
                        "Close inner column loop.", // Explanation for line 4
                        "Print newline after finishing the row.", // Explanation for line 5
                        "Close outer row loop." // Explanation for line 6
                    },
                    {"for (int col = 1; col <= 4; col++) {"} // Distractor fixed-width square loop
                }
            }},
            {"4. Arrays & 2D Grids", { // Constructor levels for Topic 4
                { // Begin first level definition
                    "Lab 1 Q8: Find Element in Array", // Level title
                    { // Ordered correct lines of code
                        "int find(int size, int arr[], int target) {", // Line 1: function signature
                        "    for (int i = 0; i < size; i++) {", // Line 2: iterate over elements
                        "        if (arr[i] == target) return i;", // Line 3: return index if matched
                        "    }", // Line 4: close loop
                        "    return -1;", // Line 5: return -1 if not found
                        "}" // Line 6: close function
                    },
                    { // Explanations for each slot
                        "Define find() with size, array, and target value.", // Explanation for line 1
                        "Scan indices 0 through size - 1.", // Explanation for line 2
                        "If element matches target, return its index i.", // Explanation for line 3
                        "Close the for loop.", // Explanation for line 4
                        "Return -1 if target was not found anywhere.", // Explanation for line 5
                        "Close the function." // Explanation for line 6
                    },
                    {"return 0; // not found"} // Distractor returning valid index 0 on failure
                },
                { // Begin second level definition
                    "Lab 1 Q15: Reverse Array In-Place", // Level title
                    { // Ordered correct lines of code
                        "for (int i = 0; i < size / 2; i++) {", // Line 1: loop halfway through array
                        "    int temp = arr[i];", // Line 2: store left value
                        "    arr[i] = arr[size - 1 - i];", // Line 3: copy right value to left
                        "    arr[size - 1 - i] = temp;", // Line 4: copy temp to right
                        "}" // Line 5: close loop
                    },
                    { // Explanations for each slot
                        "Stop at size / 2 so you don't swap elements back!", // Explanation for line 1
                        "Save the left element in temp.", // Explanation for line 2
                        "Move the mirror right element to the left slot.", // Explanation for line 3
                        "Place saved temp into the right slot.", // Explanation for line 4
                        "Close the loop." // Explanation for line 5
                    },
                    {"for (int i = 0; i < size; i++) {"} // Distractor full-length double-swap loop
                },
                { // Begin third level definition
                    "Lab 1 Q16: Rotate Array Left", // Level title
                    { // Ordered correct lines of code
                        "int first = arr[0];", // Line 1: save first element
                        "for (int i = 0; i < size - 1; i++) {", // Line 2: loop up to second-to-last
                        "    arr[i] = arr[i + 1];", // Line 3: shift elements left
                        "}", // Line 4: close loop
                        "arr[size - 1] = first;" // Line 5: place first element at end
                    },
                    { // Explanations for each slot
                        "Save arr[0] before shifting overwrites it.", // Explanation for line 1
                        "Loop from 0 up to the second-to-last index.", // Explanation for line 2
                        "Shift each neighbor one slot to the left.", // Explanation for line 3
                        "Close the for loop.", // Explanation for line 4
                        "Place the saved first element at the back." // Explanation for line 5
                    },
                    {"arr[0] = arr[size - 1];"} // Distractor overwriting arr[0] without shifting
                }
            }},
            {"5. C-Strings & Null Terminator", { // Constructor levels for Topic 5
                { // Begin first level definition
                    "Lab: Manual String Length ('\\0')", // Level title
                    { // Ordered correct lines of code
                        "char name[] = \"Alice\";", // Line 1: declare C-string
                        "int len = 0;", // Line 2: initialize length counter
                        "while (name[len] != '\\0') {", // Line 3: loop until null terminator
                        "    len++;", // Line 4: increment counter
                        "}", // Line 5: close loop
                        "printf(\"Length: %d\\n\", len);" // Line 6: print length
                    },
                    { // Explanations for each slot
                        "Declare null-terminated C-string \"Alice\".", // Explanation for line 1
                        "Start character counter len at 0.", // Explanation for line 2
                        "Loop until the null terminator '\\0' is reached.", // Explanation for line 3
                        "Increment len for each character.", // Explanation for line 4
                        "Close the while loop.", // Explanation for line 5
                        "Print the measured length." // Explanation for line 6
                    },
                    {"while (name[len] != '0') {"} // Distractor checking digit '0' instead of '\0'
                },
                { // Begin second level definition
                    "Lab 2 Q5: strlen_ptr (Pointers Only)", // Level title
                    { // Ordered correct lines of code
                        "int strlen_ptr(char *str) {", // Line 1: pointer parameter signature
                        "    int len = 0;", // Line 2: initialize length counter
                        "    while (*str != '\\0') {", // Line 3: check dereferenced char
                        "        len++;", // Line 4: increment length
                        "        str++;", // Line 5: advance pointer
                        "    } return len; }" // Line 6: return result and close function
                    },
                    { // Explanations for each slot
                        "Take a pointer to the first char of the string.", // Explanation for line 1
                        "Start length counter at 0.", // Explanation for line 2
                        "Check dereferenced char against '\\0'.", // Explanation for line 3
                        "Increment length counter.", // Explanation for line 4
                        "Move pointer forward to the next char.", // Explanation for line 5
                        "Close loop, return len, and close function." // Explanation for line 6
                    },
                    {"while (str[len] != '\\0') {"} // Distractor array indexing block
                }
            }},
            {"6. Functions & Parameters", { // Constructor levels for Topic 6
                { // Begin first level definition
                    "Lab 2 Q1: Swap Ints via Pointers", // Level title
                    { // Ordered correct lines of code
                        "void swap_ints(int *a, int *b) {", // Line 1: pointer parameters
                        "    int temp = *a;", // Line 2: dereference a into temp
                        "    *a = *b;", // Line 3: assign *b to *a
                        "    *b = temp;", // Line 4: assign temp to *b
                        "}" // Line 5: close function
                    },
                    { // Explanations for each slot
                        "Accept two int pointers holding callers' addresses.", // Explanation for line 1
                        "Dereference 'a' to save first value in temp.", // Explanation for line 2
                        "Write value at 'b' into memory pointed to by 'a'.", // Explanation for line 3
                        "Write temp into memory pointed to by 'b'.", // Explanation for line 4
                        "Close swap_ints." // Explanation for line 5
                    },
                    {"int *temp = a;"} // Distractor copying pointer instead of value
                },
                { // Begin second level definition
                    "Lab: Modify Array in doubleAll()", // Level title
                    { // Ordered correct lines of code
                        "void doubleAll(int arr[], int size) {", // Line 1: function header
                        "    for (int i = 0; i < size; i++) {", // Line 2: loop through elements
                        "        arr[i] = arr[i] * 2;", // Line 3: double element in place
                        "    }", // Line 4: close loop
                        "}" // Line 5: close function
                    },
                    { // Explanations for each slot
                        "Arrays pass directly to functions without copying.", // Explanation for line 1
                        "Loop across every index of the array.", // Explanation for line 2
                        "Double each element directly in caller's memory.", // Explanation for line 3
                        "Close the for loop.", // Explanation for line 4
                        "Close the void function." // Explanation for line 5
                    },
                    {"return arr;"} // Distractor returning array from void function
                }
            }},
            {"7. Pointers & Pointer Math", { // Constructor levels for Topic 7
                { // Begin first level definition
                    "Lab: Double Pointer Indirection", // Level title
                    { // Ordered correct lines of code
                        "int i = 5;", // Line 1: declare integer i
                        "int *ptr = &i;", // Line 2: store address of i
                        "int **pptr = &ptr;", // Line 3: store address of ptr
                        "**pptr = 12;", // Line 4: modify i through double pointer
                        "printf(\"%d\\n\", i);" // Line 5: print updated value
                    },
                    { // Explanations for each slot
                        "Declare integer i initialized to 5.", // Explanation for line 1
                        "Store address of i in pointer ptr.", // Explanation for line 2
                        "Store address of ptr in double pointer pptr.", // Explanation for line 3
                        "Dereference twice to change i to 12.", // Explanation for line 4
                        "Print updated i (12)." // Explanation for line 5
                    },
                    {"int **pptr = &i;"} // Distractor assigning int address to double pointer
                },
                { // Begin second level definition
                    "Lab 2 Q3: find_max (Pointers Only)", // Level title
                    { // Ordered correct lines of code
                        "int maxVal = *arr;", // Line 1: initialize max to first element
                        "for (int i = 1; i < size; i++) {", // Line 2: loop remaining elements
                        "    if (*(arr + i) > maxVal) {", // Line 3: compare via pointer arithmetic
                        "        maxVal = *(arr + i);", // Line 4: update maxVal
                        "    }", // Line 5: close if
                        "} return maxVal;" // Line 6: close loop and return maxVal
                    },
                    { // Explanations for each slot
                        "Read the 1st element via *arr as initial max.", // Explanation for line 1
                        "Check elements from offset 1 to size - 1.", // Explanation for line 2
                        "Use pointer math *(arr + i) instead of arr[i].", // Explanation for line 3
                        "Update maxVal when a bigger number is found.", // Explanation for line 4
                        "Close if block.", // Explanation for line 5
                        "Close loop and return maxVal." // Explanation for line 6
                    },
                    {"if (*arr + i > maxVal) {"} // Distractor missing parentheses in pointer math
                }
            }},
            {"8. Heap Memory & Func Pointers", { // Constructor levels for Topic 8
                { // Begin first level definition
                    "Lab: Dynamic Array with malloc()", // Level title
                    { // Ordered correct lines of code
                        "int *arr = (int*)malloc(5 * sizeof(int));", // Line 1: allocate heap array
                        "if (arr == NULL) { return 1; }", // Line 2: check allocation failure
                        "arr[0] = 42;", // Line 3: write value to heap memory
                        "free(arr);", // Line 4: release heap memory
                        "arr = NULL;" // Line 5: nullify dangling pointer
                    },
                    { // Explanations for each slot
                        "Allocate heap memory for 5 ints using malloc().", // Explanation for line 1
                        "Always check if malloc() returned NULL.", // Explanation for line 2
                        "Store a value inside the heap array.", // Explanation for line 3
                        "Free the heap memory to prevent leaks.", // Explanation for line 4
                        "Set pointer to NULL after freeing." // Explanation for line 5
                    },
                    {"delete[] arr;"} // Distractor C++ delete operator
                },
                { // Begin second level definition
                    "Lab: Calling a Function Pointer", // Level title
                    { // Ordered correct lines of code
                        "int (*funcPtr)(int, int);", // Line 1: declare function pointer
                        "funcPtr = Add;", // Line 2: assign function address
                        "int result = funcPtr(20, 10);", // Line 3: invoke function via pointer
                        "printf(\"Sum: %d\\n\", result);" // Line 4: print result
                    },
                    { // Explanations for each slot
                        "Declare a function pointer matching int(int, int).", // Explanation for line 1
                        "Assign the address of function Add to funcPtr.", // Explanation for line 2
                        "Call Add indirectly through funcPtr.", // Explanation for line 3
                        "Print the returned sum (30)." // Explanation for line 4
                    },
                    {"funcPtr = Add(20, 10);"} // Distractor assigning return value instead of address
                }
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Constructor levels for Topic 9
                { // Begin first level definition
                    "Linux Lab: fork() & Pipe Read", // Level title
                    { // Ordered correct lines of code
                        "int fd[2];", // Line 1: pipe descriptor array
                        "pipe(fd);", // Line 2: create pipe before fork
                        "if (fork() == 0) {", // Line 3: fork and check child process
                        "    close(fd[1]);", // Line 4: close unused write end
                        "    read(fd[0], buf, sizeof(buf));", // Line 5: read data from pipe
                        "    close(fd[0]); }" // Line 6: close read end and child block
                    },
                    { // Explanations for each slot
                        "Declare 2-int array for read (0) & write (1) ends.", // Explanation for line 1
                        "Create the pipe BEFORE calling fork().", // Explanation for line 2
                        "Fork and enter child branch (pid == 0).", // Explanation for line 3
                        "Close unused write end fd[1] so read won't hang.", // Explanation for line 4
                        "Read incoming bytes from parent into buf.", // Explanation for line 5
                        "Close read end and close child block." // Explanation for line 6
                    },
                    {"close(fd[0]); // before read"} // Distractor closing read end prematurely
                },
                { // Begin second level definition
                    "Bit Lab: Packed Struct Bit Fields", // Level title
                    { // Ordered correct lines of code
                        "struct packed_struct {", // Line 1: begin struct definition
                        "    unsigned int f1 : 1;", // Line 2: 1-bit flag field
                        "    unsigned int f2 : 1;", // Line 3: 1-bit flag field
                        "    unsigned int type : 4;", // Line 4: 4-bit type field
                        "    unsigned int my_int : 9;", // Line 5: 9-bit integer field
                        "};" // Line 6: close struct
                    },
                    { // Explanations for each slot
                        "Begin struct definition.", // Explanation for line 1
                        "Pack 1-bit flag f1.", // Explanation for line 2
                        "Pack 1-bit flag f2.", // Explanation for line 3
                        "Pack 4-bit type field.", // Explanation for line 4
                        "Pack 9-bit integer field.", // Explanation for line 5
                        "Close struct with semicolon." // Explanation for line 6
                    },
                    {"unsigned int type[4];"} // Distractor array syntax instead of bit field
                }
            }}
        };

        db.visualizerTopics = { // Populate all C visualizer topics and walkthroughs
            {"1. What is C? Basics & Keywords", { // Visualizer levels for Topic 1
                { // Begin level definition
                    "Lab: Variables & Format Specifiers", // Level title
                    "#include <stdio.h>\nint main() {\n    const double TAX = 0.08;\n    int age = 25;\n    float price = 19.99f;\n    printf(\"Age: %d, Price: %.2f\\n\", age, price);\n    return 0;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"#include <stdio.h>", "Loads Standard I/O library so we can use printf()."}, // Step 1
                        {"const double TAX = 0.08;", "Creates a locked decimal constant that cannot be modified later."}, // Step 2
                        {"int age = 25;", "Creates an integer box named 'age' holding 25."}, // Step 3
                        {"float price = 19.99f;", "Creates a single-precision decimal variable."}, // Step 4
                        {"printf(\"Age: %d, Price: %.2f\\n\", age, price);", "Replaces %d with 25 and %.2f with 19.99 (2 decimal places)."}, // Step 5
                        {"return 0;", "Signals to the operating system that main() finished with zero errors."} // Step 6
                    }
                }
            }},
            {"2. Input, Output & Math", { // Visualizer levels for Topic 2
                { // Begin first level definition
                    "Lab 1 Q2: Palindrome Number Check", // Level title
                    "bool isAPalindrome(int n) {\n    int orig = n, rev = 0;\n    while (n > 0) {\n        rev = rev * 10 + (n % 10);\n        n /= 10;\n    }\n    return (orig == rev);\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int orig = n, rev = 0;", "Saves the original number (e.g., 121) because n will shrink to 0."}, // Step 1
                        {"while (n > 0) {", "Repeats as long as digits remain in n."}, // Step 2
                        {"    rev = rev * 10 + (n % 10);", "Peels off the last digit (n % 10) and appends it to rev."}, // Step 3
                        {"    n /= 10;", "Integer division drops the last digit (121 -> 12 -> 1 -> 0)."}, // Step 4
                        {"return (orig == rev);", "Compares original (121) with reversed (121). True = Palindrome!"} // Step 5
                    }
                },
                { // Begin second level definition
                    "Lab 1 Q4: Binary to Decimal", // Level title
                    "int convertBinarytoDecimal(int bin) {\n    int dec = 0, base = 1;\n    while (bin > 0) {\n        int last = bin % 10;\n        dec += last * base;\n        base *= 2;\n        bin /= 10;\n    }\n    return dec;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int dec = 0, base = 1;", "Starts decimal total at 0 and place value (2^0) at 1."}, // Step 1
                        {"while (bin > 0) {", "Processes binary digits (e.g., 110) right-to-left."}, // Step 2
                        {"    int last = bin % 10;", "Grabs the rightmost digit (0 or 1) without bit shifts."}, // Step 3
                        {"    dec += last * base;", "Adds digit * place value (1, 2, 4, 8...) to dec."}, // Step 4
                        {"    base *= 2; bin /= 10;", "Doubles base to next power of 2 and drops rightmost digit."}, // Step 5
                        {"return dec;", "For 110, returns 0*1 + 1*2 + 1*4 = 6!"} // Step 6
                    }
                }
            }},
            {"3. Decisions & Loops", { // Visualizer levels for Topic 3
                { // Begin first level definition
                    "Lab 1 Q7: Isosceles Triangle Pattern", // Level title
                    "void drawIsocelesTriangle() {\n    for (int row = 1; row <= 7; row++) {\n        int count = (row <= 4) ? row : (8 - row);\n        for (int c = 0; c < count; c++) printf(\"A\");\n        printf(\"\\n\");\n    }\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"for (int row = 1; row <= 7; row++)", "Runs 7 rows total: growing 1..4 then shrinking 3..1."}, // Step 1
                        {"int count = (row <= 4) ? row : (8 - row);", "If row <= 4, prints 'row' A's; otherwise mirrors back down (3, 2, 1)."}, // Step 2
                        {"for (int c = 0; c < count; c++) printf(\"A\");", "Prints the exact number of A's for this row."}, // Step 3
                        {"printf(\"\\n\");", "Moves cursor to the next line."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: switch Grade Menu", // Level title
                    "switch (grade) {\n    case 'A':\n    case 'a':\n        printf(\"Excellent!\\n\");\n        break;\n    default:\n        printf(\"Invalid grade\\n\");\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"switch (grade)", "Compares grade against each constant case."}, // Step 1
                        {"case 'A': case 'a':", "Stacked cases handle both uppercase and lowercase 'A' together."}, // Step 2
                        {"printf(\"Excellent!\\n\");", "Runs when grade matches 'A' or 'a'."}, // Step 3
                        {"break;", "Exits the switch immediately so it doesn't fall into default!"}, // Step 4
                        {"default:", "Fallback branch if grade didn't match any case."} // Step 5
                    }
                }
            }},
            {"4. Arrays & 2D Grids", { // Visualizer levels for Topic 4
                { // Begin first level definition
                    "Lab 1 Q9: Find 2nd Largest in Array", // Level title
                    "int find2ndLargest(int size, int arr[]) {\n    int max1 = -1, max2 = -1;\n    for (int i = 0; i < size; i++) {\n        if (arr[i] > max1) {\n            max2 = max1; max1 = arr[i];\n        } else if (arr[i] > max2 && arr[i] != max1) {\n            max2 = arr[i];\n        }\n    }\n    return max2;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int max1 = -1, max2 = -1;", "Tracks 1st and 2nd largest values (-1 means none found yet)."}, // Step 1
                        {"if (arr[i] > max1)", "Found a new overall maximum!"}, // Step 2
                        {"max2 = max1; max1 = arr[i];", "Demotes old max1 to max2, then saves new max1."}, // Step 3
                        {"else if (arr[i] > max2 && arr[i] != max1)", "Strictly between max2 and max1 (ignores duplicates of max1)."}, // Step 4
                        {"return max2;", "Returns the 2nd largest value, or -1 if none exists."} // Step 5
                    }
                },
                { // Begin second level definition
                    "Lab 1 Q11: Insert Element & Shift", // Level title
                    "bool insertElement(int size, int *count, int arr[], int val, int idx) {\n    if (*count >= size || idx < 0 || idx > *count) return false;\n    for (int i = *count; i > idx; i--) {\n        arr[i] = arr[i - 1];\n    }\n    arr[idx] = val;\n    (*count)++;\n    return true;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"if (*count >= size || idx < 0 || idx > *count)", "Safety check: ensures array isn't full and index is valid."}, // Step 1
                        {"for (int i = *count; i > idx; i--)", "Walks backwards from the end toward idx."}, // Step 2
                        {"arr[i] = arr[i - 1];", "Shifts elements 1 slot RIGHT to open an empty space at idx."}, // Step 3
                        {"arr[idx] = val; (*count)++;", "Places val into the gap and increments count."}, // Step 4
                        {"return true;", "Confirms the insertion succeeded."} // Step 5
                    }
                },
                { // Begin third level definition
                    "Lab 1 Q17: Two Movies Flight Match", // Level title
                    "bool twoMovies(int flight, int movies[], int size) {\n    for (int i = 0; i < size; i++) {\n        for (int j = i + 1; j < size; j++) {\n            if (movies[i] + movies[j] == flight) return true;\n        }\n    }\n    return false;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"for (int i = 0; i < size; i++)", "Picks the first movie at index i."}, // Step 1
                        {"for (int j = i + 1; j < size; j++)", "Starts j at i + 1 so the same movie is NEVER watched twice!"}, // Step 2
                        {"if (movies[i] + movies[j] == flight)", "Checks if the pair's runtimes equal the exact flight length."}, // Step 3
                        {"return true; / return false;", "Returns true on first match, or false if no pair works."} // Step 4
                    }
                }
            }},
            {"5. C-Strings & Null Terminator", { // Visualizer levels for Topic 5
                { // Begin first level definition
                    "Lab 1 Q18: Count Words in Char Array", // Level title
                    "int wordCounter(int size, char chars[]) {\n    int words = 0; bool inWord = false;\n    for (int i = 0; i < size; i++) {\n        bool isLetter = (chars[i]>='A'&&chars[i]<='Z') || (chars[i]>='a'&&chars[i]<='z');\n        if (isLetter && !inWord) { words++; inWord = true; }\n        else if (!isLetter) { inWord = false; }\n    }\n    return words;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int words = 0; bool inWord = false;", "Tracks total words and whether we are currently inside a word."}, // Step 1
                        {"bool isLetter = (A..Z) || (a..z);", "Tests if the current char is an alphabetic letter."}, // Step 2
                        {"if (isLetter && !inWord) { words++; inWord = true; }", "Just stepped onto the 1st letter of a new word—count it!"}, // Step 3
                        {"else if (!isLetter) inWord = false;", "Hit a space: resets flag so the next letter starts a new word."}, // Step 4
                        {"return words;", "Returns the exact number of separate words."} // Step 5
                    }
                },
                { // Begin second level definition
                    "Lab 2 Q7: copy_string (Pointers Only)", // Level title
                    "void copy_string(char *dest, char *src) {\n    while (*src != '\\0') {\n        *dest = *src;\n        dest++; src++;\n    }\n    *dest = '\\0';\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"while (*src != '\\0')", "Loops until src reaches the null terminator."}, // Step 1
                        {"*dest = *src;", "Copies one character from src into dest."}, // Step 2
                        {"dest++; src++;", "Advances both pointers to the next character slot."}, // Step 3
                        {"*dest = '\\0';", "CRITICAL: Writes '\\0' at the end so dest is a valid C-string!"} // Step 4
                    }
                },
                { // Begin third level definition
                    "Lab 2 Q10: compare_strings (Pointers)", // Level title
                    "int compare_strings(char *s1, char *s2) {\n    while (*s1 != '\\0' && *s1 == *s2) {\n        s1++; s2++;\n    }\n    return (*s1 - *s2);\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"while (*s1 != '\\0' && *s1 == *s2)", "Walks forward while letters match and string hasn't ended."}, // Step 1
                        {"s1++; s2++;", "Advances both pointers to compare the next pair of characters."}, // Step 2
                        {"return (*s1 - *s2);", "Returns 0 if equal, <0 if s1 < s2 (\"apple\" < \"banana\"), >0 if s1 > s2."} // Step 3
                    }
                }
            }},
            {"6. Functions & Parameters", { // Visualizer levels for Topic 6
                { // Begin first level definition
                    "Lab: Pass by Value vs Pass by Pointer", // Level title
                    "void byVal(int j) { j = 5; }\nvoid byPtr(int *p) { *p = 5; }\nint main() {\n    int i = 3;\n    byVal(i);\n    byPtr(&i);\n    return 0;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int i = 3;", "Creates variable i on the stack holding 3."}, // Step 1
                        {"byVal(i);", "Passes a COPY of 3 to j. Setting j = 5 leaves i still 3!"}, // Step 2
                        {"byPtr(&i);", "Passes the memory ADDRESS of i into pointer p."}, // Step 3
                        {"*p = 5;", "Dereferences p and changes the original variable i in main() to 5!"} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab 2 Q2: swap_chars (Pointers)", // Level title
                    "void swap_chars(char *a, char *b) {\n    char temp = *a;\n    *a = *b;\n    *b = temp;\n}\nint main() {\n    char c1 = 'A', c2 = 'B';\n    swap_chars(&c1, &c2);\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"char c1 = 'A', c2 = 'B';", "Creates two character variables in main()."}, // Step 1
                        {"swap_chars(&c1, &c2);", "Passes addresses of c1 and c2 into char pointers a and b."}, // Step 2
                        {"char temp = *a; *a = *b; *b = temp;", "Exchanges the characters in memory: c1 becomes 'B', c2 becomes 'A'!"} // Step 3
                    }
                }
            }},
            {"7. Pointers & Pointer Math", { // Visualizer levels for Topic 7
                { // Begin first level definition
                    "Lab: Pointer Arithmetic on Arrays", // Level title
                    "float a[4];\nfloat *ptr = &(a[2]);\n*ptr = 3.14f;\nptr++;\n*ptr = 9.0f;\nptr = ptr - 3;\n*ptr = 6.0f;", // Complete source code for overview
                    { // Step-by-step explanations
                        {"float *ptr = &(a[2]);", "Points ptr directly at element a[2] (same as a + 2)."}, // Step 1
                        {"*ptr = 3.14f;", "Writes 3.14 into a[2]."}, // Step 2
                        {"ptr++; *ptr = 9.0f;", "Advances ptr by sizeof(float) to a[3] and sets it to 9.0."}, // Step 3
                        {"ptr = ptr - 3; *ptr = 6.0f;", "Steps back 3 elements to a[0] and sets it to 6.0."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab 2 Q3: find_max Walkthrough", // Level title
                    "int find_max(int *arr, int size) {\n    int maxVal = *arr;\n    for (int i = 1; i < size; i++) {\n        if (*(arr + i) > maxVal) maxVal = *(arr + i);\n    }\n    return maxVal;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int maxVal = *arr;", "Dereferences arr to grab the 1st element (arr[0]) as initial max."}, // Step 1
                        {"for (int i = 1; i < size; i++)", "Scans remaining offsets 1 through size - 1."}, // Step 2
                        {"if (*(arr + i) > maxVal)", "Uses pointer arithmetic *(arr + i) with zero array indexing!"}, // Step 3
                        {"return maxVal;", "Returns the maximum integer found."} // Step 4
                    }
                }
            }},
            {"8. Heap Memory & Func Pointers", { // Visualizer levels for Topic 8
                { // Begin first level definition
                    "Lab: malloc() & free() Lifecycle", // Level title
                    "int *arr = (int*)malloc(5 * sizeof(int));\nif (arr == NULL) return 1;\nfor (int i = 0; i < 5; i++) arr[i] = i * 10;\nfree(arr);\narr = NULL;", // Complete source code for overview
                    { // Step-by-step explanations
                        {"malloc(5 * sizeof(int))", "Requests 5 * 4 = 20 bytes on the heap at runtime."}, // Step 1
                        {"if (arr == NULL) return 1;", "Protects against out-of-memory allocation failure."}, // Step 2
                        {"arr[i] = i * 10;", "Uses the heap block just like a normal array (0, 10, 20, 30, 40)."}, // Step 3
                        {"free(arr); arr = NULL;", "Returns heap memory to OS and clears pointer so it isn't dangling."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: Function Pointers in C", // Level title
                    "int Add(int a, int b) { return a + b; }\nint main() {\n    int (*funcPtr)(int, int) = Add;\n    int res = funcPtr(20, 10);\n    return 0;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int Add(int a, int b)", "Regular C function taking two ints and returning an int."}, // Step 1
                        {"int (*funcPtr)(int, int) = Add;", "Stores the memory address of Add inside funcPtr."}, // Step 2
                        {"int res = funcPtr(20, 10);", "Calls Add(20, 10) indirectly through the pointer; res becomes 30."} // Step 3
                    }
                }
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Visualizer levels for Topic 9
                { // Begin first level definition
                    "Linux Lab: fork() & One-Way Pipe", // Level title
                    "int fd[2]; pipe(fd);\nif (fork() == 0) {\n    close(fd[1]);\n    read(fd[0], buf, 100);\n    close(fd[0]);\n} else {\n    close(fd[0]);\n    write(fd[1], msg, len);\n    close(fd[1]);\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int fd[2]; pipe(fd);", "Creates kernel pipe BEFORE fork(): fd[0] = read, fd[1] = write."}, // Step 1
                        {"if (fork() == 0)", "Clones process. Child enters 'if', Parent enters 'else'."}, // Step 2
                        {"close(fd[1]); read(fd[0], ...);", "Child closes unused write end, then waits to read message."}, // Step 3
                        {"close(fd[0]); write(fd[1], ...);", "Parent closes unused read end, writes message, and hangs up."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Bit Lab: Count 1-Bits Algorithm", // Level title
                    "int NumberOf1Bits(unsigned char x) {\n    int count = 0;\n    while (x != 0) {\n        if (x & 1) count++;\n        x >>= 1;\n    }\n    return count;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int count = 0;", "Starts our counter of set (1) bits at 0."}, // Step 1
                        {"while (x != 0)", "Continues shifting until all 1-bits have been counted."}, // Step 2
                        {"if (x & 1) count++;", "Bitwise AND with 0000 0001 checks if the rightmost bit is 1."}, // Step 3
                        {"x >>= 1;", "Shifts bits right by 1 so the next bit moves into position 0."}, // Step 4
                        {"return count;", "Returns how many 1-bits were inside the 8-bit number."} // Step 5
                    }
                }
            }}
        };
    }
    else { // Otherwise populate curriculum content for C++
        db.theoryTopics = { // Populate all C++ theory topics and flashcards
            {"1. Classes, Objects & Structs", { // Topic 1 covering C++ classes, structs, and access control
                {"Blueprint vs House (Class vs Object)", "In C++, a CLASS is a blueprint that bundles related data (fields) and actions (methods) together. An OBJECT (instance) is a real item built from that blueprint.\n\nclass Dog {\n    std::string name;\npublic:\n    void bark() { std::cout << \"Woof!\"; }\n}; // Never forget this semicolon!"}, // Flashcard explaining classes vs objects
                {"Access Modifiers: class vs struct", "Access modifiers control who can touch your data:\n- private: ONLY the class's own methods can access it.\n- public: ANY code (like main) can access it.\n\nDefault rule:\n- class members are PRIVATE by default.\n- struct members are PUBLIC by default."}, // Flashcard comparing class and struct default access
                {"Composition & The 'this' Pointer", "A class's fields can be objects of ANOTHER class! E.g., a Vector class can hold two Point objects (Point start, end;), accessed as vec.start.x.\n\nInside any method, 'this' is a hidden pointer to the current object instance (this->x = 0.0;)."}, // Flashcard explaining object composition and this pointer
                {"Header Files & Static Members", "Large programs declare a class in a header (vector.h) and write its methods in vector.cpp using '::' (void Point::print()).\n\nStatic members (Maths::PI, Maths::Mul()) belong to the class itself—not one object—and cannot access non-static fields."} // Flashcard explaining header separation and static members
            }},
            {"2. Constructors, Copy & Destructors", { // Topic 2 covering object lifecycle methods
                {"Constructors & Init Lists", "A constructor has the SAME name as the class and runs automatically when an object is born. You can have multiple constructors!\n\nPoint() : x(0.0), y(0.0) {} // default\nPoint(double nx, double ny) : x(nx), y(ny) {}\nThe ': x(nx)' syntax is called an initialization list."}, // Flashcard explaining overloaded constructors and init lists
                {"Shallow vs Deep Copy", "When you copy an object (Student s2 = s1;), C++'s default copy constructor copies every field.\nIf a field is a raw pointer (char *name), BOTH objects end up pointing to the SAME memory! Changing s2.name accidentally changes s1.name.\nFix: Write a custom Copy Constructor that allocates fresh memory (Deep Copy)."}, // Flashcard contrasting shallow and deep copying
                {"Destructors (~ClassName)", "A destructor (~ClassName()) runs automatically when an object dies (leaves scope or is deleted).\n\nIf your constructor allocated heap memory with 'new', your destructor MUST free it with 'delete' or 'delete[]' so your program doesn't leak gigabytes of RAM!"}, // Flashcard explaining destructors and resource cleanup
                {"Arrays of Objects & Pointers", "- Stack array: Point pts[20]; (auto-freed)\n- Heap array: Point *pts = new Point[30]; -> free with delete[] pts;\n- Array of pointers: Point **pp = new Point*[30]; -> delete each pp[i] in a loop, THEN delete[] pp;"} // Flashcard explaining object array allocation and cleanup
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Topic 3 covering C++ memory management and references
                {"Scope & Dangling Pointers", "Variables created inside { } live on the STACK and disappear when } is reached.\n\nIf a pointer still holds the address of a variable that already disappeared (e.g., returning &x from a function), it becomes a DANGLING POINTER—pointing to invalid garbage memory!"}, // Flashcard explaining scope lifetime and dangling pointers
                {"new and delete (Heap Memory)", "To keep data alive after a function finishes, allocate it on the HEAP with 'new':\nint *p = new int(5);\nWhen finished, manually free it with 'delete p;'.\n\nGolden Rules: Never use memory after delete, never delete twice, and never delete stack variables!"}, // Flashcard explaining new and delete operators
                {"Memory Leaks & Dynamic Arrays", "If you overwrite a heap pointer inside a loop without calling 'delete' first, the old memory is lost forever—a MEMORY LEAK.\n\nFor variable-size arrays, use new[] and delete[]:\nint *arr = new int[numItems];\ndelete[] arr;\nFor 2D arrays (int**), delete[] every row first, then delete[] the outer array!"}, // Flashcard explaining dynamic arrays and leaks
                {"Pass by Reference (&) vs Pointer (*)", "C++ adds Pass by Reference (void swap(int &a, int &b)): the parameter becomes a direct alias for the caller's variable—cleaner than pointers!\n\nKey difference: A pointer can be 'nullptr' (pointing to nothing), while a reference MUST always refer to a valid variable."} // Flashcard comparing references and pointers
            }},
            {"4. Singly & Doubly Linked Lists", { // Topic 4 covering pointer-based linked lists
                {"What is a Linked List?", "Unlike an array (one rigid block of lockers), a Linked List is like a treasure hunt: each element (Node) sits anywhere on the heap and holds:\n1. data (the value)\n2. next (a pointer to the next Node, or nullptr at the end)."}, // Flashcard introducing node-based linked lists
                {"Traversing & Inserting Nodes", "Start at 'head' and follow the arrows until nullptr:\nNode *cur = head;\nwhile (cur != nullptr) { cur = cur->next; }\n\nTo insert at the end via a standalone function, pass Node** ptrToHeadPtr so it can update head when the list is empty."}, // Flashcard explaining traversal and tail insertion
                {"Deleting a Node Safely", "To delete a node in the middle without breaking the chain, keep TWO pointers: 'cur' and 'prev'.\n1. Walk until cur->data == value.\n2. Bypass cur: prev->next = cur->next;\n3. Free memory: delete cur;"}, // Flashcard explaining node removal with prev and cur pointers
                {"Doubly Linked Lists & OOP Class", "A DoublyLinkedNode stores BOTH prevPointer and nextPointer so you can walk forward AND backward!\n\nWhen wrapping a LinkedList in a C++ class, make 'head' private and implement:\n- Destructor (~LinkedList) calling clear()\n- Deep Copy Constructor & operator= (checking if (this != &other))."} // Flashcard explaining doubly linked lists and Rule of Three
            }},
            {"5. Inheritance & Polymorphism", { // Topic 5 covering class hierarchies and virtual dispatch
                {"Inheritance: Reusing Blueprints", "Inheritance lets you create specialized child classes (Car, Truck) from a shared base class (Vehicle):\nclass Car : public Vehicle { ... };\n\nMembers marked 'protected:' are hidden from main(), but derived classes CAN access them directly. Call Vehicle::display() to extend base methods."}, // Flashcard introducing base and derived classes
                {"Construction & Destruction Order", "When a Derived object containing a Member object is created:\n1. Base constructor -> 2. Member constructor -> 3. Derived constructor.\n\nWhen it is destroyed, the order is EXACTLY REVERSED:\n1. ~Derived -> 2. ~Member -> 3. ~Base."}, // Flashcard explaining construction and destruction sequencing
                {"Polymorphism: 'Many Forms'", "Polymorphism lets a base pointer (Shape *s = new Circle();) automatically call the derived class's version of a method at runtime!\n\nHow: Mark the base method 'virtual', and mark the derived method 'override'."}, // Flashcard explaining runtime polymorphism with virtual/override
                {"Why Virtual Destructors Matter", "CRITICAL RULE: If a base class has ANY virtual functions, it MUST have a virtual destructor:\nvirtual ~Shape() {}\n\nWithout it, 'delete shapePtr;' only runs ~Shape() and skips ~Circle(), causing a memory leak!"} // Flashcard explaining virtual destructors
            }},
            {"6. Abstract, Interface & Friends", { // Topic 6 covering pure virtual functions and friend access
                {"Abstract Classes (= 0)", "An abstract class is an incomplete blueprint containing at least one PURE VIRTUAL function:\nvirtual double area() = 0;\n\nRules:\n- You CANNOT create objects of an abstract class directly.\n- Every derived class MUST implement all '= 0' functions."}, // Flashcard introducing abstract base classes
                {"Interface Classes (Pure Contracts)", "An Interface is a special abstract class that defines WHAT a class must do without HOW:\n- Contains ONLY pure virtual functions (= 0)\n- Usually no data members\n- Always includes a virtual destructor\n- Often prefixed with 'I' (e.g., IPrintable)."}, // Flashcard explaining pure interface classes
                {"Friend Functions", "Normally, outside functions cannot read 'private:' variables. Declaring a 'friend' function inside a class creates an explicit exception:\n\nclass Box {\n    int length;\n    friend void printLength(Box b);\n}; // printLength is NOT a member, yet can read b.length!"}, // Flashcard explaining friend functions
                {"Friend Classes", "Declaring 'friend class BestFriend;' inside SecretData gives ALL methods of BestFriend access to SecretData's private members.\nUse friends sparingly—for tightly coupled classes or unit testing private data."} // Flashcard explaining friend classes
            }},
            {"7. Operator Overloading", { // Topic 7 covering function and operator overloading
                {"Function Overloading", "In C++, multiple functions in the same scope can share the same name (e.g., print(int), print(double), print(char*)) as long as their parameter types or counts differ.\nNote: You CANNOT overload functions that differ ONLY by return type!"}, // Flashcard explaining function signature overloading
                {"Overloading Binary Operators (+)", "You can teach C++ what '+' means for your own classes using the 'operator' keyword:\n\nBox operator+(const Box& b) {\n    Box res;\n    res.length = this->length + b.length;\n    return res;\n}\nNow 'Box3 = Box1 + Box2;' works naturally!"}, // Flashcard explaining binary operator+ overloading
                {"Overloading Stream Output (<<)", "To print your class with 'cout << myClock;', overload << as a friend non-member function:\n\nfriend ostream& operator<<(ostream& os, Clock& c);\nAlways return 'os' by reference so chaining (cout << c1 << c2) works!"}, // Flashcard explaining ostream insertion overloading
                {"Prefix (++c) vs Postfix (c++)", "How does C++ tell ++c and c++ apart?\n- Prefix:  Clock operator++() { tick(); return *this; }\n- Postfix: Clock operator++(int) takes a dummy unnamed 'int'! It saves a copy of *this, calls tick(), and returns the OLD copy."} // Flashcard distinguishing prefix and postfix increment operators
            }},
            {"8. Templates (Generic Code)", { // Topic 8 covering generic functions and classes
                {"What is a Template?", "Think of a template like a cookie-cutter mold. Instead of writing separate sum() functions for int, float, and double, you write ONE generic template and let the compiler stamp out the exact version you need!"}, // Flashcard introducing generic templates
                {"Template Functions", "Use 'template <typename T>' before the function:\n\ntemplate <typename T>\nT sum(T const* arr, int count) { ... }\n\nThe compiler automatically figures out T from the arguments you pass (e.g., sum(intArr, 10)). Note: T is NEVER deduced from the return type alone!"}, // Flashcard explaining template function type deduction
                {"Template Classes", "Entire classes can be generic too:\ntemplate <typename T> class Adder { T m_sum; ... };\n\nUnlike functions, template classes require explicit angle brackets when creating objects:\nAdder<int> intAdder;\nAdder<float> floatAdder;"}, // Flashcard explaining template class instantiation
                {"External Defs & Multiple Types", "1. Defining a template method outside its class requires repeating the header:\n   template <typename T> void Adder<T>::add(T n) { ... }\n2. Templates can take multiple generic types separated by commas:\n   template <typename SumType, typename AvgType>"} // Flashcard explaining out-of-line definitions and multi-parameter templates
            }},
            {"9. STL Vectors, Iterators & Algos", { // Topic 9 covering std::vector, iterators, and STL algorithms
                {"The 3 Pillars of STL", "The Standard Template Library (STL) gives you battle-tested tools built on 3 pillars:\n1. Containers (vector, list, map, stack, queue)\n2. Algorithms (<algorithm>: sort, remove, min, max)\n3. Iterators (generalized pointers connecting algorithms to containers)"}, // Flashcard introducing STL architecture
                {"std::vector: Size vs Capacity", "std::vector is a smart dynamic array that grows automatically via push_back().\n- size(): how many items are currently inside.\n- capacity(): how many slots are allocated in memory.\nUse reserve(n) upfront to avoid slow reallocations, and shrink_to_fit() to trim unused memory."}, // Flashcard comparing vector size and capacity
                {"Iterators, begin() & end()", "Iterators use pointer-like syntax (++, --, *):\nfor (auto it = v.begin(); it != v.end(); ++it) cout << *it;\n\nCRITICAL: v.end() points ONE PAST the last item—NEVER dereference v.end()!\nAlso: push_back(), erase(), or reserve() can INVALIDATE existing vector iterators!"}, // Flashcard explaining iterators and invalidation hazards
                {"Erase-Remove, Predicates & Ranges", "- Erase-Remove Idiom removes all matching values cleanly:\n  v.erase(remove(v.begin(), v.end(), 3), v.end());\n- A Predicate is a bool function passed to remove_if() or sort().\n- Prefer range insert v.insert(v.begin(), data, data+n) over loops to avoid shifting elements repeatedly."} // Flashcard explaining erase-remove idiom and predicates
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Topic 10 covering std::list, maps, and smart pointers
                {"std::list & splice()", "std::list is a doubly linked list (fast middle insert/delete, NO random access v[i]).\nOnly std::list supports splice()—moving nodes between lists with ZERO copying!\nTip: Always check list.empty() (O(1)) instead of list.size() == 0."}, // Flashcard explaining std::list and splice
                {"std::map vs unordered_map", "Maps store unique Key -> Value pairs (like a dictionary or DNS table):\n- std::map (<map>): binary tree, keys stay SORTED (requires operator<).\n- std::unordered_map (C++11): hash table, faster lookup, UNSORTED.\nWith iterators: it->first is the key, it->second is the value."}, // Flashcard comparing ordered and unordered maps
                {"Map insert() vs []", "freq[\"One\"] = 1 overwrites if \"One\" already exists.\nfreq.insert(make_pair(\"One\", 1)) will NOT overwrite! It returns pair<iterator, bool> where .second is false if the key already existed."}, // Flashcard contrasting map insertion methods
                {"Smart Pointers (unique & shared)", "Smart pointers delete heap memory AUTOMATICALLY—no manual delete or leaks!\n- std::unique_ptr: EXCLUSIVE single owner. Cannot be copied; transfer ownership with std::move(myBook) (original becomes nullptr).\n- std::shared_ptr: MULTIPLE owners via reference counting; object is deleted only when the LAST owner leaves scope."} // Flashcard explaining RAII smart pointers
            }}
        };

        db.quizTopics = { // Populate all C++ quiz topics and questions
            {"1. Classes, Objects & Structs", { // Quiz questions for C++ Topic 1
                {"What is the default access modifier for a C++ class vs a C++ struct?", {"Both are public", "class is private, struct is public", "class is public, struct is private", "Both are private"}, 1}, // Question on default access specifiers
                {"What must ALWAYS follow the closing brace '}' of a C++ class definition?", {"A colon :", "A semicolon ;", "The keyword end", "Nothing"}, 1}, // Question on trailing semicolon after class body
                {"Inside a class method, what is 'this'?", {"The name of the file", "A pointer to the current object instance", "A static integer", "The base class"}, 1}, // Question on the this pointer
                {"Can a static member function access non-static member variables directly?", {"Yes, always", "No, that causes a compile error", "Only if they are private", "Only inside main()"}, 1} // Question on static member restrictions
            }},
            {"2. Constructors, Copy & Destructors", { // Quiz questions for C++ Topic 2
                {"When is a class constructor called?", {"When the program exits", "Automatically when an object is created", "Only when you call delete", "When a loop ends"}, 1}, // Question on constructor invocation timing
                {"Why does a class containing a raw pointer (like char *name) need a custom Copy Constructor?", {"To make it compile", "To perform a deep copy instead of sharing the same pointer", "Because structs cannot be copied", "To disable private fields"}, 1}, // Question on deep copy necessity
                {"When is a destructor (~ClassName) called?", {"When an object goes out of scope or is deleted", "Right before the constructor", "Only when you call free()", "At compile time"}, 0}, // Question on destructor invocation timing
                {"How do you free a heap array allocated with 'Point *pts = new Point[30];'?", {"delete pts;", "free(pts);", "delete[] pts;", "pts.clear();"}, 2} // Question on array delete operator
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Quiz questions for C++ Topic 3
                {"What is a dangling pointer?", {"A pointer set to nullptr", "A pointer referencing memory that already went out of scope or was deleted", "A pointer to a global int", "A static pointer"}, 1}, // Question on dangling pointer definition
                {"Where does the 'new' operator allocate memory?", {"On the stack", "On the heap", "Inside the CPU register", "In read-only ROM"}, 1}, // Question on heap allocation with new
                {"What happens if you allocate with 'new' inside a loop and only call 'delete' once AFTER the loop?", {"All memory is freed", "A memory leak—only the last allocation is freed", "Compile error", "Nothing is allocated"}, 1}, // Question on loop memory leak
                {"What is one key difference between Pass by Pointer and Pass by Reference in C++?", {"Pointers can be nullptr, references cannot", "References cannot modify the caller's variable", "Pointers are only in C", "References require new/delete"}, 0} // Question on nullability difference
            }},
            {"4. Singly & Doubly Linked Lists", { // Quiz questions for C++ Topic 4
                {"In a singly linked list, how do you know a Node is the LAST node in the list?", {"node->data == 0", "node->next == nullptr", "node == head", "node->prev == nullptr"}, 1}, // Question on list tail detection
                {"Why do we track a 'prev' pointer when deleting a middle node in a singly linked list?", {"To sort the list", "To relink prev->next = cur->next before deleting cur", "To count nodes", "To call malloc()"}, 1}, // Question on pointer relinking during deletion
                {"What extra pointer does a DoublyLinkedNode have compared to a singly linked Node?", {"headPointer", "prevPointer", "tailPointer", "dataPointer"}, 1}, // Question on backward link pointer
                {"Inside LinkedList::operator=, why do we check 'if (this != &other)'?", {"To prevent self-assignment from wiping our own data", "To reverse the list", "To check if memory is full", "To print the list"}, 0} // Question on self-assignment guard
            }},
            {"5. Inheritance & Polymorphism", { // Quiz questions for C++ Topic 5
                {"Who can access 'protected:' members of a Base class?", {"Everyone including main()", "The Base class and its Derived classes", "Nobody at all", "Only friend functions"}, 1}, // Question on protected access scope
                {"What is the exact DESTRUCTION order when a Derived object with a Member object goes out of scope?", {"Base -> Member -> Derived", "Derived -> Member -> Base", "Member -> Base -> Derived", "Base -> Derived -> Member"}, 1}, // Question on reverse destruction order
                {"Which keyword in a Base class enables runtime polymorphism via base pointers?", {"static", "virtual", "friend", "const"}, 1}, // Question on virtual dispatch keyword
                {"What happens if you delete a Derived object through a Base* pointer WITHOUT a virtual destructor?", {"Both destructors run normally", "Only ~Base() runs, causing a memory leak", "Compile error", "The program restarts"}, 1} // Question on missing virtual destructor leak
            }},
            {"6. Abstract, Interface & Friends", { // Quiz questions for C++ Topic 6
                {"How do you declare a pure virtual function in C++?", {"abstract void f();", "virtual void f() = 0;", "pure void f();", "virtual void f() = null;"}, 1}, // Question on pure virtual syntax
                {"Can you directly instantiate an object of an abstract class?", {"Yes, on the stack", "No, it causes a compile error", "Only with new", "Only if it has a constructor"}, 1}, // Question on abstract class instantiation
                {"What defines a C++ Interface class?", {"Only private variables", "Only pure virtual functions (= 0) and a virtual destructor", "No virtual functions", "Inherits from std::vector"}, 1}, // Question on interface class structure
                {"Is a 'friend' function a member function of the class that declares it?", {"Yes, always", "No, it is an external function granted access to private members", "Only if it is static", "Only if it is virtual"}, 1} // Question on non-member status of friend functions
            }},
            {"7. Operator Overloading", { // Quiz questions for C++ Topic 7
                {"Can you overload two functions that have the exact same parameters and differ ONLY by return type?", {"Yes", "No, parameter types or count must differ", "Only for int and double", "Only inside structs"}, 1}, // Question on return-type-only overloading
                {"How does C++ distinguish postfix operator++ (c++) from prefix operator++ (++c)?", {"By return type", "Postfix takes a dummy 'int' parameter", "Prefix takes a 'bool' parameter", "Postfix is always private"}, 1}, // Question on postfix dummy int parameter
                {"Why should overloaded 'operator<<' return 'ostream&'?", {"To save memory", "So stream outputs can be chained (cout << c1 << c2)", "To prevent compilation", "Because void is illegal"}, 1}, // Question on stream chaining return type
                {"Inside a member 'Box operator+(const Box& b)', how do you access the LEFT-hand operand?", {"Through 'b'", "Through 'this'", "Through 'main'", "Through 'operator'"}, 1} // Question on implicit left operand via this
            }},
            {"8. Templates (Generic Code)", { // Quiz questions for C++ Topic 8
                {"How does the C++ compiler determine the type T when you call a template function?", {"From its return type only", "By analyzing the argument types passed to it", "It always picks int", "From the file name"}, 1}, // Question on template argument deduction
                {"How must you declare an object of a template class like 'Adder'?", {"Adder intAdder;", "Adder<int> intAdder;", "template Adder intAdder;", "new Adder(int);"}, 1}, // Question on explicit template argument syntax
                {"Can a single template function or class have multiple generic type parameters?", {"No, only one (T)", "Yes, separated by commas (template <typename X, typename Y>)", "Only up to 2", "Only for structs"}, 1}, // Question on multi-parameter templates
                {"What is the main advantage of C++ templates?", {"They eliminate writing duplicate code for different data types", "They replace all pointers", "They remove the need for main()", "They disable type checking"}, 0} // Question on code reuse via templates
            }},
            {"9. STL Vectors, Iterators & Algos", { // Quiz questions for C++ Topic 9
                {"What is the difference between vector::size() and vector::capacity()?", {"They are always identical", "size() is current elements; capacity() is total allocated slots", "capacity() is always smaller", "size() is measured in bytes"}, 1}, // Question on vector size vs capacity
                {"What does v.end() point to?", {"The last element in v", "One position PAST the last element (do not dereference!)", "The first element", "nullptr"}, 1}, // Question on past-the-end iterator semantics
                {"What happens to existing vector iterators when push_back() causes memory reallocation?", {"They update automatically", "They become INVALIDATED and unsafe to use", "They point to 0", "Nothing happens"}, 1}, // Question on iterator invalidation
                {"Why must you call v.erase() AFTER std::remove() in the Erase-Remove idiom?", {"remove() only shifts elements forward and doesn't shrink the vector", "remove() deletes the whole vector", "erase() sorts the vector", "It is optional"}, 0} // Question on why erase is required after remove
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Quiz questions for C++ Topic 10
                {"Which STL container supports splice() to move nodes without copying data?", {"std::vector", "std::map", "std::list", "std::queue"}, 2}, // Question on list::splice
                {"What is the main difference between std::map and std::unordered_map?", {"std::map keeps keys sorted (tree); unordered_map uses a hash table", "unordered_map allows duplicate keys", "std::map is only for ints", "No difference"}, 0}, // Question on map vs unordered_map
                {"After 'auto yourBook = std::move(myBook);' with std::unique_ptr, what is myBook?", {"A copy of yourBook", "nullptr (no longer owns the Book)", "Deleted from stack", "Still owns the Book"}, 1}, // Question on unique_ptr moved-from state
                {"When does std::shared_ptr automatically delete its managed object?", {"After the first copy", "When the LAST shared_ptr reference goes out of scope", "Never—you must call delete", "Immediately after make_shared"}, 1} // Question on shared_ptr reference count cleanup
            }}
        };

        db.constructorTopics = { // Populate all C++ code constructor topics and levels
            {"1. Classes, Objects & Structs", { // Constructor levels for C++ Topic 1
                { // Begin first level definition
                    "Lab: Class with Init List & Getter", // Level title
                    { // Ordered correct lines of code
                        "class Point {", // Line 1: declare class
                        "private:", // Line 2: private section
                        "    double x, y;", // Line 3: member fields
                        "public:", // Line 4: public section
                        "    Point(double nx, double ny) : x(nx), y(ny) {}", // Line 5: constructor with init list
                        "    double getX() { return x; } };" // Line 6: getter and closing brace
                    },
                    { // Explanations for each slot
                        "Declare the Point class.", // Explanation for line 1
                        "Encapsulate fields under private.", // Explanation for line 2
                        "Declare x and y coordinates.", // Explanation for line 3
                        "Open the public interface section.", // Explanation for line 4
                        "Initialize x and y via member initialization list.", // Explanation for line 5
                        "Provide getter getX() and close class with ';'." // Explanation for line 6
                    },
                    {"Point(double nx, double ny) { nx = x; }"} // Distractor reversed assignment constructor
                },
                { // Begin second level definition
                    "Lab: Class Composition (Vector)", // Level title
                    { // Ordered correct lines of code
                        "class Vector {", // Line 1: declare class
                        "public:", // Line 2: public section
                        "    Point start;", // Line 3: composed Point field
                        "    Point end;", // Line 4: composed Point field
                        "    void print() { start.print(); }", // Line 5: print method
                        "};" // Line 6: close class
                    },
                    { // Explanations for each slot
                        "Declare the Vector class.", // Explanation for line 1
                        "Expose start and end Points publicly.", // Explanation for line 2
                        "Include a Point object for start.", // Explanation for line 3
                        "Include a Point object for end.", // Explanation for line 4
                        "Define print() calling start's own method.", // Explanation for line 5
                        "Close class definition with semicolon." // Explanation for line 6
                    },
                    {"class Vector : Point {"} // Distractor inheritance instead of composition
                }
            }},
            {"2. Constructors, Copy & Destructors", { // Constructor levels for C++ Topic 2
                { // Begin first level definition
                    "Lab: Deep Copy Constructor", // Level title
                    { // Ordered correct lines of code
                        "MITStudent(const MITStudent &o) {", // Line 1: copy constructor signature
                        "    studentID = o.studentID;", // Line 2: copy integer ID
                        "    name = _strdup(o.name);", // Line 3: duplicate string buffer
                        "}" // Line 4: close constructor
                    },
                    { // Explanations for each slot
                        "Declare copy constructor taking const reference.", // Explanation for line 1
                        "Copy the integer studentID directly.", // Explanation for line 2
                        "Allocate a separate copy of the string (deep copy!).", // Explanation for line 3
                        "Close copy constructor." // Explanation for line 4
                    },
                    {"name = o.name;"} // Distractor shallow pointer copy
                },
                { // Begin second level definition
                    "Lab: Destructor Heap Cleanup", // Level title
                    { // Ordered correct lines of code
                        "class PointBag {", // Line 1: declare class
                        "    Point *points;", // Line 2: raw array pointer
                        "public:", // Line 3: public section
                        "    PointBag() { points = new Point[100]; }", // Line 4: allocate array in constructor
                        "    ~PointBag() { delete[] points; }", // Line 5: free array in destructor
                        "};" // Line 6: close class
                    },
                    { // Explanations for each slot
                        "Declare PointBag class.", // Explanation for line 1
                        "Private pointer for heap array.", // Explanation for line 2
                        "Expose constructor and destructor publicly.", // Explanation for line 3
                        "Allocate 100 Points on heap in constructor.", // Explanation for line 4
                        "Free array with delete[] in destructor (~PointBag).", // Explanation for line 5
                        "Close class with semicolon." // Explanation for line 6
                    },
                    {"~PointBag() { delete points; }"} // Distractor scalar delete instead of delete[]
                }
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Constructor levels for C++ Topic 3
                { // Begin first level definition
                    "Lab 1 Q12: Delete Array Element (Ref)", // Level title
                    { // Ordered correct lines of code
                        "bool delElem(int& count, int arr[], int idx) {", // Line 1: function header with reference
                        "    if (idx < 0 || idx >= count) return false;", // Line 2: bounds check
                        "    for (int i = idx; i < count - 1; i++) {", // Line 3: loop to shift left
                        "        arr[i] = arr[i + 1];", // Line 4: overwrite with next element
                        "    }", // Line 5: close loop
                        "    count--; return true; }" // Line 6: decrement count and return
                    },
                    { // Explanations for each slot
                        "Pass count by reference (int&) to update caller.", // Explanation for line 1
                        "Validate index is inside [0, count - 1].", // Explanation for line 2
                        "Loop from deleted index to second-to-last item.", // Explanation for line 3
                        "Shift elements LEFT by 1 to close the gap.", // Explanation for line 4
                        "Close loop.", // Explanation for line 5
                        "Decrement count, return true, close function." // Explanation for line 6
                    },
                    {"arr[i + 1] = arr[i];"} // Distractor right shift overwriting data
                },
                { // Begin second level definition
                    "Lab: Dynamic 2D Array Cleanup", // Level title
                    { // Ordered correct lines of code
                        "int **m = new int*[rows];", // Line 1: allocate row pointer array
                        "for (int r = 0; r < rows; r++)", // Line 2: loop rows for allocation
                        "    m[r] = new int[cols];", // Line 3: allocate each row
                        "for (int r = 0; r < rows; r++)", // Line 4: loop rows for deletion
                        "    delete[] m[r];", // Line 5: free each row
                        "delete[] m;" // Line 6: free outer row pointer array
                    },
                    { // Explanations for each slot
                        "Allocate array of row pointers on heap.", // Explanation for line 1
                        "Loop over every row.", // Explanation for line 2
                        "Allocate cols ints for each row.", // Explanation for line 3
                        "Loop over every row again for cleanup.", // Explanation for line 4
                        "Free each row array first with delete[].", // Explanation for line 5
                        "Finally free the outer row-pointer array." // Explanation for line 6
                    },
                    {"delete m;"} // Distractor leaking inner rows
                }
            }},
            {"4. Singly & Doubly Linked Lists", { // Constructor levels for C++ Topic 4
                { // Begin first level definition
                    "Lab 2 Q4: Reverse Array via Pointers", // Level title
                    { // Ordered correct lines of code
                        "int *left = arr;", // Line 1: left pointer at start
                        "int *right = arr + size - 1;", // Line 2: right pointer at end
                        "while (left < right) {", // Line 3: loop until pointers cross
                        "    int tmp = *left; *left = *right;", // Line 4: save left and copy right
                        "    *right = tmp;", // Line 5: assign saved value to right
                        "    left++; right--; }" // Line 6: advance pointers inward and close
                    },
                    { // Explanations for each slot
                        "Point left at index 0.", // Explanation for line 1
                        "Point right at the last element using pointer math.", // Explanation for line 2
                        "Loop until pointers meet in the middle.", // Explanation for line 3
                        "Swap values using tmp.", // Explanation for line 4
                        "Store saved tmp into *right.", // Explanation for line 5
                        "Step pointers inward and close loop." // Explanation for line 6
                    },
                    {"while (left <= arr + size) {"} // Distractor out-of-bounds loop condition
                },
                { // Begin second level definition
                    "Lab 2 Pt2: DoublyLinkedNode Class", // Level title
                    { // Ordered correct lines of code
                        "class DoublyLinkedNode {", // Line 1: declare node class
                        "public:", // Line 2: public section
                        "    int data;", // Line 3: node payload
                        "    DoublyLinkedNode *prevPointer;", // Line 4: backward link pointer
                        "    DoublyLinkedNode *nextPointer;", // Line 5: forward link pointer
                        "};" // Line 6: close class
                    },
                    { // Explanations for each slot
                        "Declare DoublyLinkedNode class.", // Explanation for line 1
                        "Make fields accessible to DoublyLinkedList.", // Explanation for line 2
                        "Store integer payload data.", // Explanation for line 3
                        "Store pointer to PREVIOUS node.", // Explanation for line 4
                        "Store pointer to NEXT node.", // Explanation for line 5
                        "Close class with semicolon." // Explanation for line 6
                    },
                    {"DoublyLinkedNode prevPointer;"} // Distractor recursive value member instead of pointer
                },
                { // Begin third level definition
                    "Lab 2 Pt2: InsertAtBeginning (DLL)", // Level title
                    { // Ordered correct lines of code
                        "DoublyLinkedNode *n = new DoublyLinkedNode(val);", // Line 1: allocate new node
                        "n->nextPointer = head;", // Line 2: point new node forward to head
                        "n->prevPointer = nullptr;", // Line 3: set new node prev to null
                        "if (head != nullptr) head->prevPointer = n;", // Line 4: link old head back to n
                        "head = n;" // Line 5: update head pointer
                    },
                    { // Explanations for each slot
                        "Create new node on the heap.", // Explanation for line 1
                        "Link n forward to current head.", // Explanation for line 2
                        "New head has no previous node (nullptr).", // Explanation for line 3
                        "Point old head's prevPointer back to n.", // Explanation for line 4
                        "Update head to point to n." // Explanation for line 5
                    },
                    {"head->nextPointer = n;"} // Distractor attaching after head instead of before
                }
            }},
            {"5. Inheritance & Polymorphism", { // Constructor levels for C++ Topic 5
                { // Begin first level definition
                    "Lab: Polymorphic Derived Class", // Level title
                    { // Ordered correct lines of code
                        "class Circle : public Shape {", // Line 1: public inheritance
                        "public:", // Line 2: public section
                        "    void draw() override {", // Line 3: override virtual method
                        "        std::cout << \"Circle\";", // Line 4: print Circle
                        "    }", // Line 5: close method
                        "};" // Line 6: close class
                    },
                    { // Explanations for each slot
                        "Inherit publicly from Shape.", // Explanation for line 1
                        "Expose draw() publicly.", // Explanation for line 2
                        "Override base virtual draw() safely.", // Explanation for line 3
                        "Print Circle's specialized output.", // Explanation for line 4
                        "Close draw() method.", // Explanation for line 5
                        "Close Circle class with semicolon." // Explanation for line 6
                    },
                    {"class Circle extends Shape {"} // Distractor Java-style extends syntax
                },
                { // Begin second level definition
                    "Lab: Base Pointer Polymorphism", // Level title
                    { // Ordered correct lines of code
                        "Shape *s1 = new Circle();", // Line 1: allocate Circle via Shape*
                        "Shape *s2 = new Square();", // Line 2: allocate Square via Shape*
                        "s1->draw();", // Line 3: dynamic dispatch to Circle::draw
                        "s2->draw();", // Line 4: dynamic dispatch to Square::draw
                        "delete s1;", // Line 5: delete first shape
                        "delete s2;" // Line 6: delete second shape
                    },
                    { // Explanations for each slot
                        "Store a Circle object in a Shape base pointer.", // Explanation for line 1
                        "Store a Square object in a Shape base pointer.", // Explanation for line 2
                        "Dynamically calls Circle::draw().", // Explanation for line 3
                        "Dynamically calls Square::draw().", // Explanation for line 4
                        "Safely delete s1 (requires virtual ~Shape!).", // Explanation for line 5
                        "Safely delete s2." // Explanation for line 6
                    },
                    {"Shape s1 = new Circle();"} // Distractor missing pointer asterisk
                }
            }},
            {"6. Abstract, Interface & Friends", { // Constructor levels for C++ Topic 6
                { // Begin first level definition
                    "Lab: Defining an Interface Class", // Level title
                    { // Ordered correct lines of code
                        "class IPrintable {", // Line 1: declare interface class
                        "public:", // Line 2: public section
                        "    virtual ~IPrintable() {}", // Line 3: virtual destructor
                        "    virtual void print() = 0;", // Line 4: pure virtual method
                        "};" // Line 5: close class
                    },
                    { // Explanations for each slot
                        "Prefix interface class with 'I'.", // Explanation for line 1
                        "Interface contracts must be public.", // Explanation for line 2
                        "Always include a virtual destructor.", // Explanation for line 3
                        "Declare pure virtual method using '= 0'.", // Explanation for line 4
                        "Close class with semicolon." // Explanation for line 5
                    },
                    {"void print() = 0;"} // Distractor non-virtual pure specifier
                },
                { // Begin second level definition
                    "Lab: Declaring a Friend Function", // Level title
                    { // Ordered correct lines of code
                        "class Box {", // Line 1: declare Box class
                        "private:", // Line 2: private section
                        "    int length;", // Line 3: private field
                        "public:", // Line 4: public section
                        "    friend void printLength(Box b);", // Line 5: friend declaration
                        "};" // Line 6: close class
                    },
                    { // Explanations for each slot
                        "Begin Box class.", // Explanation for line 1
                        "Hide length under private.", // Explanation for line 2
                        "Declare private integer length.", // Explanation for line 3
                        "Open public section.", // Explanation for line 4
                        "Grant printLength() private access using 'friend'.", // Explanation for line 5
                        "Close class with semicolon." // Explanation for line 6
                    },
                    {"public friend printLength(Box b);"} // Distractor invalid friend syntax
                }
            }},
            {"7. Operator Overloading", { // Constructor levels for C++ Topic 7
                { // Begin first level definition
                    "Lab: Postfix Increment (Clock++)", // Level title
                    { // Ordered correct lines of code
                        "Clock Clock::operator++(int) {", // Line 1: postfix signature with dummy int
                        "    Clock old = *this;", // Line 2: save current state
                        "    tick();", // Line 3: increment clock
                        "    return old;", // Line 4: return previous snapshot
                        "}" // Line 5: close function
                    },
                    { // Explanations for each slot
                        "Dummy unnamed 'int' marks postfix ++.", // Explanation for line 1
                        "Save a copy of the current object before ticking.", // Explanation for line 2
                        "Advance the current Clock by 1 minute.", // Explanation for line 3
                        "Return the pre-increment copy.", // Explanation for line 4
                        "Close operator definition." // Explanation for line 5
                    },
                    {"return *this;"} // Distractor returning updated object like prefix
                },
                { // Begin second level definition
                    "Lab: Overloading Box operator+", // Level title
                    { // Ordered correct lines of code
                        "Box operator+(const Box& b) {", // Line 1: member operator+ header
                        "    Box res;", // Line 2: create result object
                        "    res.length = this->length + b.length;", // Line 3: sum lengths
                        "    return res;", // Line 4: return combined Box
                        "}" // Line 5: close function
                    },
                    { // Explanations for each slot
                        "Define member operator+ taking right-hand Box.", // Explanation for line 1
                        "Create temporary result Box.", // Explanation for line 2
                        "Add left operand (this) and right operand (b).", // Explanation for line 3
                        "Return the combined Box.", // Explanation for line 4
                        "Close operator+." // Explanation for line 5
                    },
                    {"Box operator+(Box a, Box b) {"} // Distractor two-parameter member operator
                }
            }},
            {"8. Templates (Generic Code)", { // Constructor levels for C++ Topic 8
                { // Begin first level definition
                    "Lab: Generic sum() Template", // Level title
                    { // Ordered correct lines of code
                        "template <typename T>", // Line 1: template header
                        "T sum(T const* arr, int n) {", // Line 2: generic function signature
                        "    T total = 0;", // Line 3: initialize accumulator
                        "    for (int i = 0; i < n; i++) total += arr[i];", // Line 4: accumulate elements
                        "    return total;", // Line 5: return sum
                        "}" // Line 6: close function
                    },
                    { // Explanations for each slot
                        "Declare generic type placeholder T.", // Explanation for line 1
                        "Accept a const array of T and its length.", // Explanation for line 2
                        "Initialize accumulator of type T to 0.", // Explanation for line 3
                        "Sum all elements in the array.", // Explanation for line 4
                        "Return total of type T.", // Explanation for line 5
                        "Close template function." // Explanation for line 6
                    },
                    {"int sum(T const* arr, int n) {"} // Distractor hardcoded int return type
                },
                { // Begin second level definition
                    "Lab: Template Class Adder<T>", // Level title
                    { // Ordered correct lines of code
                        "template <typename T>", // Line 1: template header
                        "class Adder {", // Line 2: declare class
                        "    T m_sum = 0;", // Line 3: private generic member
                        "public:", // Line 4: public section
                        "    void add(T x) { m_sum += x; }", // Line 5: generic add method
                        "    T sum() { return m_sum; } };" // Line 6: getter and close class
                    },
                    { // Explanations for each slot
                        "Prefix class with template <typename T>.", // Explanation for line 1
                        "Define Adder class.", // Explanation for line 2
                        "Private generic accumulator m_sum.", // Explanation for line 3
                        "Expose public methods.", // Explanation for line 4
                        "Add generic value x to m_sum.", // Explanation for line 5
                        "Return m_sum and close class with ';'." // Explanation for line 6
                    },
                    {"Adder intAdder;"} // Distractor missing template angle brackets
                }
            }},
            {"9. STL Vectors, Iterators & Algos", { // Constructor levels for C++ Topic 9
                { // Begin first level definition
                    "Lab: STL Erase-Remove Idiom", // Level title
                    { // Ordered correct lines of code
                        "#include <algorithm>", // Line 1: include algorithm header
                        "std::vector<int> v = {3, 1, 3, 2};", // Line 2: initialize vector
                        "auto newEnd = std::remove(v.begin(), v.end(), 3);", // Line 3: partition out 3s
                        "v.erase(newEnd, v.end());" // Line 4: erase trailing elements
                    },
                    { // Explanations for each slot
                        "Include <algorithm> for std::remove.", // Explanation for line 1
                        "Create a vector containing duplicate 3s.", // Explanation for line 2
                        "Shift non-3 items forward and capture new logical end.", // Explanation for line 3
                        "Erase trailing junk elements from newEnd to v.end()." // Explanation for line 4
                    },
                    {"v.remove(3);"} // Distractor non-existent vector::remove method
                },
                { // Begin second level definition
                    "Lab: Vector Reserve & Sort", // Level title
                    { // Ordered correct lines of code
                        "std::vector<int> v;", // Line 1: declare vector
                        "v.reserve(100);", // Line 2: pre-allocate capacity
                        "v.push_back(30);", // Line 3: append 30
                        "v.push_back(10);", // Line 4: append 10
                        "std::sort(v.begin(), v.end());" // Line 5: sort range
                    },
                    { // Explanations for each slot
                        "Create empty int vector.", // Explanation for line 1
                        "Pre-allocate capacity for 100 items to avoid reallocations.", // Explanation for line 2
                        "Append 30 to back of vector.", // Explanation for line 3
                        "Append 10 to back of vector.", // Explanation for line 4
                        "Sort entire range [begin, end) using STL sort." // Explanation for line 5
                    },
                    {"std::sort(v);"} // Distractor passing container instead of iterator pair
                }
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Constructor levels for C++ Topic 10
                { // Begin first level definition
                    "Lab: unique_ptr & std::move", // Level title
                    { // Ordered correct lines of code
                        "auto myBook = std::make_unique<Book>(\"C++\");", // Line 1: allocate unique_ptr
                        "myBook->read();", // Line 2: access member via arrow
                        "auto yourBook = std::move(myBook);", // Line 3: transfer ownership
                        "if (!myBook) {", // Line 4: check moved-from pointer is null
                        "    std::cout << \"Moved!\";", // Line 5: print confirmation
                        "}" // Line 6: close if
                    },
                    { // Explanations for each slot
                        "Create unique_ptr exclusively owning Book.", // Explanation for line 1
                        "Call read() using '->'.", // Explanation for line 2
                        "Transfer ownership with std::move (copying is illegal!).", // Explanation for line 3
                        "Verify myBook is now nullptr.", // Explanation for line 4
                        "Confirm ownership moved cleanly.", // Explanation for line 5
                        "Close if block." // Explanation for line 6
                    },
                    {"auto yourBook = myBook;"} // Distractor illegal copy of unique_ptr
                },
                { // Begin second level definition
                    "Lab: Safe STL Map insert()", // Level title
                    { // Ordered correct lines of code
                        "std::map<std::string, int> freq;", // Line 1: declare ordered map
                        "auto res = freq.insert(make_pair(\"One\", 1));", // Line 2: insert key-value pair
                        "if (res.second == false) {", // Line 3: check if key already existed
                        "    res.first->second++;", // Line 4: increment existing count
                        "}" // Line 5: close if
                    },
                    { // Explanations for each slot
                        "Create sorted map from string keys to int counts.", // Explanation for line 1
                        "Try inserting (\"One\", 1) without overwriting existing keys.", // Explanation for line 2
                        "Check if key already existed (res.second == false).", // Explanation for line 3
                        "Increment the existing value via returned iterator.", // Explanation for line 4
                        "Close if block." // Explanation for line 5
                    },
                    {"freq.insert(\"One\", 1);"} // Distractor passing two separate arguments to insert
                }
            }}
        };

        db.visualizerTopics = { // Populate all C++ visualizer topics and walkthroughs
            {"1. Classes, Objects & Structs", { // Visualizer levels for C++ Topic 1
                { // Begin first level definition
                    "Lab: Class, Accessors & 'this'", // Level title
                    "class Dog {\nprivate:\n    string name;\npublic:\n    Dog(string name) { this->name = name; }\n    string getName() { return name; }\n};\nint main() {\n    Dog d(\"Buddy\");\n    cout << d.getName();\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"private: string name;", "Hides name inside Dog so outside code cannot corrupt it."}, // Step 1
                        {"Dog(string name) { this->name = name; }", "'this->name' refers to the object's field, distinguishing it from parameter 'name'."}, // Step 2
                        {"string getName() { return name; }", "Public getter allows safe read-only access."}, // Step 3
                        {"Dog d(\"Buddy\"); cout << d.getName();", "Creates Dog object d and prints \"Buddy\"."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: Static Members (Maths::PI)", // Level title
                    "class Maths {\npublic:\n    static float PI;\n    static float Mul(Point a, Point b) {\n        return a.x * b.x + a.y * b.y;\n    }\n};\nMaths::PI = 3.14f;\nfloat r = Maths::Mul(p1, p2);", // Complete source code for overview
                    { // Step-by-step explanations
                        {"static float PI;", "Shared by the entire Maths class—no object instance needed!"}, // Step 1
                        {"static float Mul(Point a, Point b)", "Static method cannot access non-static fields or 'this'."}, // Step 2
                        {"Maths::PI = 3.14f; Maths::Mul(p1, p2);", "Accessed directly using the scope operator '::'."} // Step 3
                    }
                }
            }},
            {"2. Constructors, Copy & Destructors", { // Visualizer levels for C++ Topic 2
                { // Begin first level definition
                    "Lab: Shallow vs Deep Copy", // Level title
                    "class MITStudent {\npublic:\n    char *name;\n    MITStudent(const MITStudent &o) {\n        name = _strdup(o.name);\n    }\n};\nMITStudent s1; // s1.name = \"foo\"\nMITStudent s2 = s1;\ns2.name[0] = 'b'; // s1.name stays \"foo\"!", // Complete source code for overview
                    { // Step-by-step explanations
                        {"MITStudent(const MITStudent &o)", "Custom copy constructor runs when writing 'MITStudent s2 = s1;'."}, // Step 1
                        {"name = _strdup(o.name);", "Allocates a brand-new string buffer so s1 and s2 do NOT share one pointer."}, // Step 2
                        {"s2.name[0] = 'b';", "Changes s2 to \"boo\" while s1 safely remains \"foo\"!"} // Step 3
                    }
                },
                { // Begin second level definition
                    "Lab: Destructor Memory Safety", // Level title
                    "class PointBag {\npublic:\n    Point *points;\n    PointBag() { points = new Point[20000]; }\n    ~PointBag() { delete[] points; }\n};\nint main() {\n    { PointBag bag; }\n    // bag's memory is already freed here!\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"PointBag() { points = new Point[20000]; }", "Allocates 20,000 Points on the heap when bag is created."}, // Step 1
                        {"{ PointBag bag; }", "Inner block ends—bag goes out of scope immediately."}, // Step 2
                        {"~PointBag() { delete[] points; }", "Destructor runs automatically and frees all 20,000 Points (no RAM spike!)."} // Step 3
                    }
                }
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Visualizer levels for C++ Topic 3
                { // Begin first level definition
                    "Lab: Heap Allocation & Leak Fix", // Level title
                    "int* getPtrToFive() {\n    int *x = new int(5);\n    return x;\n}\nint main() {\n    for (int i = 0; i < 3; ++i) {\n        int *p = getPtrToFive();\n        cout << *p << endl;\n        delete p;\n    }\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int *x = new int(5);", "Allocates an int on the HEAP so it survives after the function returns."}, // Step 1
                        {"int *p = getPtrToFive();", "Receives the valid heap address inside the loop."}, // Step 2
                        {"cout << *p << endl;", "Dereferences p and prints 5."}, // Step 3
                        {"delete p;", "Frees memory INSIDE the loop so each turn doesn't leak!"} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: Dynamic 2D Array Cleanup", // Level title
                    "int **arr2D = new int*[rows];\nfor (int r = 0; r < rows; r++)\n    arr2D[r] = new int[cols];\n// ... use arr2D[r][c] ...\nfor (int r = 0; r < rows; r++)\n    delete[] arr2D[r];\ndelete[] arr2D;", // Complete source code for overview
                    { // Step-by-step explanations
                        {"int **arr2D = new int*[rows];", "Allocates an array of row pointers on the heap."}, // Step 1
                        {"arr2D[r] = new int[cols];", "Allocates a separate array of columns for each row r."}, // Step 2
                        {"for (int r = 0; r < rows; r++) delete[] arr2D[r];", "Deletes every individual row FIRST—otherwise rows leak!"}, // Step 3
                        {"delete[] arr2D;", "Finally deletes the outer array of row pointers."} // Step 4
                    }
                }
            }},
            {"4. Singly & Doubly Linked Lists", { // Visualizer levels for C++ Topic 4
                { // Begin first level definition
                    "Lab 2 Pt2: Singly LinkedList Insert", // Level title
                    "void InsertAtEnd(Node** headPtr, int val) {\n    Node* n = new Node(val);\n    if (*headPtr == nullptr) { *headPtr = n; return; }\n    Node* cur = *headPtr;\n    while (cur->next != nullptr) cur = cur->next;\n    cur->next = n;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"Node* n = new Node(val);", "Creates a new Node on the heap with next = nullptr."}, // Step 1
                        {"if (*headPtr == nullptr) { *headPtr = n; return; }", "If list is empty, updates main's head pointer via Node**!"}, // Step 2
                        {"while (cur->next != nullptr) cur = cur->next;", "Walks to the LAST node in the chain."}, // Step 3
                        {"cur->next = n;", "Links the last node to our brand-new node n."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab 2 Pt2: Singly LinkedList Delete", // Level title
                    "Node *cur = head, *prev = nullptr;\nwhile (cur != nullptr && cur->data != val) {\n    prev = cur; cur = cur->next;\n}\nif (cur != nullptr) {\n    prev->next = cur->next;\n    delete cur;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"Node *cur = head, *prev = nullptr;", "Tracks current node and the node right behind it."}, // Step 1
                        {"while (cur != nullptr && cur->data != val)", "Walks until target value is found or list ends."}, // Step 2
                        {"prev->next = cur->next;", "Bypasses cur so prev points directly to the node AFTER cur."}, // Step 3
                        {"delete cur;", "Frees cur's heap memory cleanly with zero leak!"} // Step 4
                    }
                },
                { // Begin third level definition
                    "Lab 2 Pt2: DoublyLinked InsertAtPos", // Level title
                    "void InsertAtPosition(int val, int pos) {\n    DoublyLinkedNode* n = new DoublyLinkedNode(val);\n    DoublyLinkedNode* cur = head;\n    for (int i = 0; i < pos - 1 && cur; i++) cur = cur->nextPointer;\n    n->nextPointer = cur->nextPointer;\n    n->prevPointer = cur;\n    if (cur->nextPointer) cur->nextPointer->prevPointer = n;\n    cur->nextPointer = n;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"DoublyLinkedNode* n = new DoublyLinkedNode(val);", "Allocates the new doubly linked node on the heap."}, // Step 1
                        {"for (int i = 0; i < pos - 1 && cur; i++) cur = cur->nextPointer;", "Walks to the node right BEFORE the desired position."}, // Step 2
                        {"n->nextPointer = cur->nextPointer; n->prevPointer = cur;", "Connects the new node's forward and backward links first."}, // Step 3
                        {"if (cur->nextPointer) cur->nextPointer->prevPointer = n;", "Points the NEXT node's backward link back to n."}, // Step 4
                        {"cur->nextPointer = n;", "Points cur's forward link to n. All 4 links are intact!"} // Step 5
                    }
                }
            }},
            {"5. Inheritance & Polymorphism", { // Visualizer levels for C++ Topic 5
                { // Begin first level definition
                    "Lab: Polymorphism & Virtual Dtor", // Level title
                    "class Shape {\npublic:\n    virtual void draw() { cout << \"Shape\"; }\n    virtual ~Shape() {}\n};\nclass Circle : public Shape {\npublic:\n    void draw() override { cout << \"Circle\"; }\n};\nShape *s = new Circle();\ns->draw();\ndelete s;", // Complete source code for overview
                    { // Step-by-step explanations
                        {"virtual void draw()", "Enables dynamic dispatch so derived methods run at runtime."}, // Step 1
                        {"virtual ~Shape() {}", "Ensures ~Circle() runs before ~Shape() when deleting via Shape*."}, // Step 2
                        {"Shape *s = new Circle();", "A Base pointer holds the address of a Derived Circle object."}, // Step 3
                        {"s->draw();", "Calls Circle::draw() dynamically instead of Shape::draw()!"}, // Step 4
                        {"delete s;", "Safely destroys Circle and Shape with no memory leak."} // Step 5
                    }
                },
                { // Begin second level definition
                    "Lab: Constructor & Destructor Order", // Level title
                    "class Base { Base(); virtual ~Base(); };\nclass Member { Member(); ~Member(); };\nclass Derived : public Base {\n    Member m;\n    Derived(); ~Derived();\n};\nint main() { Derived d; }", // Complete source code for overview
                    { // Step-by-step explanations
                        {"Derived d;", "Instantiates d on the stack, triggering construction."}, // Step 1
                        {"1. Base() -> 2. Member() -> 3. Derived()", "Construction always builds Base first, then Members, then Derived."}, // Step 2
                        {"// End of main() scope", "d leaves scope, triggering destruction in EXACT REVERSE order!"}, // Step 3
                        {"4. ~Derived() -> 5. ~Member() -> 6. ~Base()", "Destroys Derived first, then Member m, then Base last."} // Step 4
                    }
                }
            }},
            {"6. Abstract, Interface & Friends", { // Visualizer levels for C++ Topic 6
                { // Begin first level definition
                    "Lab: Abstract Animal Hierarchy", // Level title
                    "class Animal {\npublic:\n    virtual void makeSound() = 0;\n    virtual ~Animal() {}\n};\nclass Dog : public Animal {\npublic:\n    void makeSound() override { cout << \"Woof!\"; }\n};\nAnimal *a = new Dog();\na->makeSound(); delete a;", // Complete source code for overview
                    { // Step-by-step explanations
                        {"virtual void makeSound() = 0;", "Pure virtual function makes Animal abstract (cannot write 'Animal x;')."}, // Step 1
                        {"void makeSound() override", "Dog MUST implement makeSound() to become instantiable."}, // Step 2
                        {"Animal *a = new Dog(); a->makeSound();", "Polymorphically prints \"Woof!\" through an Animal* pointer."} // Step 3
                    }
                },
                { // Begin second level definition
                    "Lab: Friend Class Private Access", // Level title
                    "class SecretData {\nprivate:\n    int secret = 42;\n    friend class BestFriend;\n};\nclass BestFriend {\npublic:\n    void show(SecretData& d) { cout << d.secret; }\n};", // Complete source code for overview
                    { // Step-by-step explanations
                        {"private: int secret = 42;", "Hidden from all normal outside code."}, // Step 1
                        {"friend class BestFriend;", "Grants EVERY method of BestFriend access to SecretData's private fields."}, // Step 2
                        {"cout << d.secret;", "Compiles and prints 42 because BestFriend is a declared friend!"} // Step 3
                    }
                }
            }},
            {"7. Operator Overloading", { // Visualizer levels for C++ Topic 7
                { // Begin first level definition
                    "Lab: Prefix (++c) vs Postfix (c++)", // Level title
                    "Clock Clock::operator++() {\n    tick();\n    return *this;\n}\nClock Clock::operator++(int) {\n    Clock old = *this;\n    tick();\n    return old;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"Clock Clock::operator++()", "No parameters = PREFIX form (++c2)."}, // Step 1
                        {"tick(); return *this;", "Advances clock first, then returns the UPDATED clock."}, // Step 2
                        {"Clock Clock::operator++(int)", "Dummy unnamed 'int' tells compiler this is POSTFIX (c3++)."}, // Step 3
                        {"Clock old = *this; tick(); return old;", "Saves old time, ticks clock, and returns the pre-increment snapshot!"} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: Stream Output (operator<<)", // Level title
                    "ostream& operator<<(ostream& os, Clock& c) {\n    if (c.hour < 10) os << '0';\n    os << c.hour << \":\";\n    if (c.min < 10) os << '0';\n    os << c.min;\n    return os;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"ostream& operator<<(ostream& os, Clock& c)", "Friend non-member function teaching cout how to print a Clock."}, // Step 1
                        {"if (c.hour < 10) os << '0';", "Pads single-digit hours like 3 into \"03\"."}, // Step 2
                        {"return os;", "Returns the stream reference so chaining (cout << c1 << c2) works!"} // Step 3
                    }
                }
            }},
            {"8. Templates (Generic Code)", { // Visualizer levels for C++ Topic 8
                { // Begin first level definition
                    "Lab: Template Function Deduction", // Level title
                    "template <typename T>\nT sum(T const* arr, int n) {\n    T total = 0;\n    for (int i = 0; i < n; i++) total += arr[i];\n    return total;\n}\nint a[] = {1, 2, 3};\ncout << sum(a, 3);", // Complete source code for overview
                    { // Step-by-step explanations
                        {"template <typename T>", "Declares generic type placeholder T."}, // Step 1
                        {"T sum(T const* arr, int n)", "Works for int*, float*, double*, etc."}, // Step 2
                        {"sum(a, 3);", "Compiler deduces T = int from argument 'a' and stamps out sum<int>()!"} // Step 3
                    }
                },
                { // Begin second level definition
                    "Lab: Multiple Template Types", // Level title
                    "template <typename SumType, typename AvgType>\nAvgType average(SumType const* arr, AvgType n) {\n    SumType sum = 0;\n    for (int i = 0; i < n; i++) sum += arr[i];\n    return (AvgType)sum / n;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"template <typename SumType, typename AvgType>", "Accepts two independent generic types separated by a comma."}, // Step 1
                        {"average(intArr, 10)", "Instantiates <int, int> -> integer division."}, // Step 2
                        {"average(intArr, 10.0f)", "Instantiates <int, float> -> returns exact floating-point average!"} // Step 3
                    }
                }
            }},
            {"9. STL Vectors, Iterators & Algos", { // Visualizer levels for C++ Topic 9
                { // Begin first level definition
                    "Lab: Iterators & begin()/end()", // Level title
                    "vector<int> v = {30, 10, 5};\nsort(v.begin(), v.end());\nfor (auto it = v.begin(); it != v.end(); ++it) {\n    cout << *it << endl;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"sort(v.begin(), v.end());", "Sorts everything from begin() up to (but NOT including) end()."}, // Step 1
                        {"auto it = v.begin();", "Iterator starts pointing at the first element (5)."}, // Step 2
                        {"it != v.end(); ++it", "Loops until it reaches one-past-the-end; never dereference end()!"}, // Step 3
                        {"cout << *it << endl;", "Dereferences iterator like a pointer to print 5, 10, 30."} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: Predicate with remove_if()", // Level title
                    "bool isExpired(Particle &p) {\n    return (p.timetolive() == 0);\n}\nv.erase(remove_if(v.begin(), v.end(), isExpired), v.end());", // Complete source code for overview
                    { // Step-by-step explanations
                        {"bool isExpired(Particle &p)", "A predicate is a function returning true/false for each item."}, // Step 1
                        {"remove_if(v.begin(), v.end(), isExpired)", "Moves all alive particles forward and returns iterator to the junk tail."}, // Step 2
                        {"v.erase(..., v.end());", "Actually shrinks the vector by erasing all expired particles at the end."} // Step 3
                    }
                }
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Visualizer levels for C++ Topic 10
                { // Begin first level definition
                    "Lab: Map insert() & shared_ptr", // Level title
                    "map<string, int> freq;\nauto res = freq.insert(make_pair(\"One\", 1));\nif (!res.second) res.first->second++;\nstd::shared_ptr<Library> lib2;\n{\n    auto lib1 = std::make_shared<Library>();\n    lib2 = lib1;\n}", // Complete source code for overview
                    { // Step-by-step explanations
                        {"auto res = freq.insert(make_pair(\"One\", 1));", "Inserts only if \"One\" doesn't exist; never overwrites!"}, // Step 1
                        {"if (!res.second) res.first->second++;", "If key existed (res.second == false), increments its count."}, // Step 2
                        {"auto lib1 = std::make_shared<Library>(); lib2 = lib1;", "Both share ownership; reference count becomes 2."}, // Step 3
                        {"} // inner scope ends", "lib1 dies (ref count = 1). Library stays alive until lib2 dies!"} // Step 4
                    }
                },
                { // Begin second level definition
                    "Lab: STL List splice() (Zero Copy)", // Level title
                    "std::list<int> L1 = {1, 2, 3, 4};\nstd::list<int> L2 = {10, 20, 30};\nauto it = L1.begin(); ++it; // points to 2\nL1.splice(it, L2);\n// L1: 1 10 20 30 2 3 4 | L2: empty!", // Complete source code for overview
                    { // Step-by-step explanations
                        {"auto it = L1.begin(); ++it;", "Positions iterator at element 2 inside L1."}, // Step 1
                        {"L1.splice(it, L2);", "Relinks L2's nodes directly into L1 before 2 with ZERO copying!"}, // Step 2
                        {"// L1: 1 10 20 30 2 3 4", "L2 is now empty, and no objects were constructed or destroyed."} // Step 3
                    }
                }
            }}
        };
    }
    PopulatePracticeAndExam(lang, db.practiceTopics, db.examTopics);
    return db; // Return the populated curriculum database
}

#endif // End of CONTENT_DATA_H header guard