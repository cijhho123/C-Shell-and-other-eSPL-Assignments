# Extended System Programming Laboratory (eSPL) Assignments
This course focuses on low-level system programming principles. The labs are based on interaction with the LINUX operating system, mostly using C, but also some assembly language. 

Issues covered in the course are:
1. Processes, memory models, and interaction with the operating system.
2. A programmer’s introduction to C: storage types, pointers, structures.
3. The raw machine: basics of (x86) assembly language, linking to functions
written in C.
4. Direct system calls: a programmer’s interface with system services.
5. User’s view of the Linux file system, processes, and access permissions.
6. Command interpreters: Unix “shell”.
7. “Binary” files: formats, manipulation.
8. The ELF format, linking and loading.

All the lecture material is available [here](Lectures)

# Chronological Order
The tasks were given as pairs of home assignment and frontal lab, so the order is as follows:
A -> 1 -> B -> 2 -> C -> 3 -> D -> 4 -> E -> 5

# Frontal Labs
## Lab 1 - C, Pointers & Memory Management, and Simulating Object-Oriented Behavior
This assignment covers key concepts in C programming, focusing on memory management and pointers, debugging techniques, and simulating object-oriented behavior in C.  
It includes understanding storage addresses, using pointers with various data types, and working with structs and function pointers to mimic object-oriented programming.  
Debugging using tools like gdb and exploring memory layout through practical exercises are also emphasized.

## Lab 2 - Implementing a Simple Shell and Exploring Unix Process Management
This lab is designed to deepen your understanding of Unix/Linux system programming by guiding you through the development of a simple command interpreter, commonly known as a shell.  
The key learning objectives include implementing a basic shell that can receive and execute user commands, managing processes using system calls such as fork(), exec(), and waitpid(), and introducing the concept of Unix signals with custom signal handlers.  
Additionally, the lab covers input/output redirection and introduces the basics of inter-process communication using pipes. The emphasis is on writing modular code that can be expanded upon in future assignments.

## Lab 3 - Assembly Language and System Calls Primer
This Assignment introduces assembly language programming and system calls. It covers creating simple assembly programs, interfacing with system calls, and calling assembly code from C.  
The lab involves tasks like printing command-line arguments using system calls, implementing an encoder in assembly, and developing a virus-like program that appends code to executable files.  
Emphasis is placed on avoiding standard libraries and directly using system calls for all operations.

## Lab 4 - Introduction to ELF File Manipulation and Patching
Lab 4 introduces the basics of ELF file handling and manipulation. It covers extracting and editing ELF file information using tools like readelf and hex editors.  
Developing a tool, hexeditplus, to manage file operations such as loading, modifying, and saving data. 
The lab also involves analyzing ELF structures, fixing issues in an ELF executable, and patching faulty functions. Overall, it combines practical coding with understanding ELF file formats.

## Lab 5 -  Implementing a Static ELF Loader
This lab involves creating a static loader for ELF files, which does not rely on dynamic libraries. It covers developing an iterator to process ELF program headers, implementing a function to print header details and mapping segments into memory using ```mmap``` . 
Writing a loader that runs ELF executables and passes control to the loaded code. The assignment includes compiling with specific flags, verifying functionality, and handling command-line arguments for the loaded program.

## Lab 6 - Creating and Managing an ELF File Virus
This lab involves understanding and manipulating ELF file structures through the creation of a simple computer virus. 
Implementing a virus in assembly that infects ELF executables by appending code to the end of the file and modifying its headers so the virus runs upon execution.  
The lab also includes ensuring that the original program can run after the virus, updating the program headers to guarantee the virus code is loaded, and developing an anti-virus tool to detect and deactivate the virus.

Note: This lab is no longer part of the course for several years now, I have asked the lecturer for this out of curiosity. I'll do it in the future.
 


# Home Assignments
## Lab A - Intro to C and File Handling in Unix: Command-Line Parsing, Debugging, and Encoding
This assignment covers basic C programming in a Unix environment, focusing on command-line argument parsing, debugging, and file handling. 
Developing a text encoding program with options for input/output redirection and debug mode. The task includes creating a makefile for project management and implementing encoding based on command-line parameters.

## Lab B - Debugging, Dynamic data structures: Linked lists, Patching binary files
This assignment covers debugging with Valgrind, implementing linked lists in C, and manipulating binary files.  
Building a virus detection tool (premitive YARA-esque) that uses linked lists to store and compare virus signatures, and develop methods to detect and neutralize viruses in files.

## Lab C - Enhancing Shell Functionality: Pipes, Job Control, and History
Lab C extends the shell functionality from a previous assignment by adding features such as job control, pipes, and command history.  
It involves creating a simple pipeline program, integrating pipe support into the shell, managing processes with a process manager, and implementing a history mechanism for command retrieval and management.

## Lab D - Advanced Assembly Language Programming: Multi-Precision Arithmetic and PRNG
This lab advances assembly language skills by focusing on C integration, dynamic memory use, and multi-precision arithmetic.  
Key tasks include printing and reading multi-precision integers, performing their addition, and implementing a pseudo-random number generator.  
The final goal is to integrate these components into a versatile program handling different input modes.

Note: This lab was removed from the course this semester due to time crunch,  I'll do it in the future for fun.

## Lab E - Implementing a Basic ELF Linker: Parsing, Symbol Handling, and File Merging
This assignment focuses on building a simplified ELF linker. It involves parsing ELF files to extract and display header, section, and symbol information.  
The primary tasks include utilizing the ```mmap``` system call to access ELF file data, implementing functions to print section names and symbols, and performing a basic merge of two ELF files. 
The merging process combines sections from both files into a new ELF file, handling symbols and section data according to specified constraints.



# System Programming Laboratory (SPL)
This course is a follow up of the System Programming Laboratory (SPL) course. It's assignments are available here:  
[Project 1 - Warehouse Game in C++11](https://github.com/cijhho123/Warehouse-Game)  
[Project 2 - Set Game in Java](https://github.com/cijhho123/Set-Game)  
[Project 3 - TFTP Implementation in Java](https://github.com/cijhho123/Extended-TFTP-Implementation)

# Credit
Thanks to the eSPL course staff.
All the tasks in pairs are a collaboration with I.S.
