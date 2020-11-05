/**
 * syscall.h
 * Contains execution function & dependencies
 * by radj307
 */
#pragma once
#include <iostream>     // for cout
#include <sstream>      // for stringstream
#include <array>        // for std::array
#include <conio.h>      // for _getch()
 // determine operating system
#if _WIN32 || _WIN64
#include <Windows.h>    // for windows console API
#define WINDOWS         // set windows preproc definition
#elif __GNUC__
#define GNU             // set GNU preproc def
#endif

static bool __SHOW_LOGS = true; // when false, all messages flagged as logs will not be displayed to the console

/**
 * struct sys
 * Contains various system calls without using the system() function
 */
struct sys {
    /** STATIC **
     * exec(const char*)
     * Executes a system call, and returns the result as a stringstream reference.
     *
     * @param cmd       - The command to execute
     * @returns stringstream
     */
    static inline std::stringstream exec(const char* cmd)
    {
        // create an array of 256 chars to receive output
        std::array<char, 256> buffer;
        // create a stringstream to store results
        std::stringstream result;
        // create pipe to terminal
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if ( !pipe ) // if pipe failed to open, throw exception
            throw std::runtime_error("_popen() failed!");
        while ( fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr ) {
            result << buffer.data();
        }
        return result;
    }

    /** STATIC **
     * exec_inline(const char*)
     * Executes a system call in the terminal, and returns the result.
     * This is the equivalent of exec() function with ECHO on
     *
     * @param cmd       - The command to execute
     * @returns array<char, 256>
     */
    static inline std::array<char, 256> exec_inline(const char* cmd)
    {
        std::array<char, 256> buffer;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if ( !pipe )
            throw std::runtime_error("_popen() failed!");
        while ( fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr ) {
            std::cout << buffer.data() << std::endl;
        }
        return buffer;
    }

    /** STATIC **
     * wait_for_input(string)
     * Prints a string to the console, then waits for a key press before continuing.
     *
     * @param show_message  - A message to prompt the user for a key press
     */
    static inline void wait_for_input(std::string show_message)
    {
        std::cout << show_message; // print message to console
        (void)(_getch()); // wait for a keypress, cast to void to prevent compiler warning
        std::cout << std::endl; // print newline character
    }

    /** STATIC **
     * type_to_str(template EnumType)
     * Converts a valid EnumType to string
     *
     * @param type_to_convert   - A valid sys::<type>
     * @returns string          - Line prefix
     */
    template <class EnumType>
    static std::string type_to_str(const EnumType type_to_convert)
    {
        switch ( type_to_convert ) {
        case log:       return "[LOG]\t";       // for log information that doesn't require input
        case error:     return "[ERROR]\t";     // for error information, prompting user to fix
        case message:   return "[MSG]\t";       // for generic messages, prompting user
        default:        return "[???]\t";
        }
    }

    // disable unscoped enum warning
    #pragma warning( disable: 26812 )
    /** STATIC **
     * Valid Types - Line prefix
     *
     * @member log      - [LOG]     - for logs that don't require user intervention
     * @member error    - [ERROR]   - for errors that require intervention
     * @member message  - [MSG]     - for messages that require intervention
     */
    const enum {
        log,
        error,
        message
    };

    /** STATIC **
     * sys::msg(template EnumType, string, string)
     * Prints an info message to the console
     *
     * @param msg_type          - The type of message, displayed in a tag at the beginning of the line
     * @param msg_content       - The message to display
     * @param pressKeyPrompt    - Message to display while waiting for user input to continue, don't include this when calling to disable waiting for input.
     */
    template <class EnumType>
    static void msg(const EnumType msg_type, const std::string msg_content, const std::string pressKeyPrompt = "")
    {
        if ( msg_content != "" && msg_type != log || (msg_content != "" && __SHOW_LOGS && msg_type == log) )
            std::cout << type_to_str(msg_type) << msg_content << std::endl;
        // check if a string was included for the wait prompt
        if ( pressKeyPrompt != "" )
            wait_for_input(pressKeyPrompt);
    }

#ifdef WINDOWS
    /** STATIC **
     * cls()
     * Clears the terminal window.
     * (_WIN32 or _WIN64 evaluated to true)
     */
    static void cls()
    {
        HANDLE                     hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD                      count;
        DWORD                      cellCount;
        COORD                      homeCoords = { 0, 0 };

        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if ( hStdOut == INVALID_HANDLE_VALUE ) return;

        /* Get the number of cells in the current buffer */
        if ( !GetConsoleScreenBufferInfo(hStdOut, &csbi) ) return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
        if ( !FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count
        ) ) return;

        /* Fill the entire buffer with the current colors and attributes */
        if ( !FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
        ) ) return;

        /* Move the cursor home */
        SetConsoleCursorPosition(hStdOut, homeCoords);
    }
#elif GNU
    /** STATIC **
     * cls()
     * Clears the terminal window.
     * (__GNUC__ evaluated to true)
     */
    static void cls()
    {
        std::system("clear");
    }
#endif
};