#ifndef CONTENT_DATA_H
#define CONTENT_DATA_H

#include "AppTypes.h"

// Structure holding all learning contents for a specific programming language
struct LanguageDatabase {
    std::vector<TheoryTopic> theoryTopics;
    std::vector<QuizTopic> quizTopics;
    std::vector<ConstructorTopic> constructorTopics;
    std::vector<VisualizerTopic> visualizerTopics;
};

inline LanguageDatabase GetDatabase(TargetLang lang) {
    LanguageDatabase db;

    if (lang == TargetLang::LANG_C) {
        // ====================================================================
        // C LANGUAGE CONTENT (Topic-First across Theory, Quiz, Constructor, Visualizer)
        // ====================================================================
        db.theoryTopics = {
            {"1. What is C? Basics & Keywords", {
                {"What is Programming & C?", "A computer only understands 1s and 0s. A programming language like C lets humans write step-by-step instructions in readable English-like words.\n\nC is a 'compiled' language: a tool called a compiler translates your C code into fast machine code before the program runs."},
                {"Your First C Program", "Every C program starts running inside a special block called main():\n\n#include <stdio.h>\nint main() {\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n- #include <stdio.h> loads input/output tools.\n- Every command ends with a semicolon (;).\n- return 0 tells the computer 'everything worked!'"},
                {"Reserved Words (32 Keywords)", "C reserves 32 special words that you cannot use as your own names, plus the 'sizeof' operator:\n- Data Types (9): int, char, float, double, void, short, long, signed, unsigned\n- Control Flow (11): if, else, switch, case, default, for, while, do, break, continue, return"},
                {"More Reserved Words", "The remaining 12 reserved words control where data lives and how it behaves:\n- Storage (4): auto, register, static, extern\n- Custom Types (4): struct, union, enum, typedef\n- Qualifiers (4): const, volatile, restrict (C99), _Atomic (C11)"},
                {"Variables: Labeled Boxes", "Think of a variable as a labeled storage box in memory. In C, you MUST say what type of data fits in the box before using it:\n- int age = 25;       (whole numbers)\n- float price = 19.99f; (decimals)\n- double pi = 3.14159;  (more precise decimals)\n- char grade = 'A';   (1 character in single quotes)\nWarning: An uninitialized variable holds random leftover garbage!"},
                {"Constants (const)", "If a value should NEVER change while the program runs, lock the box with the keyword 'const':\n\nconst double TAX_RATE = 0.08;\nconst int MAX_SCORE = 100;\n\nTrying to change TAX_RATE later causes a compile error, protecting you from mistakes."}
            }},
            {"2. Input, Output & Math", {
                {"Printing with printf()", "printf() displays text on the screen. To insert variable values into text, use placeholders called Format Specifiers:\n- %d for integers (int)\n- %f for decimals (%.2f shows 2 decimal places)\n- %c for a single character\n- %s for text (strings)\nExample: printf(\"Age: %d\\n\", age); (\\n starts a new line)"},
                {"Reading Input: scanf()", "scanf() reads what the user types on the keyboard:\n\nint age;\nscanf(\"%d\", &age);\n\nNotice the '&' before age! It means 'address of'—it tells scanf the exact memory box where the typed number should be stored.\nTip: Put a space before %c (\" %c\") to skip leftover Enter keys."},
                {"Visual Studio & scanf_s()", "In Visual Studio, standard scanf() may trigger a security warning. Use scanf_s() instead and check if it returned 1 (meaning 1 valid item was read):\n\nif (scanf_s(\"%d\", &age) != 1) {\n    printf(\"Invalid input!\\n\");\n    return 1;\n}"},
                {"Arithmetic & Modulo (%)", "C supports basic math: + (add), - (subtract), * (multiply), / (divide), and % (remainder/modulo).\n\nExample: 10 % 3 is 1 (because 3 fits into 10 three times with 1 left over).\nShortcuts:\n- count++ adds 1; count-- subtracts 1\n- num += 5 means num = num + 5"},
                {"Integer Division vs Casting", "When you divide two integers in C, the decimal part is chopped off (truncated):\nint r = 7 / 2; // Result is 3, NOT 3.5!\n\nTo keep the decimal, convert (cast) one number to float first:\nfloat exact = (float)7 / 2; // Result is 3.50\nCasting float to int also chops off decimals: (int)3.9 becomes 3."}
            }},
            {"3. Decisions & Loops", {
                {"Making Decisions (if / else)", "An 'if' statement lets your program choose what to do based on a true/false question:\n\nif (score >= 90) {\n    printf(\"Grade: A\\n\");\n} else if (score >= 80) {\n    printf(\"Grade: B\\n\");\n} else {\n    printf(\"Keep practicing!\\n\");\n}"},
                {"Logical Operators (&&, ||)", "Combine multiple conditions inside an if statement:\n- && (AND): BOTH sides must be true.\n  if (age >= 18 && age <= 65)\n- || (OR): AT LEAST ONE side must be true.\n  if (age < 18 || age > 65)\n- ! (NOT): flips true to false and false to true."},
                {"The switch Statement", "When checking one variable against many exact values, 'switch' is cleaner than many else-ifs:\n\nswitch(grade) {\n    case 'A': case 'a': printf(\"Excellent!\"); break;\n    default: printf(\"Invalid grade\");\n}\nAlways put 'break;' at the end of a case so it doesn't fall into the next one!"},
                {"while vs do-while Loops", "Loops repeat code automatically.\n- while: checks the condition FIRST. If false, it runs 0 times.\n- do-while: runs the code FIRST, then checks the condition—so it ALWAYS runs at least once!\n\ndo { number--; } while (number != 0);"},
                {"for Loops, break & continue", "A 'for' loop puts start, condition, and step on one line:\nfor (int i = 1; i <= 10; i++) { ... }\n\n- break: immediately stops and exits the loop.\n- continue: skips the rest of THIS turn and jumps to the next turn.\nYou can also put one loop inside another (nested loops) to print grids or triangles!"}
            }},
            {"4. Arrays & 2D Grids", {
                {"What is an Array?", "Instead of creating 50 separate variables, an array stores a row of same-type boxes side-by-side in memory:\n\nint numbers[5] = {10, 20, 30, 40, 50};\n\nIMPORTANT: Computers start counting at 0!\n- numbers[0] is 10 (the 1st item)\n- numbers[4] is 50 (the 5th and last item)"},
                {"Finding Array Length", "If you omit the size, C counts the items for you:\nint values[] = {45, 12, 89, 34};\n\nTo calculate how many items are in an array:\nint size = sizeof(values) / sizeof(values[0]);\n(Total bytes of the whole array divided by bytes of 1 item.)"},
                {"Shifting, Inserting & Rotating", "Arrays have fixed slots, so inserting or deleting requires shifting neighbors:\n- Insert at index: shift elements from the end RIGHT by 1 to open a gap.\n- Delete at index: shift elements after the index LEFT by 1 to close the gap.\n- Rotate Left: save arr[0], shift all left, put saved item at the end."},
                {"2D Arrays (Grids & Matrices)", "A 2D array is like a spreadsheet table with rows and columns:\n\nint matrix[3][4] = {\n    {1, 2, 3, 4},\n    {5, 6, 7, 8},\n    {9, 10, 11, 12}\n};\nAccess with matrix[row][col]. Use a nested for-loop (outer = row, inner = col) to visit every cell."}
            }},
            {"5. C-Strings & Null Terminator", {
                {"What a String Really Is", "C has NO built-in 'string' type! A string in C is simply an array of characters (char) that ends with a special invisible marker called the null terminator: '\\0'.\n\nWhen you write: char name[20] = \"Alice\";\nMemory actually holds: 'A', 'l', 'i', 'c', 'e', '\\0'."},
                {"Why '\\0' Is Critical", "C does NOT store the length of a string anywhere. Functions like printf(\"%s\", name) start at index 0 and print letter by letter until they hit '\\0'.\n\nIf you build a char array manually and forget '\\0', printf keeps reading into random garbage memory!"},
                {"Manual String Operations", "Because '\\0' marks the end, you can find length or count words using a simple loop:\n\nint len = 0;\nwhile (name[len] != '\\0') {\n    len++;\n}\nWhen the loop stops, len equals the exact number of characters."}
            }},
            {"6. Functions & Parameters", {
                {"Why Use Functions?", "A function is a mini-program with a name that performs one specific job—like a recipe you can reuse anytime:\n\nint add(int a, int b) {\n    return a + b;\n}\nUse 'void' as the return type if the function only does an action (like printing) and returns nothing."},
                {"Pass by Value (Photocopy)", "When you pass a normal variable (int, float, char) to a function, C sends a PHOTOCOPY of its value.\n\nvoid tryChange(int x) { x = 999; }\nEven though x becomes 999 inside the function, the original variable in main() is completely untouched!"},
                {"Passing Arrays to Functions", "Arrays are an exception! C does NOT copy arrays—the function works directly on the caller's ORIGINAL array.\n\nvoid doubleAll(int arr[], int size)\nAny change made to arr[i] inside the function permanently changes the original array in main()."},
                {"Pass by Pointer (Changing Originals)", "How can a C function modify a caller's normal int variable? Pass its memory ADDRESS (&x) to a pointer parameter (int *ptr):\n\nvoid setFive(int *ptr) { *ptr = 5; }\n// Call it from main():\nsetFive(&myNum); // myNum is now 5!"}
            }},
            {"7. Pointers & Pointer Math", {
                {"Memory Like Numbered Lockers", "Imagine computer RAM as a hallway of numbered lockers. Every variable lives in a locker with a unique address (like 1000, 1004, 1008).\n\nA POINTER is simply a variable that stores a locker NUMBER (memory address) instead of the actual data."},
                {"The Twin Operators: & and *", "Two symbols control pointers:\n1. & (Address-of): asks 'what locker number is this variable in?'\n   int *ptr = &i;\n2. * (Dereference): 'open the locker at this address and look inside!'\n   *ptr = 10; // Changes i to 10!"},
                {"Double Pointers (int**)", "Since a pointer is also a variable in a locker, another pointer can store ITS address!\n\nint *ptr = &i;\nint **pptr = &ptr;\n- *pptr gives you ptr (address of i).\n- **pptr opens two doors and gives you the value of i!"},
                {"Pointer Arithmetic (No arr[i]!)", "Pointers can walk through arrays without using brackets arr[i]:\n- ptr + 1 jumps to the NEXT element (auto-adjusting by sizeof(type)!).\n- ptr - 1 jumps to the PREVIOUS element.\n- *(arr + i) is identical to arr[i].\nInvalid: You CANNOT multiply (ptr * 2) or divide (ptr / 2) pointers!"},
                {"Null Pointers (Safety)", "An uninitialized pointer points to a random dangerous address! Always set empty pointers to NULL (or nullptr in C++):\n\nint *ptr = NULL;\nif (ptr == NULL) { /* Safe: don't dereference! */ }"}
            }},
            {"8. Heap Memory & Func Pointers", {
                {"Stack vs Heap Memory", "Normal variables live on the STACK and are automatically erased when their function ends.\n\nThe HEAP is a big shared memory pool where YOU decide when memory is created and when it is freed—perfect for data whose size is only known at runtime."},
                {"malloc() and free() in C", "Include <stdlib.h> to request heap memory:\n\nint *ptr = (int*)malloc(sizeof(int));\nif (ptr == NULL) return 1; // Always check!\n*ptr = 42;\nfree(ptr); // Return memory when done!\nptr = NULL; // Good safety habit"},
                {"Dynamic Arrays & Leaks", "To allocate an array of 'n' ints on the heap:\nint *arr = (int*)malloc(n * sizeof(int));\n\nRules:\n- Always free() every malloc() or you cause a MEMORY LEAK.\n- Never use memory after calling free()!"},
                {"Global Shadowing & Function Ptrs", "1. Global variables are visible everywhere, but a local variable with the SAME name hides (shadows) the global one inside that function.\n2. Function Pointers store a function's address so you can call it indirectly:\nint (*fn)(int, int) = Add;\nint ans = fn(20, 10); // No arithmetic allowed on fn!"}
            }},
            {"9. Linux fork/pipe & Bit Ops", {
                {"Linux fork(): Cloning a Process", "fork() (<unistd.h>) clones your running program into two separate processes: Parent and Child.\nCheck its return value to know who you are:\n- -1: Error (failed)\n-  0: You are the CHILD\n- >0: You are the PARENT (value is Child's PID)\nCalling fork() 3 times creates 2^3 = 8 processes!"},
                {"Linux pipe(): Process Tunnel", "A pipe is a one-way water pipe between processes:\nint fd[2]; pipe(fd);\n- fd[0] = READ end, fd[1] = WRITE end.\nCall pipe() BEFORE fork(). Always close() unused ends—if the write end stays open, the reader waits forever! For two-way chat, use TWO pipes."},
                {"Bitwise Operators (&, |, ^, ~)", "Bit manipulation edits individual 1s and 0s directly!\nFor A = 60 (0011 1100) and B = 13 (0000 1101):\n- A & B (AND): 1 only if BOTH are 1 -> 0000 1100 (12)\n- A | B (OR):  1 if EITHER is 1 -> 0011 1101 (61)\n- A ^ B (XOR): 1 if ONLY ONE is 1 -> 0011 0001 (49)\n- ~A (NOT):    flips every bit -> 1100 0011"},
                {"Bit Shifts & Struct Bit Fields", "Shifts move bits left/right (filling gaps with 0):\n- x << 1 is a super-fast multiply by 2.\n- x >> 1 is a super-fast divide by 2.\nBit Fields pack tiny flags tightly into structs using ': bits':\nstruct packed { unsigned int flag : 1; unsigned int type : 4; };"}
            }}
        };

        db.quizTopics = {
            {"1. What is C? Basics & Keywords", {
                {"Where does every C program start executing?", {"start()", "main()", "begin()", "printf()"}, 1},
                {"What does 'return 0;' at the end of main() tell the computer?", {"There was an error", "The program ran successfully", "Print the number 0", "Restart the program"}, 1},
                {"How many reserved keywords does standard C have (excluding sizeof)?", {"16", "24", "32", "50"}, 2},
                {"What happens if you declare 'int x;' without giving it a starting value?", {"It is always 0", "It holds random leftover memory (garbage)", "It causes a compile error", "It becomes -1"}, 1},
                {"Which keyword locks a variable so its value can never be changed?", {"static", "lock", "const", "fixed"}, 2}
            }},
            {"2. Input, Output & Math", {
                {"Which printf() format specifier prints a whole number (int)?", {"%f", "%c", "%d", "%s"}, 2},
                {"Why do we write '&age' in scanf(\"%d\", &age)?", {"To multiply age", "To give scanf the memory address of age", "To check if age is positive", "To print age"}, 1},
                {"What is the result of integer division (7 / 2) in C?", {"3.5", "4", "3", "0"}, 2},
                {"What does the modulo operator (10 % 3) return?", {"3", "1", "0.33", "30"}, 1},
                {"How do you force 7 / 2 to give 3.5 instead of 3?", {"7 // 2", "(float)7 / 2", "int(7 / 2)", "7 % 2"}, 1}
            }},
            {"3. Decisions & Loops", {
                {"Which logical operator means BOTH conditions must be true?", {"||", "&&", "==", "!="}, 1},
                {"Why do we put 'break;' at the end of each case in a switch statement?", {"To exit the whole program", "To stop execution from falling into the next case", "To restart the switch", "It is optional decoration"}, 1},
                {"What is the key difference between a 'while' loop and a 'do-while' loop?", {"do-while is faster", "do-while always executes its body at least once", "while cannot use integers", "do-while never stops"}, 1},
                {"What does the 'continue;' statement do inside a loop?", {"Exits the loop completely", "Skips the rest of the current turn and starts the next turn", "Pauses for 1 second", "Ends main()"}, 1}
            }},
            {"4. Arrays & 2D Grids", {
                {"In 'int arr[5] = {10, 20, 30, 40, 50};', what is the index of 10?", {"1", "0", "5", "-1"}, 1},
                {"How do you calculate the number of elements in a C array 'values'?", {"values.length()", "sizeof(values)", "sizeof(values) / sizeof(values[0])", "count(values)"}, 2},
                {"To delete an element from the middle of an array, which way do you shift the remaining items?", {"Shift them LEFT to close the gap", "Shift them RIGHT", "Reverse the array", "Multiply by 2"}, 0},
                {"In a 2D array 'matrix[3][4]', what do 3 and 4 represent?", {"3 columns and 4 rows", "3 rows and 4 columns", "Values 3 and 4", "3 arrays of 4 bytes"}, 1}
            }},
            {"5. C-Strings & Null Terminator", {
                {"How are strings represented in the C language?", {"A built-in String class", "A char array ending with '\\0'", "An int array", "A float pointer"}, 1},
                {"What happens if you forget '\\0' at the end of a manual char array and call printf(\"%s\")?", {"C adds it at runtime", "printf keeps reading into random memory", "Nothing prints", "The compiler fixes it"}, 1},
                {"How does a manual loop find the length of a C-string?", {"Counts until it hits a space", "Counts while str[len] != '\\0'", "Reads str[-1]", "Checks str.size()"}, 1},
                {"How many array slots does \"Alice\" actually occupy in memory (including the null terminator)?", {"5", "6", "4", "10"}, 1}
            }},
            {"6. Functions & Parameters", {
                {"What return type do you use for a function that does not return any value?", {"null", "empty", "void", "int"}, 2},
                {"In C, when you pass an ordinary int to a function (pass by value), what does the function get?", {"The original variable", "A photocopy (copy) of the value", "A memory address", "Nothing"}, 1},
                {"When you pass an array to a C function and modify arr[0], what happens in main()?", {"The original array in main() is modified too", "Nothing changes in main()", "The program crashes", "It creates a second array"}, 0},
                {"How can a C function modify a caller's ordinary int variable?", {"Return void", "Pass its address (&x) to a pointer parameter (int *p)", "Use const int", "Use printf"}, 1}
            }},
            {"7. Pointers & Pointer Math", {
                {"What does a pointer variable actually store?", {"A copy of the value", "The memory address of another variable", "The name of the variable", "Always 0"}, 1},
                {"Which operator dereferences a pointer to access or change the value it points to?", {"&", "*", "%", "->"}, 1},
                {"If 'int *p = &i;' and 'int **pp = &p;', what does '**pp = 7;' do?", {"Changes p to 7", "Changes i to 7", "Changes the address of pp", "Causes a syntax error"}, 1},
                {"Which pointer arithmetic expression is identical to arr[2]?", {"*(arr + 2)", "*arr + 2", "&arr + 2", "arr * 2"}, 0},
                {"Which operation is INVALID on pointers in C?", {"ptr++", "ptr + 3", "ptr * 2", "ptr - 1"}, 2}
            }},
            {"8. Heap Memory & Func Pointers", {
                {"Which function allocates memory on the heap in C?", {"new", "malloc()", "alloc_heap()", "create()"}, 1},
                {"What should you ALWAYS check immediately after calling malloc()?", {"If ptr == NULL", "If ptr == 0xFF", "If free() ran", "If sizeof > 0"}, 0},
                {"What happens if you allocate memory with malloc() and never call free()?", {"It frees automatically in 1 second", "A memory leak occurs", "The CPU runs faster", "Compile error"}, 1},
                {"If a function declares a local variable with the same name as a global variable, what happens?", {"Compile error", "The local variable shadows the global one inside that function", "The global variable is deleted", "Both change together"}, 1},
                {"Which operation is NOT allowed on a function pointer?", {"Assigning a function address", "Calling the function through it", "Pointer arithmetic (fn + 1)", "Passing it to another function"}, 2}
            }},
            {"9. Linux fork/pipe & Bit Ops", {
                {"What value does fork() return inside the CHILD process?", {"-1", "0", "Child's PID (>0)", "Parent's PID"}, 1},
                {"If a program calls fork() 3 times in a row, how many total processes exist?", {"3", "6", "8", "9"}, 2},
                {"Why must processes close unused pipe ends after fork()?", {"To save disk space", "So the reader doesn't wait forever for more data", "Because pipes only work in C++", "To double the speed"}, 1},
                {"Given A = 0011 1100 (60) and B = 0000 1101 (13), what is A & B (AND)?", {"0000 1100 (12)", "0011 1101 (61)", "0011 0001 (49)", "1100 0011"}, 0},
                {"Shifting an unsigned integer left by 1 (x << 1) is a fast equivalent to:", {"Adding 1", "Multiplying by 2", "Dividing by 2", "Flipping all bits"}, 1}
            }}
        };

        db.constructorTopics = {
            {"1. What is C? Basics & Keywords", {
                {
                    "Lab: Your First C Program",
                    {
                        "#include <stdio.h>",
                        "int main() {",
                        "    printf(\"Hello, World!\\n\");",
                        "    return 0;",
                        "}"
                    },
                    {
                        "Include <stdio.h> so printf() is available.",
                        "Every C program starts execution inside main().",
                        "Print Hello, World! followed by a newline.",
                        "Return 0 to tell the OS everything succeeded.",
                        "Close the main() function block."
                    },
                    {"cout << \"Hello\";"}
                },
                {
                    "Lab: Variables & const Lock",
                    {
                        "const int MAX_SCORE = 100;",
                        "int age = 25;",
                        "float price = 19.99f;",
                        "printf(\"Age: %d\\n\", age);",
                        "return 0;"
                    },
                    {
                        "Lock MAX_SCORE as a constant so it cannot change.",
                        "Declare an integer variable age set to 25.",
                        "Declare a float variable price with 'f' suffix.",
                        "Print age using the %d integer format specifier.",
                        "Exit main() with status 0."
                    },
                    {"MAX_SCORE = 200;"}
                }
            }},
            {"2. Input, Output & Math", {
                {
                    "Lab: Safe Casting & Division",
                    {
                        "int a = 7, b = 2;",
                        "float exact = (float)a / b;",
                        "printf(\"Result: %.2f\\n\", exact);",
                        "return 0;"
                    },
                    {
                        "Declare two integer variables a and b.",
                        "Cast 'a' to float so 7/2 gives 3.5 instead of 3.",
                        "Print the float formatted to 2 decimal places.",
                        "Return 0 at the end of main."
                    },
                    {"float exact = a / b;"}
                },
                {
                    "Lab 1 Q1: Leap Year Check",
                    {
                        "bool isLeapYear(int year) {",
                        "    if (year % 400 == 0) return true;",
                        "    if (year % 100 == 0) return false;",
                        "    if (year % 4 == 0) return true;",
                        "    return false;",
                        "}"
                    },
                    {
                        "Define isLeapYear taking an integer year.",
                        "Years divisible by 400 (e.g., 1600) are leap years.",
                        "Other centuries divisible by 100 (e.g., 1700) are NOT.",
                        "Remaining years divisible by 4 (e.g., 2016) are leap years.",
                        "All other years return false.",
                        "Close the function."
                    },
                    {"if (year % 2 == 0) return true;"}
                }
            }},
            {"3. Decisions & Loops", {
                {
                    "Lab 1 Q2: Reverse Number Digits",
                    {
                        "int rev = 0;",
                        "while (n > 0) {",
                        "    int digit = n % 10;",
                        "    rev = rev * 10 + digit;",
                        "    n = n / 10;",
                        "}"
                    },
                    {
                        "Initialize reversed accumulator rev to 0.",
                        "Loop while digits remain in n.",
                        "Extract the last digit using modulo 10.",
                        "Shift rev left by 10 and add the extracted digit.",
                        "Remove the last digit via integer division by 10.",
                        "Close the while loop."
                    },
                    {"n = n % 10;"}
                },
                {
                    "Lab 1 Q3: Prime Number Check",
                    {
                        "if (n <= 1) return false;",
                        "for (int i = 2; i * i <= n; i++) {",
                        "    if (n % i == 0) {",
                        "        return false;",
                        "    }",
                        "} return true;"
                    },
                    {
                        "Numbers <= 1 are never prime.",
                        "Check candidate divisors up to sqrt(n).",
                        "Test if i divides n with zero remainder.",
                        "Found a factor, so n is NOT prime.",
                        "Close the if block.",
                        "No factors found: close loop and return true."
                    },
                    {"if (n % 2 == 0) return true;"}
                },
                {
                    "Lab 1 Q6: Right-Angled Triangle",
                    {
                        "for (int row = 1; row <= 4; row++) {",
                        "    for (int col = 1; col <= row; col++) {",
                        "        printf(\"A\");",
                        "    }",
                        "    printf(\"\\n\");",
                        "}"
                    },
                    {
                        "Outer loop iterates over rows 1 through 4.",
                        "Inner loop prints 'row' A's on the current row.",
                        "Print one 'A' character.",
                        "Close inner column loop.",
                        "Print newline after finishing the row.",
                        "Close outer row loop."
                    },
                    {"for (int col = 1; col <= 4; col++) {"}
                }
            }},
            {"4. Arrays & 2D Grids", {
                {
                    "Lab 1 Q8: Find Element in Array",
                    {
                        "int find(int size, int arr[], int target) {",
                        "    for (int i = 0; i < size; i++) {",
                        "        if (arr[i] == target) return i;",
                        "    }",
                        "    return -1;",
                        "}"
                    },
                    {
                        "Define find() with size, array, and target value.",
                        "Scan indices 0 through size - 1.",
                        "If element matches target, return its index i.",
                        "Close the for loop.",
                        "Return -1 if target was not found anywhere.",
                        "Close the function."
                    },
                    {"return 0; // not found"}
                },
                {
                    "Lab 1 Q15: Reverse Array In-Place",
                    {
                        "for (int i = 0; i < size / 2; i++) {",
                        "    int temp = arr[i];",
                        "    arr[i] = arr[size - 1 - i];",
                        "    arr[size - 1 - i] = temp;",
                        "}"
                    },
                    {
                        "Stop at size / 2 so you don't swap elements back!",
                        "Save the left element in temp.",
                        "Move the mirror right element to the left slot.",
                        "Place saved temp into the right slot.",
                        "Close the loop."
                    },
                    {"for (int i = 0; i < size; i++) {"}
                },
                {
                    "Lab 1 Q16: Rotate Array Left",
                    {
                        "int first = arr[0];",
                        "for (int i = 0; i < size - 1; i++) {",
                        "    arr[i] = arr[i + 1];",
                        "}",
                        "arr[size - 1] = first;"
                    },
                    {
                        "Save arr[0] before shifting overwrites it.",
                        "Loop from 0 up to the second-to-last index.",
                        "Shift each neighbor one slot to the left.",
                        "Close the for loop.",
                        "Place the saved first element at the back."
                    },
                    {"arr[0] = arr[size - 1];"}
                }
            }},
            {"5. C-Strings & Null Terminator", {
                {
                    "Lab: Manual String Length ('\\0')",
                    {
                        "char name[] = \"Alice\";",
                        "int len = 0;",
                        "while (name[len] != '\\0') {",
                        "    len++;",
                        "}",
                        "printf(\"Length: %d\\n\", len);"
                    },
                    {
                        "Declare null-terminated C-string \"Alice\".",
                        "Start character counter len at 0.",
                        "Loop until the null terminator '\\0' is reached.",
                        "Increment len for each character.",
                        "Close the while loop.",
                        "Print the measured length."
                    },
                    {"while (name[len] != '0') {"}
                },
                {
                    "Lab 2 Q5: strlen_ptr (Pointers Only)",
                    {
                        "int strlen_ptr(char *str) {",
                        "    int len = 0;",
                        "    while (*str != '\\0') {",
                        "        len++;",
                        "        str++;",
                        "    } return len; }"
                    },
                    {
                        "Take a pointer to the first char of the string.",
                        "Start length counter at 0.",
                        "Check dereferenced char against '\\0'.",
                        "Increment length counter.",
                        "Move pointer forward to the next char.",
                        "Close loop, return len, and close function."
                    },
                    {"while (str[len] != '\\0') {"}
                }
            }},
            {"6. Functions & Parameters", {
                {
                    "Lab 2 Q1: Swap Ints via Pointers",
                    {
                        "void swap_ints(int *a, int *b) {",
                        "    int temp = *a;",
                        "    *a = *b;",
                        "    *b = temp;",
                        "}"
                    },
                    {
                        "Accept two int pointers holding callers' addresses.",
                        "Dereference 'a' to save first value in temp.",
                        "Write value at 'b' into memory pointed to by 'a'.",
                        "Write temp into memory pointed to by 'b'.",
                        "Close swap_ints."
                    },
                    {"int *temp = a;"}
                },
                {
                    "Lab: Modify Array in doubleAll()",
                    {
                        "void doubleAll(int arr[], int size) {",
                        "    for (int i = 0; i < size; i++) {",
                        "        arr[i] = arr[i] * 2;",
                        "    }",
                        "}"
                    },
                    {
                        "Arrays pass directly to functions without copying.",
                        "Loop across every index of the array.",
                        "Double each element directly in caller's memory.",
                        "Close the for loop.",
                        "Close the void function."
                    },
                    {"return arr;"}
                }
            }},
            {"7. Pointers & Pointer Math", {
                {
                    "Lab: Double Pointer Indirection",
                    {
                        "int i = 5;",
                        "int *ptr = &i;",
                        "int **pptr = &ptr;",
                        "**pptr = 12;",
                        "printf(\"%d\\n\", i);"
                    },
                    {
                        "Declare integer i initialized to 5.",
                        "Store address of i in pointer ptr.",
                        "Store address of ptr in double pointer pptr.",
                        "Dereference twice to change i to 12.",
                        "Print updated i (12)."
                    },
                    {"int **pptr = &i;"}
                },
                {
                    "Lab 2 Q3: find_max (Pointers Only)",
                    {
                        "int maxVal = *arr;",
                        "for (int i = 1; i < size; i++) {",
                        "    if (*(arr + i) > maxVal) {",
                        "        maxVal = *(arr + i);",
                        "    }",
                        "} return maxVal;"
                    },
                    {
                        "Read the 1st element via *arr as initial max.",
                        "Check elements from offset 1 to size - 1.",
                        "Use pointer math *(arr + i) instead of arr[i].",
                        "Update maxVal when a bigger number is found.",
                        "Close if block.",
                        "Close loop and return maxVal."
                    },
                    {"if (*arr + i > maxVal) {"}
                }
            }},
            {"8. Heap Memory & Func Pointers", {
                {
                    "Lab: Dynamic Array with malloc()",
                    {
                        "int *arr = (int*)malloc(5 * sizeof(int));",
                        "if (arr == NULL) { return 1; }",
                        "arr[0] = 42;",
                        "free(arr);",
                        "arr = NULL;"
                    },
                    {
                        "Allocate heap memory for 5 ints using malloc().",
                        "Always check if malloc() returned NULL.",
                        "Store a value inside the heap array.",
                        "Free the heap memory to prevent leaks.",
                        "Set pointer to NULL after freeing."
                    },
                    {"delete[] arr;"}
                },
                {
                    "Lab: Calling a Function Pointer",
                    {
                        "int (*funcPtr)(int, int);",
                        "funcPtr = Add;",
                        "int result = funcPtr(20, 10);",
                        "printf(\"Sum: %d\\n\", result);"
                    },
                    {
                        "Declare a function pointer matching int(int, int).",
                        "Assign the address of function Add to funcPtr.",
                        "Call Add indirectly through funcPtr.",
                        "Print the returned sum (30)."
                    },
                    {"funcPtr = Add(20, 10);"}
                }
            }},
            {"9. Linux fork/pipe & Bit Ops", {
                {
                    "Linux Lab: fork() & Pipe Read",
                    {
                        "int fd[2];",
                        "pipe(fd);",
                        "if (fork() == 0) {",
                        "    close(fd[1]);",
                        "    read(fd[0], buf, sizeof(buf));",
                        "    close(fd[0]); }"
                    },
                    {
                        "Declare 2-int array for read (0) & write (1) ends.",
                        "Create the pipe BEFORE calling fork().",
                        "Fork and enter child branch (pid == 0).",
                        "Close unused write end fd[1] so read won't hang.",
                        "Read incoming bytes from parent into buf.",
                        "Close read end and close child block."
                    },
                    {"close(fd[0]); // before read"}
                },
                {
                    "Bit Lab: Packed Struct Bit Fields",
                    {
                        "struct packed_struct {",
                        "    unsigned int f1 : 1;",
                        "    unsigned int f2 : 1;",
                        "    unsigned int type : 4;",
                        "    unsigned int my_int : 9;",
                        "};"
                    },
                    {
                        "Begin struct definition.",
                        "Pack 1-bit flag f1.",
                        "Pack 1-bit flag f2.",
                        "Pack 4-bit type field.",
                        "Pack 9-bit integer field.",
                        "Close struct with semicolon."
                    },
                    {"unsigned int type[4];"}
                }
            }}
        };

        db.visualizerTopics = {
            {"1. What is C? Basics & Keywords", {
                {
                    "Lab: Variables & Format Specifiers",
                    "#include <stdio.h>\nint main() {\n    const double TAX = 0.08;\n    int age = 25;\n    float price = 19.99f;\n    printf(\"Age: %d, Price: %.2f\\n\", age, price);\n    return 0;\n}",
                    {
                        {"#include <stdio.h>", "Loads Standard I/O library so we can use printf()."},
                        {"const double TAX = 0.08;", "Creates a locked decimal constant that cannot be modified later."},
                        {"int age = 25;", "Creates an integer box named 'age' holding 25."},
                        {"float price = 19.99f;", "Creates a single-precision decimal variable."},
                        {"printf(\"Age: %d, Price: %.2f\\n\", age, price);", "Replaces %d with 25 and %.2f with 19.99 (2 decimal places)."},
                        {"return 0;", "Signals to the operating system that main() finished with zero errors."}
                    }
                }
            }},
            {"2. Input, Output & Math", {
                {
                    "Lab 1 Q2: Palindrome Number Check",
                    "bool isAPalindrome(int n) {\n    int orig = n, rev = 0;\n    while (n > 0) {\n        rev = rev * 10 + (n % 10);\n        n /= 10;\n    }\n    return (orig == rev);\n}",
                    {
                        {"int orig = n, rev = 0;", "Saves the original number (e.g., 121) because n will shrink to 0."},
                        {"while (n > 0) {", "Repeats as long as digits remain in n."},
                        {"    rev = rev * 10 + (n % 10);", "Peels off the last digit (n % 10) and appends it to rev."},
                        {"    n /= 10;", "Integer division drops the last digit (121 -> 12 -> 1 -> 0)."},
                        {"return (orig == rev);", "Compares original (121) with reversed (121). True = Palindrome!"}
                    }
                },
                {
                    "Lab 1 Q4: Binary to Decimal",
                    "int convertBinarytoDecimal(int bin) {\n    int dec = 0, base = 1;\n    while (bin > 0) {\n        int last = bin % 10;\n        dec += last * base;\n        base *= 2;\n        bin /= 10;\n    }\n    return dec;\n}",
                    {
                        {"int dec = 0, base = 1;", "Starts decimal total at 0 and place value (2^0) at 1."},
                        {"while (bin > 0) {", "Processes binary digits (e.g., 110) right-to-left."},
                        {"    int last = bin % 10;", "Grabs the rightmost digit (0 or 1) without bit shifts."},
                        {"    dec += last * base;", "Adds digit * place value (1, 2, 4, 8...) to dec."},
                        {"    base *= 2; bin /= 10;", "Doubles base to next power of 2 and drops rightmost digit."},
                        {"return dec;", "For 110, returns 0*1 + 1*2 + 1*4 = 6!"}
                    }
                }
            }},
            {"3. Decisions & Loops", {
                {
                    "Lab 1 Q7: Isosceles Triangle Pattern",
                    "void drawIsocelesTriangle() {\n    for (int row = 1; row <= 7; row++) {\n        int count = (row <= 4) ? row : (8 - row);\n        for (int c = 0; c < count; c++) printf(\"A\");\n        printf(\"\\n\");\n    }\n}",
                    {
                        {"for (int row = 1; row <= 7; row++)", "Runs 7 rows total: growing 1..4 then shrinking 3..1."},
                        {"int count = (row <= 4) ? row : (8 - row);", "If row <= 4, prints 'row' A's; otherwise mirrors back down (3, 2, 1)."},
                        {"for (int c = 0; c < count; c++) printf(\"A\");", "Prints the exact number of A's for this row."},
                        {"printf(\"\\n\");", "Moves cursor to the next line."}
                    }
                },
                {
                    "Lab: switch Grade Menu",
                    "switch (grade) {\n    case 'A':\n    case 'a':\n        printf(\"Excellent!\\n\");\n        break;\n    default:\n        printf(\"Invalid grade\\n\");\n}",
                    {
                        {"switch (grade)", "Compares grade against each constant case."},
                        {"case 'A': case 'a':", "Stacked cases handle both uppercase and lowercase 'A' together."},
                        {"printf(\"Excellent!\\n\");", "Runs when grade matches 'A' or 'a'."},
                        {"break;", "Exits the switch immediately so it doesn't fall into default!"},
                        {"default:", "Fallback branch if grade didn't match any case."}
                    }
                }
            }},
            {"4. Arrays & 2D Grids", {
                {
                    "Lab 1 Q9: Find 2nd Largest in Array",
                    "int find2ndLargest(int size, int arr[]) {\n    int max1 = -1, max2 = -1;\n    for (int i = 0; i < size; i++) {\n        if (arr[i] > max1) {\n            max2 = max1; max1 = arr[i];\n        } else if (arr[i] > max2 && arr[i] != max1) {\n            max2 = arr[i];\n        }\n    }\n    return max2;\n}",
                    {
                        {"int max1 = -1, max2 = -1;", "Tracks 1st and 2nd largest values (-1 means none found yet)."},
                        {"if (arr[i] > max1)", "Found a new overall maximum!"},
                        {"max2 = max1; max1 = arr[i];", "Demotes old max1 to max2, then saves new max1."},
                        {"else if (arr[i] > max2 && arr[i] != max1)", "Strictly between max2 and max1 (ignores duplicates of max1)."},
                        {"return max2;", "Returns the 2nd largest value, or -1 if none exists."}
                    }
                },
                {
                    "Lab 1 Q11: Insert Element & Shift",
                    "bool insertElement(int size, int *count, int arr[], int val, int idx) {\n    if (*count >= size || idx < 0 || idx > *count) return false;\n    for (int i = *count; i > idx; i--) {\n        arr[i] = arr[i - 1];\n    }\n    arr[idx] = val;\n    (*count)++;\n    return true;\n}",
                    {
                        {"if (*count >= size || idx < 0 || idx > *count)", "Safety check: ensures array isn't full and index is valid."},
                        {"for (int i = *count; i > idx; i--)", "Walks backwards from the end toward idx."},
                        {"arr[i] = arr[i - 1];", "Shifts elements 1 slot RIGHT to open an empty space at idx."},
                        {"arr[idx] = val; (*count)++;", "Places val into the gap and increments count."},
                        {"return true;", "Confirms the insertion succeeded."}
                    }
                },
                {
                    "Lab 1 Q17: Two Movies Flight Match",
                    "bool twoMovies(int flight, int movies[], int size) {\n    for (int i = 0; i < size; i++) {\n        for (int j = i + 1; j < size; j++) {\n            if (movies[i] + movies[j] == flight) return true;\n        }\n    }\n    return false;\n}",
                    {
                        {"for (int i = 0; i < size; i++)", "Picks the first movie at index i."},
                        {"for (int j = i + 1; j < size; j++)", "Starts j at i + 1 so the same movie is NEVER watched twice!"},
                        {"if (movies[i] + movies[j] == flight)", "Checks if the pair's runtimes equal the exact flight length."},
                        {"return true; / return false;", "Returns true on first match, or false if no pair works."}
                    }
                }
            }},
            {"5. C-Strings & Null Terminator", {
                {
                    "Lab 1 Q18: Count Words in Char Array",
                    "int wordCounter(int size, char chars[]) {\n    int words = 0; bool inWord = false;\n    for (int i = 0; i < size; i++) {\n        bool isLetter = (chars[i]>='A'&&chars[i]<='Z') || (chars[i]>='a'&&chars[i]<='z');\n        if (isLetter && !inWord) { words++; inWord = true; }\n        else if (!isLetter) { inWord = false; }\n    }\n    return words;\n}",
                    {
                        {"int words = 0; bool inWord = false;", "Tracks total words and whether we are currently inside a word."},
                        {"bool isLetter = (A..Z) || (a..z);", "Tests if the current char is an alphabetic letter."},
                        {"if (isLetter && !inWord) { words++; inWord = true; }", "Just stepped onto the 1st letter of a new word—count it!"},
                        {"else if (!isLetter) inWord = false;", "Hit a space: resets flag so the next letter starts a new word."},
                        {"return words;", "Returns the exact number of separate words."}
                    }
                },
                {
                    "Lab 2 Q7: copy_string (Pointers Only)",
                    "void copy_string(char *dest, char *src) {\n    while (*src != '\\0') {\n        *dest = *src;\n        dest++; src++;\n    }\n    *dest = '\\0';\n}",
                    {
                        {"while (*src != '\\0')", "Loops until src reaches the null terminator."},
                        {"*dest = *src;", "Copies one character from src into dest."},
                        {"dest++; src++;", "Advances both pointers to the next character slot."},
                        {"*dest = '\\0';", "CRITICAL: Writes '\\0' at the end so dest is a valid C-string!"}
                    }
                },
                {
                    "Lab 2 Q10: compare_strings (Pointers)",
                    "int compare_strings(char *s1, char *s2) {\n    while (*s1 != '\\0' && *s1 == *s2) {\n        s1++; s2++;\n    }\n    return (*s1 - *s2);\n}",
                    {
                        {"while (*s1 != '\\0' && *s1 == *s2)", "Walks forward while letters match and string hasn't ended."},
                        {"s1++; s2++;", "Advances both pointers to compare the next pair of characters."},
                        {"return (*s1 - *s2);", "Returns 0 if equal, <0 if s1 < s2 (\"apple\" < \"banana\"), >0 if s1 > s2."}
                    }
                }
            }},
            {"6. Functions & Parameters", {
                {
                    "Lab: Pass by Value vs Pass by Pointer",
                    "void byVal(int j) { j = 5; }\nvoid byPtr(int *p) { *p = 5; }\nint main() {\n    int i = 3;\n    byVal(i);\n    byPtr(&i);\n    return 0;\n}",
                    {
                        {"int i = 3;", "Creates variable i on the stack holding 3."},
                        {"byVal(i);", "Passes a COPY of 3 to j. Setting j = 5 leaves i still 3!"},
                        {"byPtr(&i);", "Passes the memory ADDRESS of i into pointer p."},
                        {"*p = 5;", "Dereferences p and changes the original variable i in main() to 5!"}
                    }
                },
                {
                    "Lab 2 Q2: swap_chars (Pointers)",
                    "void swap_chars(char *a, char *b) {\n    char temp = *a;\n    *a = *b;\n    *b = temp;\n}\nint main() {\n    char c1 = 'A', c2 = 'B';\n    swap_chars(&c1, &c2);\n}",
                    {
                        {"char c1 = 'A', c2 = 'B';", "Creates two character variables in main()."},
                        {"swap_chars(&c1, &c2);", "Passes addresses of c1 and c2 into char pointers a and b."},
                        {"char temp = *a; *a = *b; *b = temp;", "Exchanges the characters in memory: c1 becomes 'B', c2 becomes 'A'!"}
                    }
                }
            }},
            {"7. Pointers & Pointer Math", {
                {
                    "Lab: Pointer Arithmetic on Arrays",
                    "float a[4];\nfloat *ptr = &(a[2]);\n*ptr = 3.14f;\nptr++;\n*ptr = 9.0f;\nptr = ptr - 3;\n*ptr = 6.0f;",
                    {
                        {"float *ptr = &(a[2]);", "Points ptr directly at element a[2] (same as a + 2)."},
                        {"*ptr = 3.14f;", "Writes 3.14 into a[2]."},
                        {"ptr++; *ptr = 9.0f;", "Advances ptr by sizeof(float) to a[3] and sets it to 9.0."},
                        {"ptr = ptr - 3; *ptr = 6.0f;", "Steps back 3 elements to a[0] and sets it to 6.0."}
                    }
                },
                {
                    "Lab 2 Q3: find_max Walkthrough",
                    "int find_max(int *arr, int size) {\n    int maxVal = *arr;\n    for (int i = 1; i < size; i++) {\n        if (*(arr + i) > maxVal) maxVal = *(arr + i);\n    }\n    return maxVal;\n}",
                    {
                        {"int maxVal = *arr;", "Dereferences arr to grab the 1st element (arr[0]) as initial max."},
                        {"for (int i = 1; i < size; i++)", "Scans remaining offsets 1 through size - 1."},
                        {"if (*(arr + i) > maxVal)", "Uses pointer arithmetic *(arr + i) with zero array indexing!"},
                        {"return maxVal;", "Returns the maximum integer found."}
                    }
                }
            }},
            {"8. Heap Memory & Func Pointers", {
                {
                    "Lab: malloc() & free() Lifecycle",
                    "int *arr = (int*)malloc(5 * sizeof(int));\nif (arr == NULL) return 1;\nfor (int i = 0; i < 5; i++) arr[i] = i * 10;\nfree(arr);\narr = NULL;",
                    {
                        {"malloc(5 * sizeof(int))", "Requests 5 * 4 = 20 bytes on the heap at runtime."},
                        {"if (arr == NULL) return 1;", "Protects against out-of-memory allocation failure."},
                        {"arr[i] = i * 10;", "Uses the heap block just like a normal array (0, 10, 20, 30, 40)."},
                        {"free(arr); arr = NULL;", "Returns heap memory to OS and clears pointer so it isn't dangling."}
                    }
                },
                {
                    "Lab: Function Pointers in C",
                    "int Add(int a, int b) { return a + b; }\nint main() {\n    int (*funcPtr)(int, int) = Add;\n    int res = funcPtr(20, 10);\n    return 0;\n}",
                    {
                        {"int Add(int a, int b)", "Regular C function taking two ints and returning an int."},
                        {"int (*funcPtr)(int, int) = Add;", "Stores the memory address of Add inside funcPtr."},
                        {"int res = funcPtr(20, 10);", "Calls Add(20, 10) indirectly through the pointer; res becomes 30."}
                    }
                }
            }},
            {"9. Linux fork/pipe & Bit Ops", {
                {
                    "Linux Lab: fork() & One-Way Pipe",
                    "int fd[2]; pipe(fd);\nif (fork() == 0) {\n    close(fd[1]);\n    read(fd[0], buf, 100);\n    close(fd[0]);\n} else {\n    close(fd[0]);\n    write(fd[1], msg, len);\n    close(fd[1]);\n}",
                    {
                        {"int fd[2]; pipe(fd);", "Creates kernel pipe BEFORE fork(): fd[0] = read, fd[1] = write."},
                        {"if (fork() == 0)", "Clones process. Child enters 'if', Parent enters 'else'."},
                        {"close(fd[1]); read(fd[0], ...);", "Child closes unused write end, then waits to read message."},
                        {"close(fd[0]); write(fd[1], ...);", "Parent closes unused read end, writes message, and hangs up."}
                    }
                },
                {
                    "Bit Lab: Count 1-Bits Algorithm",
                    "int NumberOf1Bits(unsigned char x) {\n    int count = 0;\n    while (x != 0) {\n        if (x & 1) count++;\n        x >>= 1;\n    }\n    return count;\n}",
                    {
                        {"int count = 0;", "Starts our counter of set (1) bits at 0."},
                        {"while (x != 0)", "Continues shifting until all 1-bits have been counted."},
                        {"if (x & 1) count++;", "Bitwise AND with 0000 0001 checks if the rightmost bit is 1."},
                        {"x >>= 1;", "Shifts bits right by 1 so the next bit moves into position 0."},
                        {"return count;", "Returns how many 1-bits were inside the 8-bit number."}
                    }
                }
            }}
        };
    }
    else {
        // ====================================================================
        // C++ LANGUAGE CONTENT (Topic-First across Theory, Quiz, Constructor, Visualizer)
        // ====================================================================
        db.theoryTopics = {
            {"1. Classes, Objects & Structs", {
                {"Blueprint vs House (Class vs Object)", "In C++, a CLASS is a blueprint that bundles related data (fields) and actions (methods) together. An OBJECT (instance) is a real item built from that blueprint.\n\nclass Dog {\n    std::string name;\npublic:\n    void bark() { std::cout << \"Woof!\"; }\n}; // Never forget this semicolon!"},
                {"Access Modifiers: class vs struct", "Access modifiers control who can touch your data:\n- private: ONLY the class's own methods can access it.\n- public: ANY code (like main) can access it.\n\nDefault rule:\n- class members are PRIVATE by default.\n- struct members are PUBLIC by default."},
                {"Composition & The 'this' Pointer", "A class's fields can be objects of ANOTHER class! E.g., a Vector class can hold two Point objects (Point start, end;), accessed as vec.start.x.\n\nInside any method, 'this' is a hidden pointer to the current object instance (this->x = 0.0;)."},
                {"Header Files & Static Members", "Large programs declare a class in a header (vector.h) and write its methods in vector.cpp using '::' (void Point::print()).\n\nStatic members (Maths::PI, Maths::Mul()) belong to the class itself—not one object—and cannot access non-static fields."}
            }},
            {"2. Constructors, Copy & Destructors", {
                {"Constructors & Init Lists", "A constructor has the SAME name as the class and runs automatically when an object is born. You can have multiple constructors!\n\nPoint() : x(0.0), y(0.0) {} // default\nPoint(double nx, double ny) : x(nx), y(ny) {}\nThe ': x(nx)' syntax is called an initialization list."},
                {"Shallow vs Deep Copy", "When you copy an object (Student s2 = s1;), C++'s default copy constructor copies every field.\nIf a field is a raw pointer (char *name), BOTH objects end up pointing to the SAME memory! Changing s2.name accidentally changes s1.name.\nFix: Write a custom Copy Constructor that allocates fresh memory (Deep Copy)."},
                {"Destructors (~ClassName)", "A destructor (~ClassName()) runs automatically when an object dies (leaves scope or is deleted).\n\nIf your constructor allocated heap memory with 'new', your destructor MUST free it with 'delete' or 'delete[]' so your program doesn't leak gigabytes of RAM!"},
                {"Arrays of Objects & Pointers", "- Stack array: Point pts[20]; (auto-freed)\n- Heap array: Point *pts = new Point[30]; -> free with delete[] pts;\n- Array of pointers: Point **pp = new Point*[30]; -> delete each pp[i] in a loop, THEN delete[] pp;"}
            }},
            {"3. Memory: Stack, Heap & new/delete", {
                {"Scope & Dangling Pointers", "Variables created inside { } live on the STACK and disappear when } is reached.\n\nIf a pointer still holds the address of a variable that already disappeared (e.g., returning &x from a function), it becomes a DANGLING POINTER—pointing to invalid garbage memory!"},
                {"new and delete (Heap Memory)", "To keep data alive after a function finishes, allocate it on the HEAP with 'new':\nint *p = new int(5);\nWhen finished, manually free it with 'delete p;'.\n\nGolden Rules: Never use memory after delete, never delete twice, and never delete stack variables!"},
                {"Memory Leaks & Dynamic Arrays", "If you overwrite a heap pointer inside a loop without calling 'delete' first, the old memory is lost forever—a MEMORY LEAK.\n\nFor variable-size arrays, use new[] and delete[]:\nint *arr = new int[numItems];\ndelete[] arr;\nFor 2D arrays (int**), delete[] every row first, then delete[] the outer array!"},
                {"Pass by Reference (&) vs Pointer (*)", "C++ adds Pass by Reference (void swap(int &a, int &b)): the parameter becomes a direct alias for the caller's variable—cleaner than pointers!\n\nKey difference: A pointer can be 'nullptr' (pointing to nothing), while a reference MUST always refer to a valid variable."}
            }},
            {"4. Singly & Doubly Linked Lists", {
                {"What is a Linked List?", "Unlike an array (one rigid block of lockers), a Linked List is like a treasure hunt: each element (Node) sits anywhere on the heap and holds:\n1. data (the value)\n2. next (a pointer to the next Node, or nullptr at the end)."},
                {"Traversing & Inserting Nodes", "Start at 'head' and follow the arrows until nullptr:\nNode *cur = head;\nwhile (cur != nullptr) { cur = cur->next; }\n\nTo insert at the end via a standalone function, pass Node** ptrToHeadPtr so it can update head when the list is empty."},
                {"Deleting a Node Safely", "To delete a node in the middle without breaking the chain, keep TWO pointers: 'cur' and 'prev'.\n1. Walk until cur->data == value.\n2. Bypass cur: prev->next = cur->next;\n3. Free memory: delete cur;"},
                {"Doubly Linked Lists & OOP Class", "A DoublyLinkedNode stores BOTH prevPointer and nextPointer so you can walk forward AND backward!\n\nWhen wrapping a LinkedList in a C++ class, make 'head' private and implement:\n- Destructor (~LinkedList) calling clear()\n- Deep Copy Constructor & operator= (checking if (this != &other))."}
            }},
            {"5. Inheritance & Polymorphism", {
                {"Inheritance: Reusing Blueprints", "Inheritance lets you create specialized child classes (Car, Truck) from a shared base class (Vehicle):\nclass Car : public Vehicle { ... };\n\nMembers marked 'protected:' are hidden from main(), but derived classes CAN access them directly. Call Vehicle::display() to extend base methods."},
                {"Construction & Destruction Order", "When a Derived object containing a Member object is created:\n1. Base constructor -> 2. Member constructor -> 3. Derived constructor.\n\nWhen it is destroyed, the order is EXACTLY REVERSED:\n1. ~Derived -> 2. ~Member -> 3. ~Base."},
                {"Polymorphism: 'Many Forms'", "Polymorphism lets a base pointer (Shape *s = new Circle();) automatically call the derived class's version of a method at runtime!\n\nHow: Mark the base method 'virtual', and mark the derived method 'override'."},
                {"Why Virtual Destructors Matter", "CRITICAL RULE: If a base class has ANY virtual functions, it MUST have a virtual destructor:\nvirtual ~Shape() {}\n\nWithout it, 'delete shapePtr;' only runs ~Shape() and skips ~Circle(), causing a memory leak!"}
            }},
            {"6. Abstract, Interface & Friends", {
                {"Abstract Classes (= 0)", "An abstract class is an incomplete blueprint containing at least one PURE VIRTUAL function:\nvirtual double area() = 0;\n\nRules:\n- You CANNOT create objects of an abstract class directly.\n- Every derived class MUST implement all '= 0' functions."},
                {"Interface Classes (Pure Contracts)", "An Interface is a special abstract class that defines WHAT a class must do without HOW:\n- Contains ONLY pure virtual functions (= 0)\n- Usually no data members\n- Always includes a virtual destructor\n- Often prefixed with 'I' (e.g., IPrintable)."},
                {"Friend Functions", "Normally, outside functions cannot read 'private:' variables. Declaring a 'friend' function inside a class creates an explicit exception:\n\nclass Box {\n    int length;\n    friend void printLength(Box b);\n}; // printLength is NOT a member, yet can read b.length!"},
                {"Friend Classes", "Declaring 'friend class BestFriend;' inside SecretData gives ALL methods of BestFriend access to SecretData's private members.\nUse friends sparingly—for tightly coupled classes or unit testing private data."}
            }},
            {"7. Operator Overloading", {
                {"Function Overloading", "In C++, multiple functions in the same scope can share the same name (e.g., print(int), print(double), print(char*)) as long as their parameter types or counts differ.\nNote: You CANNOT overload functions that differ ONLY by return type!"},
                {"Overloading Binary Operators (+)", "You can teach C++ what '+' means for your own classes using the 'operator' keyword:\n\nBox operator+(const Box& b) {\n    Box res;\n    res.length = this->length + b.length;\n    return res;\n}\nNow 'Box3 = Box1 + Box2;' works naturally!"},
                {"Overloading Stream Output (<<)", "To print your class with 'cout << myClock;', overload << as a friend non-member function:\n\nfriend ostream& operator<<(ostream& os, Clock& c);\nAlways return 'os' by reference so chaining (cout << c1 << c2) works!"},
                {"Prefix (++c) vs Postfix (c++)", "How does C++ tell ++c and c++ apart?\n- Prefix:  Clock operator++() { tick(); return *this; }\n- Postfix: Clock operator++(int) takes a dummy unnamed 'int'! It saves a copy of *this, calls tick(), and returns the OLD copy."}
            }},
            {"8. Templates (Generic Code)", {
                {"What is a Template?", "Think of a template like a cookie-cutter mold. Instead of writing separate sum() functions for int, float, and double, you write ONE generic template and let the compiler stamp out the exact version you need!"},
                {"Template Functions", "Use 'template <typename T>' before the function:\n\ntemplate <typename T>\nT sum(T const* arr, int count) { ... }\n\nThe compiler automatically figures out T from the arguments you pass (e.g., sum(intArr, 10)). Note: T is NEVER deduced from the return type alone!"},
                {"Template Classes", "Entire classes can be generic too:\ntemplate <typename T> class Adder { T m_sum; ... };\n\nUnlike functions, template classes require explicit angle brackets when creating objects:\nAdder<int> intAdder;\nAdder<float> floatAdder;"},
                {"External Defs & Multiple Types", "1. Defining a template method outside its class requires repeating the header:\n   template <typename T> void Adder<T>::add(T n) { ... }\n2. Templates can take multiple generic types separated by commas:\n   template <typename SumType, typename AvgType>"}
            }},
            {"9. STL Vectors, Iterators & Algos", {
                {"The 3 Pillars of STL", "The Standard Template Library (STL) gives you battle-tested tools built on 3 pillars:\n1. Containers (vector, list, map, stack, queue)\n2. Algorithms (<algorithm>: sort, remove, min, max)\n3. Iterators (generalized pointers connecting algorithms to containers)"},
                {"std::vector: Size vs Capacity", "std::vector is a smart dynamic array that grows automatically via push_back().\n- size(): how many items are currently inside.\n- capacity(): how many slots are allocated in memory.\nUse reserve(n) upfront to avoid slow reallocations, and shrink_to_fit() to trim unused memory."},
                {"Iterators, begin() & end()", "Iterators use pointer-like syntax (++, --, *):\nfor (auto it = v.begin(); it != v.end(); ++it) cout << *it;\n\nCRITICAL: v.end() points ONE PAST the last item—NEVER dereference v.end()!\nAlso: push_back(), erase(), or reserve() can INVALIDATE existing vector iterators!"},
                {"Erase-Remove, Predicates & Ranges", "- Erase-Remove Idiom removes all matching values cleanly:\n  v.erase(remove(v.begin(), v.end(), 3), v.end());\n- A Predicate is a bool function passed to remove_if() or sort().\n- Prefer range insert v.insert(v.begin(), data, data+n) over loops to avoid shifting elements repeatedly."}
            }},
            {"10. STL Lists, Maps & Smart Ptrs", {
                {"std::list & splice()", "std::list is a doubly linked list (fast middle insert/delete, NO random access v[i]).\nOnly std::list supports splice()—moving nodes between lists with ZERO copying!\nTip: Always check list.empty() (O(1)) instead of list.size() == 0."},
                {"std::map vs unordered_map", "Maps store unique Key -> Value pairs (like a dictionary or DNS table):\n- std::map (<map>): binary tree, keys stay SORTED (requires operator<).\n- std::unordered_map (C++11): hash table, faster lookup, UNSORTED.\nWith iterators: it->first is the key, it->second is the value."},
                {"Map insert() vs []", "freq[\"One\"] = 1 overwrites if \"One\" already exists.\nfreq.insert(make_pair(\"One\", 1)) will NOT overwrite! It returns pair<iterator, bool> where .second is false if the key already existed."},
                {"Smart Pointers (unique & shared)", "Smart pointers delete heap memory AUTOMATICALLY—no manual delete or leaks!\n- std::unique_ptr: EXCLUSIVE single owner. Cannot be copied; transfer ownership with std::move(myBook) (original becomes nullptr).\n- std::shared_ptr: MULTIPLE owners via reference counting; object is deleted only when the LAST owner leaves scope."}
            }}
        };

        db.quizTopics = {
            {"1. Classes, Objects & Structs", {
                {"What is the default access modifier for a C++ class vs a C++ struct?", {"Both are public", "class is private, struct is public", "class is public, struct is private", "Both are private"}, 1},
                {"What must ALWAYS follow the closing brace '}' of a C++ class definition?", {"A colon :", "A semicolon ;", "The keyword end", "Nothing"}, 1},
                {"Inside a class method, what is 'this'?", {"The name of the file", "A pointer to the current object instance", "A static integer", "The base class"}, 1},
                {"Can a static member function access non-static member variables directly?", {"Yes, always", "No, that causes a compile error", "Only if they are private", "Only inside main()"}, 1}
            }},
            {"2. Constructors, Copy & Destructors", {
                {"When is a class constructor called?", {"When the program exits", "Automatically when an object is created", "Only when you call delete", "When a loop ends"}, 1},
                {"Why does a class containing a raw pointer (like char *name) need a custom Copy Constructor?", {"To make it compile", "To perform a deep copy instead of sharing the same pointer", "Because structs cannot be copied", "To disable private fields"}, 1},
                {"When is a destructor (~ClassName) called?", {"When an object goes out of scope or is deleted", "Right before the constructor", "Only when you call free()", "At compile time"}, 0},
                {"How do you free a heap array allocated with 'Point *pts = new Point[30];'?", {"delete pts;", "free(pts);", "delete[] pts;", "pts.clear();"}, 2}
            }},
            {"3. Memory: Stack, Heap & new/delete", {
                {"What is a dangling pointer?", {"A pointer set to nullptr", "A pointer referencing memory that already went out of scope or was deleted", "A pointer to a global int", "A static pointer"}, 1},
                {"Where does the 'new' operator allocate memory?", {"On the stack", "On the heap", "Inside the CPU register", "In read-only ROM"}, 1},
                {"What happens if you allocate with 'new' inside a loop and only call 'delete' once AFTER the loop?", {"All memory is freed", "A memory leak—only the last allocation is freed", "Compile error", "Nothing is allocated"}, 1},
                {"What is one key difference between Pass by Pointer and Pass by Reference in C++?", {"Pointers can be nullptr, references cannot", "References cannot modify the caller's variable", "Pointers are only in C", "References require new/delete"}, 0}
            }},
            {"4. Singly & Doubly Linked Lists", {
                {"In a singly linked list, how do you know a Node is the LAST node in the list?", {"node->data == 0", "node->next == nullptr", "node == head", "node->prev == nullptr"}, 1},
                {"Why do we track a 'prev' pointer when deleting a middle node in a singly linked list?", {"To sort the list", "To relink prev->next = cur->next before deleting cur", "To count nodes", "To call malloc()"}, 1},
                {"What extra pointer does a DoublyLinkedNode have compared to a singly linked Node?", {"headPointer", "prevPointer", "tailPointer", "dataPointer"}, 1},
                {"Inside LinkedList::operator=, why do we check 'if (this != &other)'?", {"To prevent self-assignment from wiping our own data", "To reverse the list", "To check if memory is full", "To print the list"}, 0}
            }},
            {"5. Inheritance & Polymorphism", {
                {"Who can access 'protected:' members of a Base class?", {"Everyone including main()", "The Base class and its Derived classes", "Nobody at all", "Only friend functions"}, 1},
                {"What is the exact DESTRUCTION order when a Derived object with a Member object goes out of scope?", {"Base -> Member -> Derived", "Derived -> Member -> Base", "Member -> Base -> Derived", "Base -> Derived -> Member"}, 1},
                {"Which keyword in a Base class enables runtime polymorphism via base pointers?", {"static", "virtual", "friend", "const"}, 1},
                {"What happens if you delete a Derived object through a Base* pointer WITHOUT a virtual destructor?", {"Both destructors run normally", "Only ~Base() runs, causing a memory leak", "Compile error", "The program restarts"}, 1}
            }},
            {"6. Abstract, Interface & Friends", {
                {"How do you declare a pure virtual function in C++?", {"abstract void f();", "virtual void f() = 0;", "pure void f();", "virtual void f() = null;"}, 1},
                {"Can you directly instantiate an object of an abstract class?", {"Yes, on the stack", "No, it causes a compile error", "Only with new", "Only if it has a constructor"}, 1},
                {"What defines a C++ Interface class?", {"Only private variables", "Only pure virtual functions (= 0) and a virtual destructor", "No virtual functions", "Inherits from std::vector"}, 1},
                {"Is a 'friend' function a member function of the class that declares it?", {"Yes, always", "No, it is an external function granted access to private members", "Only if it is static", "Only if it is virtual"}, 1}
            }},
            {"7. Operator Overloading", {
                {"Can you overload two functions that have the exact same parameters and differ ONLY by return type?", {"Yes", "No, parameter types or count must differ", "Only for int and double", "Only inside structs"}, 1},
                {"How does C++ distinguish postfix operator++ (c++) from prefix operator++ (++c)?", {"By return type", "Postfix takes a dummy 'int' parameter", "Prefix takes a 'bool' parameter", "Postfix is always private"}, 1},
                {"Why should overloaded 'operator<<' return 'ostream&'?", {"To save memory", "So stream outputs can be chained (cout << c1 << c2)", "To prevent compilation", "Because void is illegal"}, 1},
                {"Inside a member 'Box operator+(const Box& b)', how do you access the LEFT-hand operand?", {"Through 'b'", "Through 'this'", "Through 'main'", "Through 'operator'"}, 1}
            }},
            {"8. Templates (Generic Code)", {
                {"How does the C++ compiler determine the type T when you call a template function?", {"From its return type only", "By analyzing the argument types passed to it", "It always picks int", "From the file name"}, 1},
                {"How must you declare an object of a template class like 'Adder'?", {"Adder intAdder;", "Adder<int> intAdder;", "template Adder intAdder;", "new Adder(int);"}, 1},
                {"Can a single template function or class have multiple generic type parameters?", {"No, only one (T)", "Yes, separated by commas (template <typename X, typename Y>)", "Only up to 2", "Only for structs"}, 1},
                {"What is the main advantage of C++ templates?", {"They eliminate writing duplicate code for different data types", "They replace all pointers", "They remove the need for main()", "They disable type checking"}, 0}
            }},
            {"9. STL Vectors, Iterators & Algos", {
                {"What is the difference between vector::size() and vector::capacity()?", {"They are always identical", "size() is current elements; capacity() is total allocated slots", "capacity() is always smaller", "size() is measured in bytes"}, 1},
                {"What does v.end() point to?", {"The last element in v", "One position PAST the last element (do not dereference!)", "The first element", "nullptr"}, 1},
                {"What happens to existing vector iterators when push_back() causes memory reallocation?", {"They update automatically", "They become INVALIDATED and unsafe to use", "They point to 0", "Nothing happens"}, 1},
                {"Why must you call v.erase() AFTER std::remove() in the Erase-Remove idiom?", {"remove() only shifts elements forward and doesn't shrink the vector", "remove() deletes the whole vector", "erase() sorts the vector", "It is optional"}, 0}
            }},
            {"10. STL Lists, Maps & Smart Ptrs", {
                {"Which STL container supports splice() to move nodes without copying data?", {"std::vector", "std::map", "std::list", "std::queue"}, 2},
                {"What is the main difference between std::map and std::unordered_map?", {"std::map keeps keys sorted (tree); unordered_map uses a hash table", "unordered_map allows duplicate keys", "std::map is only for ints", "No difference"}, 0},
                {"After 'auto yourBook = std::move(myBook);' with std::unique_ptr, what is myBook?", {"A copy of yourBook", "nullptr (no longer owns the Book)", "Deleted from stack", "Still owns the Book"}, 1},
                {"When does std::shared_ptr automatically delete its managed object?", {"After the first copy", "When the LAST shared_ptr reference goes out of scope", "Never—you must call delete", "Immediately after make_shared"}, 1}
            }}
        };

        db.constructorTopics = {
            {"1. Classes, Objects & Structs", {
                {
                    "Lab: Class with Init List & Getter",
                    {
                        "class Point {",
                        "private:",
                        "    double x, y;",
                        "public:",
                        "    Point(double nx, double ny) : x(nx), y(ny) {}",
                        "    double getX() { return x; } };"
                    },
                    {
                        "Declare the Point class.",
                        "Encapsulate fields under private.",
                        "Declare x and y coordinates.",
                        "Open the public interface section.",
                        "Initialize x and y via member initialization list.",
                        "Provide getter getX() and close class with ';'."
                    },
                    {"Point(double nx, double ny) { nx = x; }"}
                },
                {
                    "Lab: Class Composition (Vector)",
                    {
                        "class Vector {",
                        "public:",
                        "    Point start;",
                        "    Point end;",
                        "    void print() { start.print(); }",
                        "};"
                    },
                    {
                        "Declare the Vector class.",
                        "Expose start and end Points publicly.",
                        "Include a Point object for start.",
                        "Include a Point object for end.",
                        "Define print() calling start's own method.",
                        "Close class definition with semicolon."
                    },
                    {"class Vector : Point {"}
                }
            }},
            {"2. Constructors, Copy & Destructors", {
                {
                    "Lab: Deep Copy Constructor",
                    {
                        "MITStudent(const MITStudent &o) {",
                        "    studentID = o.studentID;",
                        "    name = _strdup(o.name);",
                        "}"
                    },
                    {
                        "Declare copy constructor taking const reference.",
                        "Copy the integer studentID directly.",
                        "Allocate a separate copy of the string (deep copy!).",
                        "Close copy constructor."
                    },
                    {"name = o.name;"}
                },
                {
                    "Lab: Destructor Heap Cleanup",
                    {
                        "class PointBag {",
                        "    Point *points;",
                        "public:",
                        "    PointBag() { points = new Point[100]; }",
                        "    ~PointBag() { delete[] points; }",
                        "};"
                    },
                    {
                        "Declare PointBag class.",
                        "Private pointer for heap array.",
                        "Expose constructor and destructor publicly.",
                        "Allocate 100 Points on heap in constructor.",
                        "Free array with delete[] in destructor (~PointBag).",
                        "Close class with semicolon."
                    },
                    {"~PointBag() { delete points; }"}
                }
            }},
            {"3. Memory: Stack, Heap & new/delete", {
                {
                    "Lab 1 Q12: Delete Array Element (Ref)",
                    {
                        "bool delElem(int& count, int arr[], int idx) {",
                        "    if (idx < 0 || idx >= count) return false;",
                        "    for (int i = idx; i < count - 1; i++) {",
                        "        arr[i] = arr[i + 1];",
                        "    }",
                        "    count--; return true; }"
                    },
                    {
                        "Pass count by reference (int&) to update caller.",
                        "Validate index is inside [0, count - 1].",
                        "Loop from deleted index to second-to-last item.",
                        "Shift elements LEFT by 1 to close the gap.",
                        "Close loop.",
                        "Decrement count, return true, close function."
                    },
                    {"arr[i + 1] = arr[i];"}
                },
                {
                    "Lab: Dynamic 2D Array Cleanup",
                    {
                        "int **m = new int*[rows];",
                        "for (int r = 0; r < rows; r++)",
                        "    m[r] = new int[cols];",
                        "for (int r = 0; r < rows; r++)",
                        "    delete[] m[r];",
                        "delete[] m;"
                    },
                    {
                        "Allocate array of row pointers on heap.",
                        "Loop over every row.",
                        "Allocate cols ints for each row.",
                        "Loop over every row again for cleanup.",
                        "Free each row array first with delete[].",
                        "Finally free the outer row-pointer array."
                    },
                    {"delete m;"}
                }
            }},
            {"4. Singly & Doubly Linked Lists", {
                {
                    "Lab 2 Q4: Reverse Array via Pointers",
                    {
                        "int *left = arr;",
                        "int *right = arr + size - 1;",
                        "while (left < right) {",
                        "    int tmp = *left; *left = *right;",
                        "    *right = tmp;",
                        "    left++; right--; }"
                    },
                    {
                        "Point left at index 0.",
                        "Point right at the last element using pointer math.",
                        "Loop until pointers meet in the middle.",
                        "Swap values using tmp.",
                        "Store saved tmp into *right.",
                        "Step pointers inward and close loop."
                    },
                    {"while (left <= arr + size) {"}
                },
                {
                    "Lab 2 Pt2: DoublyLinkedNode Class",
                    {
                        "class DoublyLinkedNode {",
                        "public:",
                        "    int data;",
                        "    DoublyLinkedNode *prevPointer;",
                        "    DoublyLinkedNode *nextPointer;",
                        "};"
                    },
                    {
                        "Declare DoublyLinkedNode class.",
                        "Make fields accessible to DoublyLinkedList.",
                        "Store integer payload data.",
                        "Store pointer to PREVIOUS node.",
                        "Store pointer to NEXT node.",
                        "Close class with semicolon."
                    },
                    {"DoublyLinkedNode prevPointer;"}
                },
                {
                    "Lab 2 Pt2: InsertAtBeginning (DLL)",
                    {
                        "DoublyLinkedNode *n = new DoublyLinkedNode(val);",
                        "n->nextPointer = head;",
                        "n->prevPointer = nullptr;",
                        "if (head != nullptr) head->prevPointer = n;",
                        "head = n;"
                    },
                    {
                        "Create new node on the heap.",
                        "Link n forward to current head.",
                        "New head has no previous node (nullptr).",
                        "Point old head's prevPointer back to n.",
                        "Update head to point to n."
                    },
                    {"head->nextPointer = n;"}
                }
            }},
            {"5. Inheritance & Polymorphism", {
                {
                    "Lab: Polymorphic Derived Class",
                    {
                        "class Circle : public Shape {",
                        "public:",
                        "    void draw() override {",
                        "        std::cout << \"Circle\";",
                        "    }",
                        "};"
                    },
                    {
                        "Inherit publicly from Shape.",
                        "Expose draw() publicly.",
                        "Override base virtual draw() safely.",
                        "Print Circle's specialized output.",
                        "Close draw() method.",
                        "Close Circle class with semicolon."
                    },
                    {"class Circle extends Shape {"}
                },
                {
                    "Lab: Base Pointer Polymorphism",
                    {
                        "Shape *s1 = new Circle();",
                        "Shape *s2 = new Square();",
                        "s1->draw();",
                        "s2->draw();",
                        "delete s1;",
                        "delete s2;"
                    },
                    {
                        "Store a Circle object in a Shape base pointer.",
                        "Store a Square object in a Shape base pointer.",
                        "Dynamically calls Circle::draw().",
                        "Dynamically calls Square::draw().",
                        "Safely delete s1 (requires virtual ~Shape!).",
                        "Safely delete s2."
                    },
                    {"Shape s1 = new Circle();"}
                }
            }},
            {"6. Abstract, Interface & Friends", {
                {
                    "Lab: Defining an Interface Class",
                    {
                        "class IPrintable {",
                        "public:",
                        "    virtual ~IPrintable() {}",
                        "    virtual void print() = 0;",
                        "};"
                    },
                    {
                        "Prefix interface class with 'I'.",
                        "Interface contracts must be public.",
                        "Always include a virtual destructor.",
                        "Declare pure virtual method using '= 0'.",
                        "Close class with semicolon."
                    },
                    {"void print() = 0;"}
                },
                {
                    "Lab: Declaring a Friend Function",
                    {
                        "class Box {",
                        "private:",
                        "    int length;",
                        "public:",
                        "    friend void printLength(Box b);",
                        "};"
                    },
                    {
                        "Begin Box class.",
                        "Hide length under private.",
                        "Declare private integer length.",
                        "Open public section.",
                        "Grant printLength() private access using 'friend'.",
                        "Close class with semicolon."
                    },
                    {"public friend printLength(Box b);"}
                }
            }},
            {"7. Operator Overloading", {
                {
                    "Lab: Postfix Increment (Clock++)",
                    {
                        "Clock Clock::operator++(int) {",
                        "    Clock old = *this;",
                        "    tick();",
                        "    return old;",
                        "}"
                    },
                    {
                        "Dummy unnamed 'int' marks postfix ++.",
                        "Save a copy of the current object before ticking.",
                        "Advance the current Clock by 1 minute.",
                        "Return the pre-increment copy.",
                        "Close operator definition."
                    },
                    {"return *this;"}
                },
                {
                    "Lab: Overloading Box operator+",
                    {
                        "Box operator+(const Box& b) {",
                        "    Box res;",
                        "    res.length = this->length + b.length;",
                        "    return res;",
                        "}"
                    },
                    {
                        "Define member operator+ taking right-hand Box.",
                        "Create temporary result Box.",
                        "Add left operand (this) and right operand (b).",
                        "Return the combined Box.",
                        "Close operator+."
                    },
                    {"Box operator+(Box a, Box b) {"}
                }
            }},
            {"8. Templates (Generic Code)", {
                {
                    "Lab: Generic sum() Template",
                    {
                        "template <typename T>",
                        "T sum(T const* arr, int n) {",
                        "    T total = 0;",
                        "    for (int i = 0; i < n; i++) total += arr[i];",
                        "    return total;",
                        "}"
                    },
                    {
                        "Declare generic type placeholder T.",
                        "Accept a const array of T and its length.",
                        "Initialize accumulator of type T to 0.",
                        "Sum all elements in the array.",
                        "Return total of type T.",
                        "Close template function."
                    },
                    {"int sum(T const* arr, int n) {"}
                },
                {
                    "Lab: Template Class Adder<T>",
                    {
                        "template <typename T>",
                        "class Adder {",
                        "    T m_sum = 0;",
                        "public:",
                        "    void add(T x) { m_sum += x; }",
                        "    T sum() { return m_sum; } };"
                    },
                    {
                        "Prefix class with template <typename T>.",
                        "Define Adder class.",
                        "Private generic accumulator m_sum.",
                        "Expose public methods.",
                        "Add generic value x to m_sum.",
                        "Return m_sum and close class with ';'."
                    },
                    {"Adder intAdder;"}
                }
            }},
            {"9. STL Vectors, Iterators & Algos", {
                {
                    "Lab: STL Erase-Remove Idiom",
                    {
                        "#include <algorithm>",
                        "std::vector<int> v = {3, 1, 3, 2};",
                        "auto newEnd = std::remove(v.begin(), v.end(), 3);",
                        "v.erase(newEnd, v.end());"
                    },
                    {
                        "Include <algorithm> for std::remove.",
                        "Create a vector containing duplicate 3s.",
                        "Shift non-3 items forward and capture new logical end.",
                        "Erase trailing junk elements from newEnd to v.end()."
                    },
                    {"v.remove(3);"}
                },
                {
                    "Lab: Vector Reserve & Sort",
                    {
                        "std::vector<int> v;",
                        "v.reserve(100);",
                        "v.push_back(30);",
                        "v.push_back(10);",
                        "std::sort(v.begin(), v.end());"
                    },
                    {
                        "Create empty int vector.",
                        "Pre-allocate capacity for 100 items to avoid reallocations.",
                        "Append 30 to back of vector.",
                        "Append 10 to back of vector.",
                        "Sort entire range [begin, end) using STL sort."
                    },
                    {"std::sort(v);"}
                }
            }},
            {"10. STL Lists, Maps & Smart Ptrs", {
                {
                    "Lab: unique_ptr & std::move",
                    {
                        "auto myBook = std::make_unique<Book>(\"C++\");",
                        "myBook->read();",
                        "auto yourBook = std::move(myBook);",
                        "if (!myBook) {",
                        "    std::cout << \"Moved!\";",
                        "}"
                    },
                    {
                        "Create unique_ptr exclusively owning Book.",
                        "Call read() using '->'.",
                        "Transfer ownership with std::move (copying is illegal!).",
                        "Verify myBook is now nullptr.",
                        "Confirm ownership moved cleanly.",
                        "Close if block."
                    },
                    {"auto yourBook = myBook;"}
                },
                {
                    "Lab: Safe STL Map insert()",
                    {
                        "std::map<std::string, int> freq;",
                        "auto res = freq.insert(make_pair(\"One\", 1));",
                        "if (res.second == false) {",
                        "    res.first->second++;",
                        "}"
                    },
                    {
                        "Create sorted map from string keys to int counts.",
                        "Try inserting (\"One\", 1) without overwriting existing keys.",
                        "Check if key already existed (res.second == false).",
                        "Increment the existing value via returned iterator.",
                        "Close if block."
                    },
                    {"freq.insert(\"One\", 1);"}
                }
            }}
        };

        db.visualizerTopics = {
            {"1. Classes, Objects & Structs", {
                {
                    "Lab: Class, Accessors & 'this'",
                    "class Dog {\nprivate:\n    string name;\npublic:\n    Dog(string name) { this->name = name; }\n    string getName() { return name; }\n};\nint main() {\n    Dog d(\"Buddy\");\n    cout << d.getName();\n}",
                    {
                        {"private: string name;", "Hides name inside Dog so outside code cannot corrupt it."},
                        {"Dog(string name) { this->name = name; }", "'this->name' refers to the object's field, distinguishing it from parameter 'name'."},
                        {"string getName() { return name; }", "Public getter allows safe read-only access."},
                        {"Dog d(\"Buddy\"); cout << d.getName();", "Creates Dog object d and prints \"Buddy\"."}
                    }
                },
                {
                    "Lab: Static Members (Maths::PI)",
                    "class Maths {\npublic:\n    static float PI;\n    static float Mul(Point a, Point b) {\n        return a.x * b.x + a.y * b.y;\n    }\n};\nMaths::PI = 3.14f;\nfloat r = Maths::Mul(p1, p2);",
                    {
                        {"static float PI;", "Shared by the entire Maths class—no object instance needed!"},
                        {"static float Mul(Point a, Point b)", "Static method cannot access non-static fields or 'this'."},
                        {"Maths::PI = 3.14f; Maths::Mul(p1, p2);", "Accessed directly using the scope operator '::'."}
                    }
                }
            }},
            {"2. Constructors, Copy & Destructors", {
                {
                    "Lab: Shallow vs Deep Copy",
                    "class MITStudent {\npublic:\n    char *name;\n    MITStudent(const MITStudent &o) {\n        name = _strdup(o.name);\n    }\n};\nMITStudent s1; // s1.name = \"foo\"\nMITStudent s2 = s1;\ns2.name[0] = 'b'; // s1.name stays \"foo\"!",
                    {
                        {"MITStudent(const MITStudent &o)", "Custom copy constructor runs when writing 'MITStudent s2 = s1;'."},
                        {"name = _strdup(o.name);", "Allocates a brand-new string buffer so s1 and s2 do NOT share one pointer."},
                        {"s2.name[0] = 'b';", "Changes s2 to \"boo\" while s1 safely remains \"foo\"!"}
                    }
                },
                {
                    "Lab: Destructor Memory Safety",
                    "class PointBag {\npublic:\n    Point *points;\n    PointBag() { points = new Point[20000]; }\n    ~PointBag() { delete[] points; }\n};\nint main() {\n    { PointBag bag; }\n    // bag's memory is already freed here!\n}",
                    {
                        {"PointBag() { points = new Point[20000]; }", "Allocates 20,000 Points on the heap when bag is created."},
                        {"{ PointBag bag; }", "Inner block ends—bag goes out of scope immediately."},
                        {"~PointBag() { delete[] points; }", "Destructor runs automatically and frees all 20,000 Points (no RAM spike!)."}
                    }
                }
            }},
            {"3. Memory: Stack, Heap & new/delete", {
                {
                    "Lab: Heap Allocation & Leak Fix",
                    "int* getPtrToFive() {\n    int *x = new int(5);\n    return x;\n}\nint main() {\n    for (int i = 0; i < 3; ++i) {\n        int *p = getPtrToFive();\n        cout << *p << endl;\n        delete p;\n    }\n}",
                    {
                        {"int *x = new int(5);", "Allocates an int on the HEAP so it survives after the function returns."},
                        {"int *p = getPtrToFive();", "Receives the valid heap address inside the loop."},
                        {"cout << *p << endl;", "Dereferences p and prints 5."},
                        {"delete p;", "Frees memory INSIDE the loop so each turn doesn't leak!"}
                    }
                },
                {
                    "Lab: Dynamic 2D Array Cleanup",
                    "int **arr2D = new int*[rows];\nfor (int r = 0; r < rows; r++)\n    arr2D[r] = new int[cols];\n// ... use arr2D[r][c] ...\nfor (int r = 0; r < rows; r++)\n    delete[] arr2D[r];\ndelete[] arr2D;",
                    {
                        {"int **arr2D = new int*[rows];", "Allocates an array of row pointers on the heap."},
                        {"arr2D[r] = new int[cols];", "Allocates a separate array of columns for each row r."},
                        {"for (int r = 0; r < rows; r++) delete[] arr2D[r];", "Deletes every individual row FIRST—otherwise rows leak!"},
                        {"delete[] arr2D;", "Finally deletes the outer array of row pointers."}
                    }
                }
            }},
            {"4. Singly & Doubly Linked Lists", {
                {
                    "Lab 2 Pt2: Singly LinkedList Insert",
                    "void InsertAtEnd(Node** headPtr, int val) {\n    Node* n = new Node(val);\n    if (*headPtr == nullptr) { *headPtr = n; return; }\n    Node* cur = *headPtr;\n    while (cur->next != nullptr) cur = cur->next;\n    cur->next = n;\n}",
                    {
                        {"Node* n = new Node(val);", "Creates a new Node on the heap with next = nullptr."},
                        {"if (*headPtr == nullptr) { *headPtr = n; return; }", "If list is empty, updates main's head pointer via Node**!"},
                        {"while (cur->next != nullptr) cur = cur->next;", "Walks to the LAST node in the chain."},
                        {"cur->next = n;", "Links the last node to our brand-new node n."}
                    }
                },
                {
                    "Lab 2 Pt2: Singly LinkedList Delete",
                    "Node *cur = head, *prev = nullptr;\nwhile (cur != nullptr && cur->data != val) {\n    prev = cur; cur = cur->next;\n}\nif (cur != nullptr) {\n    prev->next = cur->next;\n    delete cur;\n}",
                    {
                        {"Node *cur = head, *prev = nullptr;", "Tracks current node and the node right behind it."},
                        {"while (cur != nullptr && cur->data != val)", "Walks until target value is found or list ends."},
                        {"prev->next = cur->next;", "Bypasses cur so prev points directly to the node AFTER cur."},
                        {"delete cur;", "Frees cur's heap memory cleanly with zero leak!"}
                    }
                },
                {
                    "Lab 2 Pt2: DoublyLinked InsertAtPos",
                    "void InsertAtPosition(int val, int pos) {\n    DoublyLinkedNode* n = new DoublyLinkedNode(val);\n    DoublyLinkedNode* cur = head;\n    for (int i = 0; i < pos - 1 && cur; i++) cur = cur->nextPointer;\n    n->nextPointer = cur->nextPointer;\n    n->prevPointer = cur;\n    if (cur->nextPointer) cur->nextPointer->prevPointer = n;\n    cur->nextPointer = n;\n}",
                    {
                        {"DoublyLinkedNode* n = new DoublyLinkedNode(val);", "Allocates the new doubly linked node on the heap."},
                        {"for (int i = 0; i < pos - 1 && cur; i++) cur = cur->nextPointer;", "Walks to the node right BEFORE the desired position."},
                        {"n->nextPointer = cur->nextPointer; n->prevPointer = cur;", "Connects the new node's forward and backward links first."},
                        {"if (cur->nextPointer) cur->nextPointer->prevPointer = n;", "Points the NEXT node's backward link back to n."},
                        {"cur->nextPointer = n;", "Points cur's forward link to n. All 4 links are intact!"}
                    }
                }
            }},
            {"5. Inheritance & Polymorphism", {
                {
                    "Lab: Polymorphism & Virtual Dtor",
                    "class Shape {\npublic:\n    virtual void draw() { cout << \"Shape\"; }\n    virtual ~Shape() {}\n};\nclass Circle : public Shape {\npublic:\n    void draw() override { cout << \"Circle\"; }\n};\nShape *s = new Circle();\ns->draw();\ndelete s;",
                    {
                        {"virtual void draw()", "Enables dynamic dispatch so derived methods run at runtime."},
                        {"virtual ~Shape() {}", "Ensures ~Circle() runs before ~Shape() when deleting via Shape*."},
                        {"Shape *s = new Circle();", "A Base pointer holds the address of a Derived Circle object."},
                        {"s->draw();", "Calls Circle::draw() dynamically instead of Shape::draw()!"},
                        {"delete s;", "Safely destroys Circle and Shape with no memory leak."}
                    }
                },
                {
                    "Lab: Constructor & Destructor Order",
                    "class Base { Base(); virtual ~Base(); };\nclass Member { Member(); ~Member(); };\nclass Derived : public Base {\n    Member m;\n    Derived(); ~Derived();\n};\nint main() { Derived d; }",
                    {
                        {"Derived d;", "Instantiates d on the stack, triggering construction."},
                        {"1. Base() -> 2. Member() -> 3. Derived()", "Construction always builds Base first, then Members, then Derived."},
                        {"// End of main() scope", "d leaves scope, triggering destruction in EXACT REVERSE order!"},
                        {"4. ~Derived() -> 5. ~Member() -> 6. ~Base()", "Destroys Derived first, then Member m, then Base last."}
                    }
                }
            }},
            {"6. Abstract, Interface & Friends", {
                {
                    "Lab: Abstract Animal Hierarchy",
                    "class Animal {\npublic:\n    virtual void makeSound() = 0;\n    virtual ~Animal() {}\n};\nclass Dog : public Animal {\npublic:\n    void makeSound() override { cout << \"Woof!\"; }\n};\nAnimal *a = new Dog();\na->makeSound(); delete a;",
                    {
                        {"virtual void makeSound() = 0;", "Pure virtual function makes Animal abstract (cannot write 'Animal x;')."},
                        {"void makeSound() override", "Dog MUST implement makeSound() to become instantiable."},
                        {"Animal *a = new Dog(); a->makeSound();", "Polymorphically prints \"Woof!\" through an Animal* pointer."}
                    }
                },
                {
                    "Lab: Friend Class Private Access",
                    "class SecretData {\nprivate:\n    int secret = 42;\n    friend class BestFriend;\n};\nclass BestFriend {\npublic:\n    void show(SecretData& d) { cout << d.secret; }\n};",
                    {
                        {"private: int secret = 42;", "Hidden from all normal outside code."},
                        {"friend class BestFriend;", "Grants EVERY method of BestFriend access to SecretData's private fields."},
                        {"cout << d.secret;", "Compiles and prints 42 because BestFriend is a declared friend!"}
                    }
                }
            }},
            {"7. Operator Overloading", {
                {
                    "Lab: Prefix (++c) vs Postfix (c++)",
                    "Clock Clock::operator++() {\n    tick();\n    return *this;\n}\nClock Clock::operator++(int) {\n    Clock old = *this;\n    tick();\n    return old;\n}",
                    {
                        {"Clock Clock::operator++()", "No parameters = PREFIX form (++c2)."},
                        {"tick(); return *this;", "Advances clock first, then returns the UPDATED clock."},
                        {"Clock Clock::operator++(int)", "Dummy unnamed 'int' tells compiler this is POSTFIX (c3++)."},
                        {"Clock old = *this; tick(); return old;", "Saves old time, ticks clock, and returns the pre-increment snapshot!"}
                    }
                },
                {
                    "Lab: Stream Output (operator<<)",
                    "ostream& operator<<(ostream& os, Clock& c) {\n    if (c.hour < 10) os << '0';\n    os << c.hour << \":\";\n    if (c.min < 10) os << '0';\n    os << c.min;\n    return os;\n}",
                    {
                        {"ostream& operator<<(ostream& os, Clock& c)", "Friend non-member function teaching cout how to print a Clock."},
                        {"if (c.hour < 10) os << '0';", "Pads single-digit hours like 3 into \"03\"."},
                        {"return os;", "Returns the stream reference so chaining (cout << c1 << c2) works!"}
                    }
                }
            }},
            {"8. Templates (Generic Code)", {
                {
                    "Lab: Template Function Deduction",
                    "template <typename T>\nT sum(T const* arr, int n) {\n    T total = 0;\n    for (int i = 0; i < n; i++) total += arr[i];\n    return total;\n}\nint a[] = {1, 2, 3};\ncout << sum(a, 3);",
                    {
                        {"template <typename T>", "Declares generic type placeholder T."},
                        {"T sum(T const* arr, int n)", "Works for int*, float*, double*, etc."},
                        {"sum(a, 3);", "Compiler deduces T = int from argument 'a' and stamps out sum<int>()!"}
                    }
                },
                {
                    "Lab: Multiple Template Types",
                    "template <typename SumType, typename AvgType>\nAvgType average(SumType const* arr, AvgType n) {\n    SumType sum = 0;\n    for (int i = 0; i < n; i++) sum += arr[i];\n    return (AvgType)sum / n;\n}",
                    {
                        {"template <typename SumType, typename AvgType>", "Accepts two independent generic types separated by a comma."},
                        {"average(intArr, 10)", "Instantiates <int, int> -> integer division."},
                        {"average(intArr, 10.0f)", "Instantiates <int, float> -> returns exact floating-point average!"}
                    }
                }
            }},
            {"9. STL Vectors, Iterators & Algos", {
                {
                    "Lab: Iterators & begin()/end()",
                    "vector<int> v = {30, 10, 5};\nsort(v.begin(), v.end());\nfor (auto it = v.begin(); it != v.end(); ++it) {\n    cout << *it << endl;\n}",
                    {
                        {"sort(v.begin(), v.end());", "Sorts everything from begin() up to (but NOT including) end()."},
                        {"auto it = v.begin();", "Iterator starts pointing at the first element (5)."},
                        {"it != v.end(); ++it", "Loops until it reaches one-past-the-end; never dereference end()!"},
                        {"cout << *it << endl;", "Dereferences iterator like a pointer to print 5, 10, 30."}
                    }
                },
                {
                    "Lab: Predicate with remove_if()",
                    "bool isExpired(Particle &p) {\n    return (p.timetolive() == 0);\n}\nv.erase(remove_if(v.begin(), v.end(), isExpired), v.end());",
                    {
                        {"bool isExpired(Particle &p)", "A predicate is a function returning true/false for each item."},
                        {"remove_if(v.begin(), v.end(), isExpired)", "Moves all alive particles forward and returns iterator to the junk tail."},
                        {"v.erase(..., v.end());", "Actually shrinks the vector by erasing all expired particles at the end."}
                    }
                }
            }},
            {"10. STL Lists, Maps & Smart Ptrs", {
                {
                    "Lab: Map insert() & shared_ptr",
                    "map<string, int> freq;\nauto res = freq.insert(make_pair(\"One\", 1));\nif (!res.second) res.first->second++;\nstd::shared_ptr<Library> lib2;\n{\n    auto lib1 = std::make_shared<Library>();\n    lib2 = lib1;\n}",
                    {
                        {"auto res = freq.insert(make_pair(\"One\", 1));", "Inserts only if \"One\" doesn't exist; never overwrites!"},
                        {"if (!res.second) res.first->second++;", "If key existed (res.second == false), increments its count."},
                        {"auto lib1 = std::make_shared<Library>(); lib2 = lib1;", "Both share ownership; reference count becomes 2."},
                        {"} // inner scope ends", "lib1 dies (ref count = 1). Library stays alive until lib2 dies!"}
                    }
                },
                {
                    "Lab: STL List splice() (Zero Copy)",
                    "std::list<int> L1 = {1, 2, 3, 4};\nstd::list<int> L2 = {10, 20, 30};\nauto it = L1.begin(); ++it; // points to 2\nL1.splice(it, L2);\n// L1: 1 10 20 30 2 3 4 | L2: empty!",
                    {
                        {"auto it = L1.begin(); ++it;", "Positions iterator at element 2 inside L1."},
                        {"L1.splice(it, L2);", "Relinks L2's nodes directly into L1 before 2 with ZERO copying!"},
                        {"// L1: 1 10 20 30 2 3 4", "L2 is now empty, and no objects were constructed or destroyed."}
                    }
                }
            }}
        };
    }

    return db;
}

#endif // CONTENT_DATA_H
