// Purpose of PracticeExamData.h: Populates the curriculum collections of hands-on Practice Mode coding tasks 
// (with validation rules and expected outputs) and Exam Mode assessments 
// (multiple-choice and text-input questions) for both the C and C++ programming languages.

#ifndef PRACTICE_EXAM_DATA_H // Guard against multiple inclusions of this header file
#define PRACTICE_EXAM_DATA_H // Define the PRACTICE_EXAM_DATA_H include guard macro

#include "AppTypes.h" // Include TargetLang, PracticeTopic, ExamTopic, and related data structures

inline void PopulatePracticeAndExam( // Define inline helper function to populate Practice and Exam content
    TargetLang lang, // Target programming language used to select C or C++ content
    std::vector<PracticeTopic>& practiceTopics, // Output vector to fill with Practice Mode topics and tasks
    std::vector<ExamTopic>& examTopics) // Output vector to fill with Exam Mode topics and questions
{ // Begin function body
    if (lang == TargetLang::LANG_C) { // Populate C language Practice tasks and Exam questions when C is selected
        practiceTopics = { // Initialize the list of C Practice Mode topics
            {"1. What is C? Basics & Keywords", { // Start Topic 1 for C basics and keywords
                { // Start Task 1 definition
                    "Task 1: Hello World Program", // Set task title
                    "Write a complete C program that includes <stdio.h>, defines int main(), prints \"Hello, C!\\n\" using printf, and returns 0.", // Set task instructions
                    "Hello, C!", // Set expected output displayed on success
                    { // Begin validation rules for Task 1
                        {"#include<stdio.h>", "Missing '#include <stdio.h>' header required for printf()."}, // Require standard I/O header inclusion
                        {"intmain(", "Missing entry point function 'int main()'."}, // Require main function declaration
                        {"printf(\"Hello,C!\\n\");", "Missing printf(\"Hello, C!\\n\"); statement."}, // Require printf statement with Hello, C!
                        {"return0;", "Missing 'return 0;' at the end of main()."} // Require return 0 statement
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Constant and Variable", // Set task title
                    "Declare a constant integer MAX_HP = 100 and an integer hp = 75, then print hp using printf(\"%d\\n\", hp);.", // Set task instructions
                    "75", // Set expected output
                    { // Begin validation rules for Task 2
                        {"constintMAX_HP=100;", "Declare 'const int MAX_HP = 100;'."}, // Require constant MAX_HP declaration
                        {"inthp=75;", "Declare 'int hp = 75;'."}, // Require mutable hp integer declaration
                        {"printf(\"%d\\n\",hp);", "Print hp using printf(\"%d\\n\", hp);."} // Require printf call printing hp
                    }
                }
            }},
            {"2. Input, Output & Math", { // Start Topic 2 for C input, output, and arithmetic
                { // Start Task 1 definition
                    "Task 1: Exact Float Division", // Set task title
                    "Declare int a = 7; and int b = 2;. Cast a to float to compute float res = (float)a / b; and print it with printf(\"%.1f\\n\", res);.", // Set task instructions
                    "3.5", // Set expected output
                    { // Begin validation rules for Task 1
                        {"inta=7;", "Declare 'int a = 7;'."}, // Require declaration of integer a
                        {"intb=2;", "Declare 'int b = 2;'."}, // Require declaration of integer b
                        {"(float)a/b", "Cast 'a' to float using '(float)a / b' to avoid integer truncation."}, // Require explicit float cast during division
                        {"printf(\"%.1f\\n\",res);", "Print the result using printf(\"%.1f\\n\", res);."} // Require formatted floating-point output
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Remainder with Modulo", // Set task title
                    "Compute the remainder of 10 divided by 3 in 'int rem = 10 % 3;' and print it using printf(\"%d\\n\", rem);.", // Set task instructions
                    "1", // Set expected output
                    { // Begin validation rules for Task 2
                        {"intrem=10%3;", "Compute remainder with 'int rem = 10 % 3;'."}, // Require modulo remainder calculation
                        {"printf(\"%d\\n\",rem);", "Print rem using printf(\"%d\\n\", rem);."} // Require printing the remainder variable
                    }
                }
            }},
            {"3. Decisions & Loops", { // Start Topic 3 for C conditionals and loops
                { // Start Task 1 definition
                    "Task 1: Pass or Fail Check", // Set task title
                    "Given 'int score = 85;', write an if/else statement that prints \"Pass\\n\" if score >= 60, else prints \"Fail\\n\".", // Set task instructions
                    "Pass", // Set expected output
                    { // Begin validation rules for Task 1
                        {"intscore=85;", "Initialize 'int score = 85;'."}, // Require score variable initialization
                        {"if(score>=60)", "Check condition 'if (score >= 60)'."}, // Require passing threshold condition
                        {"printf(\"Pass\\n\");", "Print \"Pass\\n\" in the if branch."}, // Require Pass output statement
                        {"else", "Add an 'else' branch for failing scores."}, // Require else branch keyword
                        {"printf(\"Fail\\n\");", "Print \"Fail\\n\" in the else branch."} // Require Fail output statement
                    }
                },
                { // Start Task 2 definition
                    "Task 2: For Loop 1 to 3", // Set task title
                    "Write a for loop from int i = 1; i <= 3; i++ that prints each number with printf(\"%d \", i);.", // Set task instructions
                    "1 2 3", // Set expected output
                    { // Begin validation rules for Task 2
                        {"for(inti=1;i<=3;i++)", "Use loop header 'for (int i = 1; i <= 3; i++)'."}, // Require counting for-loop header
                        {"printf(\"%d\",i);", "Inside the loop, call printf(\"%d \", i);."} // Require printing loop index i
                    }
                }
            }},
            {"4. Arrays & 2D Grids", { // Start Topic 4 for C arrays and 2D grids
                { // Start Task 1 definition
                    "Task 1: Sum of Array Elements", // Set task title
                    "Declare 'int arr[3] = {10, 20, 30};', initialize 'int sum = 0;', loop from 0 to 2 adding arr[i] to sum, and print sum with printf(\"%d\\n\", sum);.", // Set task instructions
                    "60", // Set expected output
                    { // Begin validation rules for Task 1
                        {"intarr[3]={10,20,30};", "Declare 'int arr[3] = {10, 20, 30};'."}, // Require 3-element integer array declaration
                        {"intsum=0;", "Initialize 'int sum = 0;'."}, // Require accumulator initialization
                        {"sum+=arr[i];", "Accumulate elements with 'sum += arr[i];'."}, // Require summing array elements
                        {"printf(\"%d\\n\",sum);", "Print sum with printf(\"%d\\n\", sum);."} // Require printing the final sum
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Array Size with sizeof", // Set task title
                    "Declare 'int nums[] = {5, 10, 15, 20};', compute 'int len = sizeof(nums) / sizeof(nums[0]);', and print len.", // Set task instructions
                    "4", // Set expected output
                    { // Begin validation rules for Task 2
                        {"intnums[]={5,10,15,20};", "Declare 'int nums[] = {5, 10, 15, 20};'."}, // Require unsized array initialization
                        {"sizeof(nums)/sizeof(nums[0])", "Compute length via 'sizeof(nums) / sizeof(nums[0])'."}, // Require sizeof division formula for element count
                        {"printf(", "Print the calculated length using printf()."} // Require printing length with printf
                    }
                }
            }},
            {"5. C-Strings & Null Terminator", { // Start Topic 5 for C-strings and null termination
                { // Start Task 1 definition
                    "Task 1: Manual String Length", // Set task title
                    "Declare 'char s[] = \"Code\";' and 'int len = 0;'. Use a while loop checking s[len] != '\\0' to increment len, then print len.", // Set task instructions
                    "4", // Set expected output
                    { // Begin validation rules for Task 1
                        {"chars[]=\"Code\";", "Declare 'char s[] = \"Code\";'."}, // Require C-string character array declaration
                        {"intlen=0;", "Initialize 'int len = 0;'."}, // Require length counter initialization
                        {"while(s[len]!='\\0')", "Loop while 's[len] != '\\0''."}, // Require loop condition checking for null terminator
                        {"len++;", "Increment 'len++' inside the loop."}, // Require counter increment inside loop
                        {"printf(", "Print len using printf()."} // Require printf output call
                    }
                }
            }},
            {"6. Functions & Parameters", { // Start Topic 6 for C functions and parameter passing
                { // Start Task 1 definition
                    "Task 1: Function Returning Sum", // Set task title
                    "Define 'int add(int a, int b)' that returns 'a + b;'.", // Set task instructions
                    "Function add(3, 4) -> 7", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"intadd(inta,intb)", "Define signature 'int add(int a, int b)'."}, // Require add function signature
                        {"returna+b;", "Return the sum using 'return a + b;'."} // Require returning the sum of a and b
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Modify via Pointer", // Set task title
                    "Write 'void setTen(int *p)' that sets '*p = 10;'.", // Set task instructions
                    "Caller variable updated to 10", // Set expected output description
                    { // Begin validation rules for Task 2
                        {"voidsetTen(int*p)", "Define 'void setTen(int *p)'."}, // Require pointer parameter function signature
                        {"*p=10;", "Dereference p to assign 10: '*p = 10;'."} // Require pointer dereference assignment
                    }
                }
            }},
            {"7. Pointers & Pointer Math", { // Start Topic 7 for C pointers and pointer arithmetic
                { // Start Task 1 definition
                    "Task 1: Dereference and Update", // Set task title
                    "Declare 'int x = 5;', 'int *ptr = &x;', update x to 20 via '*ptr = 20;', and print x using printf(\"%d\\n\", x);.", // Set task instructions
                    "20", // Set expected output
                    { // Begin validation rules for Task 1
                        {"intx=5;", "Declare 'int x = 5;'."}, // Require integer variable x initialization
                        {"int*ptr=&x;", "Store address of x in 'int *ptr = &x;'."}, // Require pointer initialization to address of x
                        {"*ptr=20;", "Modify x through pointer with '*ptr = 20;'."}, // Require updating x via dereferencing ptr
                        {"printf(\"%d\\n\",x);", "Print x using printf(\"%d\\n\", x);."} // Require printing updated value of x
                    }
                }
            }},
            {"8. Heap Memory & Func Pointers", { // Start Topic 8 for C dynamic heap memory and function pointers
                { // Start Task 1 definition
                    "Task 1: Allocate and Free on Heap", // Set task title
                    "Allocate one int on the heap using 'int *p = (int*)malloc(sizeof(int));', check 'if (p == NULL) return 1;', set '*p = 42;', and call 'free(p);'.", // Set task instructions
                    "Allocated 42 and freed safely (0 leaks)", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"malloc(sizeof(int))", "Allocate memory using 'malloc(sizeof(int))'."}, // Require heap allocation via malloc
                        {"if(p==NULL)", "Always check 'if (p == NULL)' after malloc()."}, // Require NULL check after allocation
                        {"*p=42;", "Assign '*p = 42;'."}, // Require storing 42 into allocated memory
                        {"free(p);", "Release heap memory with 'free(p);'."} // Require freeing allocated heap pointer
                    }
                }
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Start Topic 9 for Linux processes, pipes, and bitwise operations
                { // Start Task 1 definition
                    "Task 1: Fast Multiply by 2 (Bit Shift)", // Set task title
                    "Declare 'unsigned int x = 7;', compute 'unsigned int y = x << 1;', and print y with printf(\"%u\\n\", y);.", // Set task instructions
                    "14", // Set expected output
                    { // Begin validation rules for Task 1
                        {"unsignedintx=7;", "Declare 'unsigned int x = 7;'."}, // Require unsigned integer x declaration
                        {"x<<1", "Use left bit shift 'x << 1' to multiply by 2."}, // Require left shift bitwise operation
                        {"printf(\"%u\\n\",y);", "Print y using printf(\"%u\\n\", y);."} // Require unsigned integer printf call
                    }
                }
            }}
        };

        examTopics = { // Initialize the list of C Exam Mode topics
            {"1. What is C? Basics & Keywords", { // Start Exam Topic 1 for C basics
                {ExamQuestionType::MULTIPLE_CHOICE, "Which function is the mandatory entry point of every C program?", {"start()", "main()", "init()", "run()"}, 1, {}, "main()"}, // Multiple-choice question on C program entry point
                {ExamQuestionType::TEXT_INPUT, "Type the C keyword used to lock a variable so its value cannot be modified:", {}, 0, {"const"}, "const"}, // Text-input question on const keyword
                {ExamQuestionType::MULTIPLE_CHOICE, "How many reserved keywords does standard C89 have (excluding sizeof)?", {"16", "24", "32", "64"}, 2, {}, "32"}, // Multiple-choice question on C89 keyword count
                {ExamQuestionType::TEXT_INPUT, "What value should main() return to signal successful execution to the OS?", {}, 0, {"0"}, "0"} // Text-input question on main return code
            }},
            {"2. Input, Output & Math", { // Start Exam Topic 2 for C I/O and arithmetic
                {ExamQuestionType::MULTIPLE_CHOICE, "What is the result of integer division (9 / 2) in C?", {"4.5", "4", "5", "1"}, 1, {}, "4"}, // Multiple-choice question on integer division truncation
                {ExamQuestionType::TEXT_INPUT, "Type the printf format specifier used for a standard int:", {}, 0, {"%d", "%i"}, "%d"}, // Text-input question on int format specifier
                {ExamQuestionType::TEXT_INPUT, "What is the result of the modulo expression 17 % 5?", {}, 0, {"2"}, "2"}, // Text-input question on modulo remainder
                {ExamQuestionType::MULTIPLE_CHOICE, "Why is '&' placed before a variable in scanf(\"%d\", &x)?", {"To double x", "To pass the memory address of x", "To make x constant", "To print x"}, 1, {}, "To pass the memory address of x"} // Multiple-choice question on scanf address-of operator
            }},
            {"3. Decisions & Loops", { // Start Exam Topic 3 for C control flow
                {ExamQuestionType::MULTIPLE_CHOICE, "Which loop is guaranteed to execute its body at least once?", {"for", "while", "do-while", "switch"}, 2, {}, "do-while"}, // Multiple-choice question on do-while loop guarantee
                {ExamQuestionType::TEXT_INPUT, "Type the logical AND operator in C:", {}, 0, {"&&"}, "&&"}, // Text-input question on logical AND operator
                {ExamQuestionType::TEXT_INPUT, "Which keyword stops a switch case from falling through to the next case?", {}, 0, {"break", "break;"}, "break"}, // Text-input question on switch break statement
                {ExamQuestionType::MULTIPLE_CHOICE, "What does 'continue;' do inside a loop?", {"Exits the program", "Exits the loop", "Skips to the next iteration", "Restarts main()"}, 2, {}, "Skips to the next iteration"} // Multiple-choice question on continue statement behavior
            }},
            {"4. Arrays & 2D Grids", { // Start Exam Topic 4 for C arrays
                {ExamQuestionType::TEXT_INPUT, "In 'int a[5] = {10, 20, 30, 40, 50};', what is the index of the last element (50)?", {}, 0, {"4"}, "4"}, // Text-input question on zero-based last index
                {ExamQuestionType::MULTIPLE_CHOICE, "How do you compute the number of elements in a local C array 'arr'?", {"arr.size()", "sizeof(arr) / sizeof(arr[0])", "length(arr)", "sizeof(arr)"}, 1, {}, "sizeof(arr) / sizeof(arr[0])"}, // Multiple-choice question on computing C array length
                {ExamQuestionType::MULTIPLE_CHOICE, "In 'int grid[3][4];', how many rows does grid have?", {"4", "3", "7", "12"}, 1, {}, "3"}, // Multiple-choice question on 2D array row dimension
                {ExamQuestionType::TEXT_INPUT, "What index does the first element of any C array have?", {}, 0, {"0"}, "0"} // Text-input question on starting array index
            }},
            {"5. C-Strings & Null Terminator", { // Start Exam Topic 5 for C-strings
                {ExamQuestionType::TEXT_INPUT, "Type the escape sequence for the C-string null terminator:", {}, 0, {"\\0", "'\\0'"}, "\\0"}, // Text-input question on null terminator escape sequence
                {ExamQuestionType::MULTIPLE_CHOICE, "How many bytes does the string literal \"Hi\" occupy in memory?", {"2", "3", "4", "1"}, 1, {}, "3"}, // Multiple-choice question on string literal size including null byte
                {ExamQuestionType::MULTIPLE_CHOICE, "What happens if printf(\"%s\", buf) prints a char array missing '\\0'?", {"It stops safely", "It reads past the end into garbage memory", "Compile error", "Prints nothing"}, 1, {}, "It reads past the end into garbage memory"} // Multiple-choice question on missing null terminator danger
            }},
            {"6. Functions & Parameters", { // Start Exam Topic 6 for C functions
                {ExamQuestionType::TEXT_INPUT, "Type the return type used for a function that returns no value:", {}, 0, {"void"}, "void"}, // Text-input question on void return type
                {ExamQuestionType::MULTIPLE_CHOICE, "How are regular int arguments passed to functions in C?", {"By value (copy)", "By reference", "By heap", "By macro"}, 0, {}, "By value (copy)"}, // Multiple-choice question on pass-by-value semantics
                {ExamQuestionType::MULTIPLE_CHOICE, "When an array is passed to a C function and modified, what happens to the caller's array?", {"It stays unchanged", "It is modified directly", "It is deleted", "Compile error"}, 1, {}, "It is modified directly"} // Multiple-choice question on array decay to pointer in function calls
            }},
            {"7. Pointers & Pointer Math", { // Start Exam Topic 7 for C pointers
                {ExamQuestionType::TEXT_INPUT, "Type the operator used to get the memory address of a variable:", {}, 0, {"&"}, "&"}, // Text-input question on address-of operator
                {ExamQuestionType::TEXT_INPUT, "Type the operator used to dereference a pointer:", {}, 0, {"*"}, "*"}, // Text-input question on dereference operator
                {ExamQuestionType::MULTIPLE_CHOICE, "Which expression is equivalent to arr[3] using pointer arithmetic?", {"*arr + 3", "*(arr + 3)", "&arr + 3", "arr * 3"}, 1, {}, "*(arr + 3)"}, // Multiple-choice question on array indexing via pointer math
                {ExamQuestionType::MULTIPLE_CHOICE, "Which pointer operation is ILLEGAL in C?", {"ptr++", "ptr + 2", "ptr - 1", "ptr * 2"}, 3, {}, "ptr * 2"} // Multiple-choice question on illegal pointer multiplication
            }},
            {"8. Heap Memory & Func Pointers", { // Start Exam Topic 8 for C dynamic memory
                {ExamQuestionType::TEXT_INPUT, "Type the C standard library function used to allocate heap memory:", {}, 0, {"malloc", "malloc()"}, "malloc"}, // Text-input question on malloc
                {ExamQuestionType::TEXT_INPUT, "Type the C function used to release heap memory:", {}, 0, {"free", "free()"}, "free"}, // Text-input question on free
                {ExamQuestionType::MULTIPLE_CHOICE, "What does malloc() return if memory allocation fails?", {"0xFF", "-1", "NULL", "1"}, 2, {}, "NULL"}, // Multiple-choice question on malloc failure return value
                {ExamQuestionType::MULTIPLE_CHOICE, "Forgetting to call free() on allocated heap memory causes:", {"A memory leak", "A syntax error", "Faster execution", "Stack overflow"}, 0, {}, "A memory leak"} // Multiple-choice question on memory leaks
            }},
            {"9. Linux fork/pipe & Bit Ops", { // Start Exam Topic 9 for Linux system calls and bitwise operations
                {ExamQuestionType::TEXT_INPUT, "What integer value does fork() return inside the CHILD process?", {}, 0, {"0"}, "0"}, // Text-input question on fork child return value
                {ExamQuestionType::MULTIPLE_CHOICE, "Calling fork() 3 times in a row results in how many total processes?", {"3", "6", "8", "9"}, 2, {}, "8"}, // Multiple-choice question on exponential process creation with fork
                {ExamQuestionType::TEXT_INPUT, "What is the result of bitwise expression 5 << 1?", {}, 0, {"10"}, "10"}, // Text-input question on left shift arithmetic
                {ExamQuestionType::MULTIPLE_CHOICE, "In 'int fd[2]; pipe(fd);', which index is the READ end?", {"fd[0]", "fd[1]", "fd[2]", "Both"}, 0, {}, "fd[0]"} // Multiple-choice question on pipe read file descriptor index
            }}
        };
    }
    else { // Populate C++ language Practice tasks and Exam questions when C++ is selected
        practiceTopics = { // Initialize the list of C++ Practice Mode topics
            {"1. Classes, Objects & Structs", { // Start C++ Topic 1 for classes and objects
                { // Start Task 1 definition
                    "Task 1: Simple Dog Class", // Set task title
                    "Define a class Dog with a public method 'void bark()' that prints \"Woof!\" using std::cout, and remember the closing semicolon ';'.", // Set task instructions
                    "Woof!", // Set expected output
                    { // Begin validation rules for Task 1
                        {"classDog{", "Declare 'class Dog {'."}, // Require Dog class header
                        {"public:", "Add 'public:' access specifier so bark() can be called."}, // Require public access specifier
                        {"voidbark()", "Define method 'void bark()'."}, // Require bark member function signature
                        {"cout<<\"Woof!\"", "Print \"Woof!\" using cout."}, // Require cout stream output of Woof!
                        {"};", "Do not forget the semicolon '};' at the end of the class definition."} // Require trailing semicolon after class block
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Using the 'this' Pointer", // Set task title
                    "Write a setter 'void setX(int x)' that assigns the parameter to member x using 'this->x = x;'.", // Set task instructions
                    "Member x updated via this->x", // Set expected output description
                    { // Begin validation rules for Task 2
                        {"voidsetX(intx)", "Define 'void setX(int x)'."}, // Require setX setter signature
                        {"this->x=x;", "Assign member using 'this->x = x;'."} // Require disambiguating member assignment with this->
                    }
                }
            }},
            {"2. Constructors, Copy & Destructors", { // Start C++ Topic 2 for constructors and destructors
                { // Start Task 1 definition
                    "Task 1: Constructor with Init List", // Set task title
                    "Write a constructor 'Point(int nx, int ny) : x(nx), y(ny) {}' using a member initialization list.", // Set task instructions
                    "Point(3, 4) initialized cleanly", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"Point(intnx,intny)", "Define constructor 'Point(int nx, int ny)'."}, // Require parameterized Point constructor signature
                        {":x(nx),y(ny)", "Use member initialization list ': x(nx), y(ny)'."} // Require member initializer list syntax
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Destructor Cleanup", // Set task title
                    "Write a destructor '~Buffer()' that frees a heap array member 'data' using 'delete[] data;'.", // Set task instructions
                    "Heap array freed cleanly on destruction", // Set expected output description
                    { // Begin validation rules for Task 2
                        {"~Buffer()", "Define destructor '~Buffer()'."}, // Require Buffer destructor signature
                        {"delete[]data;", "Free heap array with 'delete[] data;' (not scalar delete)."} // Require array delete[] operator
                    }
                }
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Start C++ Topic 3 for memory management and references
                { // Start Task 1 definition
                    "Task 1: Heap Int with new & delete", // Set task title
                    "Allocate an integer initialized to 42 on the heap: 'int *p = new int(42);', print '*p', and free it with 'delete p;'.", // Set task instructions
                    "42", // Set expected output
                    { // Begin validation rules for Task 1
                        {"int*p=newint(42);", "Allocate using 'int *p = new int(42);'."}, // Require dynamic allocation with new int(42)
                        {"*p", "Dereference '*p' to access 42."}, // Require dereferencing heap pointer
                        {"deletep;", "Free heap memory with 'delete p;'."} // Require releasing memory via delete
                    }
                },
                { // Start Task 2 definition
                    "Task 2: Swap by Reference", // Set task title
                    "Write 'void swapRef(int &a, int &b)' that swaps a and b using a temporary int variable.", // Set task instructions
                    "Caller variables swapped in-place", // Set expected output description
                    { // Begin validation rules for Task 2
                        {"voidswapRef(int&a,int&b)", "Pass parameters by reference: 'int &a, int &b'."}, // Require reference parameter function signature
                        {"a=b;", "Assign b into a."}, // Require assigning b to a
                        {"b=temp;", "Assign saved temp into b."} // Require restoring temp into b
                    }
                }
            }},
            {"4. Singly & Doubly Linked Lists", { // Start C++ Topic 4 for linked lists
                { // Start Task 1 definition
                    "Task 1: Traverse Linked List", // Set task title
                    "Start with 'Node *cur = head;' and write a while loop 'while (cur != nullptr)' that moves 'cur = cur->next;'.", // Set task instructions
                    "Visited all nodes until nullptr", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"Node*cur=head;", "Initialize pointer 'Node *cur = head;'."}, // Require traversal pointer initialization
                        {"while(cur!=nullptr)", "Loop while 'cur != nullptr'."}, // Require null-checked traversal loop
                        {"cur=cur->next;", "Advance pointer using 'cur = cur->next;'."} // Require advancing to next node
                    }
                }
            }},
            {"5. Inheritance & Polymorphism", { // Start C++ Topic 5 for inheritance and polymorphism
                { // Start Task 1 definition
                    "Task 1: Override Virtual Method", // Set task title
                    "Declare 'class Circle : public Shape' with a public method 'void draw() override' that prints \"Circle\".", // Set task instructions
                    "Circle", // Set expected output
                    { // Begin validation rules for Task 1
                        {"classCircle:publicShape", "Inherit publicly: 'class Circle : public Shape'."}, // Require public inheritance from Shape
                        {"voiddraw()override", "Mark overridden method with 'void draw() override'."}, // Require override specifier on draw()
                        {"\"Circle\"", "Print \"Circle\" inside draw()."}, // Require Circle output text
                        {"};", "End class definition with '};'."} // Require closing class semicolon
                    }
                }
            }},
            {"6. Abstract, Interface & Friends", { // Start C++ Topic 6 for abstract classes and interfaces
                { // Start Task 1 definition
                    "Task 1: Pure Virtual Interface", // Set task title
                    "Define 'class IPrintable' with a public virtual destructor 'virtual ~IPrintable() {}' and pure virtual method 'virtual void print() = 0;'.", // Set task instructions
                    "Interface contract IPrintable defined", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"classIPrintable", "Declare 'class IPrintable'."}, // Require IPrintable class declaration
                        {"virtual~IPrintable()", "Include a virtual destructor 'virtual ~IPrintable()'."}, // Require virtual destructor for safe polymorphic deletion
                        {"virtualvoidprint()=0;", "Declare pure virtual method 'virtual void print() = 0;'."}, // Require pure virtual method declaration
                        {"};", "End class with '};'."} // Require closing class semicolon
                    }
                }
            }},
            {"7. Operator Overloading", { // Start C++ Topic 7 for operator overloading
                { // Start Task 1 definition
                    "Task 1: Postfix Increment Signature", // Set task title
                    "Write 'Counter operator++(int)' that saves 'Counter old = *this;', increments 'count++;', and returns 'old;'.", // Set task instructions
                    "Returns pre-increment snapshot", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"operator++(int)", "Include dummy 'int' parameter for postfix ++."}, // Require postfix operator++ dummy int parameter
                        {"Counterold=*this;", "Save previous state in 'Counter old = *this;'."}, // Require saving copy of current object
                        {"returnold;", "Return the saved snapshot 'return old;'."} // Require returning pre-increment snapshot
                    }
                }
            }},
            {"8. Templates (Generic Code)", { // Start C++ Topic 8 for generic templates
                { // Start Task 1 definition
                    "Task 1: Generic Max Template", // Set task title
                    "Write 'template <typename T>' followed by 'T getMax(T a, T b)' returning '(a > b) ? a : b;'.", // Set task instructions
                    "getMax(3, 9) -> 9 | getMax(2.5, 1.2) -> 2.5", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"template<typenameT>", "Start with 'template <typename T>'."}, // Require template parameter header
                        {"TgetMax(Ta,Tb)", "Define generic signature 'T getMax(T a, T b)'."}, // Require templated function signature
                        {"return", "Return the larger value."} // Require return statement
                    }
                }
            }},
            {"9. STL Vectors, Iterators & Algos", { // Start C++ Topic 9 for STL vectors and algorithms
                { // Start Task 1 definition
                    "Task 1: Vector Push and Sort", // Set task title
                    "Create 'std::vector<int> v = {30, 10, 20};', push 5 with 'v.push_back(5);', and sort it using 'std::sort(v.begin(), v.end());'.", // Set task instructions
                    "5 10 20 30", // Set expected output
                    { // Begin validation rules for Task 1
                        {"vector<int>v", "Declare 'std::vector<int> v'."}, // Require integer vector declaration
                        {"v.push_back(5);", "Append 5 using 'v.push_back(5);'."}, // Require appending 5 with push_back
                        {"sort(v.begin(),v.end());", "Sort range with 'std::sort(v.begin(), v.end());'."} // Require sorting full iterator range
                    }
                }
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Start C++ Topic 10 for STL containers and smart pointers
                { // Start Task 1 definition
                    "Task 1: Transfer unique_ptr Ownership", // Set task title
                    "Create 'auto p1 = std::make_unique<int>(99);' and transfer ownership to p2 using 'auto p2 = std::move(p1);'.", // Set task instructions
                    "p1 is nullptr, *p2 == 99", // Set expected output description
                    { // Begin validation rules for Task 1
                        {"make_unique<int>(99)", "Create unique_ptr with 'std::make_unique<int>(99)'."}, // Require make_unique allocation
                        {"std::move(p1)", "Transfer ownership via 'std::move(p1)'."} // Require ownership move with std::move
                    }
                }
            }}
        };

        examTopics = { // Initialize the list of C++ Exam Mode topics
            {"1. Classes, Objects & Structs", { // Start C++ Exam Topic 1 for classes and structs
                {ExamQuestionType::MULTIPLE_CHOICE, "What is the default access level of members inside a C++ class?", {"public", "private", "protected", "static"}, 1, {}, "private"}, // Multiple-choice question on default class visibility
                {ExamQuestionType::TEXT_INPUT, "Type the keyword pointer inside a method that refers to the current object instance:", {}, 0, {"this"}, "this"}, // Text-input question on this pointer
                {ExamQuestionType::MULTIPLE_CHOICE, "What is the default access level of members inside a C++ struct?", {"private", "protected", "public", "friend"}, 2, {}, "public"}, // Multiple-choice question on default struct visibility
                {ExamQuestionType::TEXT_INPUT, "What punctuation character MUST follow the closing brace '}' of a class definition?", {}, 0, {";"}, ";"} // Text-input question on trailing class semicolon
            }},
            {"2. Constructors, Copy & Destructors", { // Start C++ Exam Topic 2 for object lifecycle
                {ExamQuestionType::TEXT_INPUT, "What symbol prefixes a class name to declare its destructor?", {}, 0, {"~"}, "~"}, // Text-input question on destructor tilde prefix
                {ExamQuestionType::MULTIPLE_CHOICE, "Which operator frees a heap array allocated with 'new Point[20]'?", {"delete", "free", "delete[]", "remove"}, 2, {}, "delete[]"}, // Multiple-choice question on array delete[]
                {ExamQuestionType::MULTIPLE_CHOICE, "When a class owns a raw pointer, what kind of copy should a custom Copy Constructor perform?", {"Shallow copy", "Deep copy", "Null copy", "Static copy"}, 1, {}, "Deep copy"} // Multiple-choice question on deep vs shallow copying
            }},
            {"3. Memory: Stack, Heap & new/delete", { // Start C++ Exam Topic 3 for memory and references
                {ExamQuestionType::TEXT_INPUT, "Type the C++ operator used to allocate memory on the heap:", {}, 0, {"new"}, "new"}, // Text-input question on new operator
                {ExamQuestionType::TEXT_INPUT, "Type the modern C++11 keyword representing a null pointer:", {}, 0, {"nullptr"}, "nullptr"}, // Text-input question on nullptr keyword
                {ExamQuestionType::MULTIPLE_CHOICE, "Which statement is TRUE about C++ references vs pointers?", {"References can be nullptr", "Pointers can be nullptr, references cannot", "References use ->", "Pointers cannot be reassigned"}, 1, {}, "Pointers can be nullptr, references cannot"} // Multiple-choice question comparing pointers and references
            }},
            {"4. Singly & Doubly Linked Lists", { // Start C++ Exam Topic 4 for linked lists
                {ExamQuestionType::TEXT_INPUT, "In a singly linked list, what is the value of the last node's 'next' pointer?", {}, 0, {"nullptr", "NULL", "0"}, "nullptr"}, // Text-input question on tail node next pointer
                {ExamQuestionType::MULTIPLE_CHOICE, "Why do we check 'if (this != &other)' inside operator= ?", {"To prevent self-assignment bugs", "To sort nodes", "To make it faster", "To delete other"}, 0, {}, "To prevent self-assignment bugs"}, // Multiple-choice question on self-assignment guard
                {ExamQuestionType::MULTIPLE_CHOICE, "What extra pointer does a Doubly Linked List node store?", {"tail", "prev", "root", "index"}, 1, {}, "prev"} // Multiple-choice question on doubly linked list prev pointer
            }},
            {"5. Inheritance & Polymorphism", { // Start C++ Exam Topic 5 for inheritance and polymorphism
                {ExamQuestionType::TEXT_INPUT, "Type the keyword placed on a base class method to enable runtime polymorphism:", {}, 0, {"virtual"}, "virtual"}, // Text-input question on virtual keyword
                {ExamQuestionType::MULTIPLE_CHOICE, "In what order are destructors called when a Derived object goes out of scope?", {"Base then Derived", "Derived then Base", "Random order", "Only Base"}, 1, {}, "Derived then Base"}, // Multiple-choice question on destructor invocation order
                {ExamQuestionType::MULTIPLE_CHOICE, "Which access specifier allows Derived classes access while hiding members from main()?", {"private", "public", "protected", "static"}, 2, {}, "protected"} // Multiple-choice question on protected access level
            }},
            {"6. Abstract, Interface & Friends", { // Start C++ Exam Topic 6 for abstract classes and friends
                {ExamQuestionType::TEXT_INPUT, "Type the suffix added to a virtual function declaration to make it pure virtual:", {}, 0, {"= 0", "=0"}, "= 0"}, // Text-input question on pure virtual = 0 syntax
                {ExamQuestionType::TEXT_INPUT, "Type the keyword that grants an external function access to a class's private members:", {}, 0, {"friend"}, "friend"}, // Text-input question on friend keyword
                {ExamQuestionType::MULTIPLE_CHOICE, "Can you directly instantiate an object of an abstract class?", {"Yes", "No, it causes a compile error", "Only on the heap", "Only in structs"}, 1, {}, "No, it causes a compile error"} // Multiple-choice question on abstract class instantiation
            }},
            {"7. Operator Overloading", { // Start C++ Exam Topic 7 for operator overloading
                {ExamQuestionType::TEXT_INPUT, "What dummy parameter type distinguishes postfix operator++ from prefix operator++?", {}, 0, {"int"}, "int"}, // Text-input question on postfix increment dummy parameter
                {ExamQuestionType::MULTIPLE_CHOICE, "Can you overload two functions that differ ONLY by their return type?", {"Yes", "No", "Only for templates", "Only for void"}, 1, {}, "No"}, // Multiple-choice question on overloading rules
                {ExamQuestionType::MULTIPLE_CHOICE, "Why does overloaded 'operator<<' return 'ostream&'?", {"To allow chaining like cout << a << b", "To save stack space", "Because void is invalid", "To copy cout"}, 0, {}, "To allow chaining like cout << a << b"} // Multiple-choice question on stream insertion chaining
            }},
            {"8. Templates (Generic Code)", { // Start C++ Exam Topic 8 for templates
                {ExamQuestionType::TEXT_INPUT, "Type the keyword that starts a generic function or class declaration:", {}, 0, {"template"}, "template"}, // Text-input question on template keyword
                {ExamQuestionType::MULTIPLE_CHOICE, "How do you instantiate a template class 'Box' for integers?", {"Box b;", "Box<int> b;", "template Box b;", "Box(int) b;"}, 1, {}, "Box<int> b;"}, // Multiple-choice question on class template instantiation syntax
                {ExamQuestionType::MULTIPLE_CHOICE, "Can a template function deduce T solely from its return type?", {"Yes", "No, it deduces T from arguments", "Only for int", "Only in C++98"}, 1, {}, "No, it deduces T from arguments"} // Multiple-choice question on template type deduction
            }},
            {"9. STL Vectors, Iterators & Algos", { // Start C++ Exam Topic 9 for STL vectors and algorithms
                {ExamQuestionType::MULTIPLE_CHOICE, "What does v.end() point to?", {"The last element", "One position PAST the last element", "The first element", "nullptr"}, 1, {}, "One position PAST the last element"}, // Multiple-choice question on past-the-end iterator
                {ExamQuestionType::TEXT_INPUT, "Which vector method adds a new element to the back of the vector?", {}, 0, {"push_back", "push_back()"}, "push_back"}, // Text-input question on vector push_back
                {ExamQuestionType::MULTIPLE_CHOICE, "Which pair of calls forms the Erase-Remove idiom?", {"v.delete + v.pop", "v.erase(std::remove(...), v.end())", "v.clear + v.resize", "std::sort + v.erase"}, 1, {}, "v.erase(std::remove(...), v.end())"} // Multiple-choice question on Erase-Remove idiom
            }},
            {"10. STL Lists, Maps & Smart Ptrs", { // Start C++ Exam Topic 10 for STL containers and smart pointers
                {ExamQuestionType::TEXT_INPUT, "Type the std::list method that transfers nodes between lists with zero copying:", {}, 0, {"splice", "splice()"}, "splice"}, // Text-input question on std::list::splice
                {ExamQuestionType::MULTIPLE_CHOICE, "Which smart pointer enforces exclusive single ownership?", {"std::shared_ptr", "std::unique_ptr", "std::weak_ptr", "raw pointer"}, 1, {}, "std::unique_ptr"}, // Multiple-choice question on std::unique_ptr ownership
                {ExamQuestionType::TEXT_INPUT, "Type the function used to transfer ownership from one std::unique_ptr to another:", {}, 0, {"std::move", "move"}, "std::move"}, // Text-input question on std::move
                {ExamQuestionType::MULTIPLE_CHOICE, "Which associative container keeps its keys automatically sorted?", {"std::unordered_map", "std::map", "std::vector", "std::list"}, 1, {}, "std::map"} // Multiple-choice question on sorted std::map container
            }}
        };
    }
}

#endif // End of PRACTICE_EXAM_DATA_H include guard