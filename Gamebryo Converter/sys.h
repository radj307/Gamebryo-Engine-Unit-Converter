/**
 * sys.h
 * Contains system functions, mostly native API calls where possible
 * by radj307
 */
#pragma once
// Global libs:
#include <iostream>     // for cout
#include <sstream>      // for stringstream
#include <chrono>       // for time units       Used in sleep()
#include <thread>       // for thread           Used in sleep()
#include <array>        // for std::array

// Optional libs:
#include "termcolor.hpp"

// Determine Platform:
#if _WIN32 || _WIN64
#define CROSS_PLATFORM  // Works on Windows & Linux
#define ARCH_WINNT        // Only works on Windows
// Windows-Only libs:
#include <conio.h>      // for _getch()
#include <Windows.h>    // for windows console API
#elif __GNUC__
#define CROSS_PLATFORM  // Works on Windows & Linux
#define ARCH_UNIX       // Only works on Linux
#endif

// Global Vars:

// when false, all messages flagged as logs will not be displayed to the console
static bool __SHOW_LOGS = true;

// Objects:

/**
 * struct sys
 * Contains various system calls without using the system() function
 */
struct sys {
#if defined(CROSS_PLATFORM)

    /** STATIC **
     * msg_prefix(const EnumType (template))
     * Returns the matching line prefix for a given msg type
     *
     * @param type_to_convert   - A valid info::<type>
     * @returns string          - Line prefix
     */
    template <class EnumType>
    static inline std::string msg_prefix(const EnumType type_to_convert)
    {
        switch (type_to_convert) {
        case log:       return "[LOG]\t";
        case message:   return "[MSG]\t";
        case warning:   return "[WARN]\t";
        case error:     return "[ERROR]\t";
        case debug:     return "[DEBUG]\t";
        default:        return "[???]\t";
        }
    }


    #pragma warning( disable: 26812 )
    // Used by the sys::msg() function to determine line prefix.
    enum {
        // Used for non-critical logs
        log,        // [LOG]
        // Used for critical logs
        message,    // [MSG]
        // Used for non-critical errors
        warning,    // [WARN]
        // Used for critical errors
        error,      // [ERROR]
        // Used for debug messages, only shown in Debug configuration.
        debug,      // [DEBUG]
    };

    /** STATIC **
     * info::msg(template EnumType, string, string)
     * Wrapper for prefixed console output. Displays a given string in the format: [???]\t<msg_content>\n<pressKeyPrompt>
     *
     * @param msg_type          - The type of message, displayed in a tag at the beginning of the line
     * @param msg_content       - The message to display
     * @param pressKeyPrompt    - (Default: disabled) Replace with any string to call the sys::pause(pressKeyPrompt) command after displaying the message.
     */
    template <class EnumType>
    static void msg(const EnumType msg_type, const std::string msg_content, const std::string pressKeyPrompt = "")
    {
        // debug message type
        if ( msg_type == debug ) {
        #ifdef _DEBUG // If in debug configuration
            #ifdef TERMCOLOR_HPP_
                std::cout << termcolor::on_magenta << msg_prefix(msg_type) << msg_content << termcolor::reset << std::endl;
            #else
                std::cout << msg_prefix(msg_type) << msg_content << std::endl;
            #endif
        #endif
        }
        // other message type
        else if ( msg_content != "" && msg_type != log || (msg_content != "" && __SHOW_LOGS && msg_type == log) ) {
        #ifdef TERMCOLOR_HPP_ // 
            if ( msg_type == error )
                std::cout << termcolor::red << msg_prefix(msg_type) << msg_content << std::endl << termcolor::reset;
            else if ( msg_type == warning )
                std::cout << termcolor::yellow << msg_prefix(msg_type) << msg_content << std::endl << termcolor::reset;
            else
                std::cout << termcolor::grey << msg_prefix(msg_type) << msg_content << std::endl << termcolor::reset;
        #else
            std::cout << msg_prefix(msg_type) << msg_content << std::endl;
        #endif
        }
        // check for wait prompt
        if ( pressKeyPrompt != "" )
            pause(pressKeyPrompt);
    }

    /** STATIC **
     * info::msg(template EnumType, const char*, const char*)
     * Wrapper for prefixed console output. Displays a given const char* in the format: [???]\t<msg_content>\n<pressKeyPrompt>
     *
     * @param msg_type          - The type of message, displayed in a tag at the beginning of the line
     * @param msg_content       - The message to display
     * @param pressKeyPrompt    - (Default: disabled) Replace with any const char* to call the sys::pause(pressKeyPrompt) command after displaying the message.
     */
    template <class EnumType>
    static void msg(const EnumType msg_type, const char* msg_content, const char* pressKeyPrompt = "")
    {
        if ((msg_content != "" && msg_type != log) || (msg_content != "" && __SHOW_LOGS && msg_type == log))
            std::cout << msg_prefix(msg_type) << msg_content << std::endl;
        // check if a string was included for the wait prompt
        if (pressKeyPrompt != "")
            pause(pressKeyPrompt);
    }

    /** STATIC **
     * sleep(time_t)
     * Thread waits for an amount of time.
     *
     * @param CHRONO_TIME_UNIT	- Unit of time from the chrono library. (Default: milliseconds)
     * @param count				- How long to wait, measured in CHRONO_TIME_UNIT.
     */
    template <class CHRONO_TIME_UNIT = std::chrono::milliseconds>
    static inline void sleep(time_t count)
    {
        if (count > 0)
            std::this_thread::sleep_for(CHRONO_TIME_UNIT(count));
        else
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

#endif
#if defined(ARCH_WINNT)

    /** STATIC **
     * exec(const char*)
     * Executes a system call, and returns the result as a string.
     *
     * @param cmd       - The command to execute
     * @returns string
     */
    static inline std::string exec(const char* cmd)
    {
        // create an array of 256 chars to receive output
        std::array<char, 256> buffer;
        // create a stringstream to store results
        std::string result = "";
        // create pipe to terminal
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if (!pipe) // if pipe failed to open, throw exception
            throw std::runtime_error("_popen() failed!");
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    /** STATIC **
     * exec(const char*, int&)
     * Executes a system call, returns the result as string, and the return code to a referenced int.
     *
     * @param cmd        - The command to execute
     * @param returnCode - Ref to an int to hold the return code from process
     * @returns string
     */
    static inline std::string exec(const char* cmd, int& returnCode)
    {
        // create an array of 256 chars to receive output
        std::array<char, 256> buffer;
        // create a stringstream to store results
        std::string result = "";
        // create pipe to terminal:
        auto pipe = _popen(cmd, "r");
        if (!pipe) // if pipe failed to open, throw exception
            throw std::runtime_error("_popen() failed!");
        while (feof(pipe)) {
            if (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
                result += buffer.data();
        }
        returnCode = _pclose(pipe);
        return result;
    }

    /** STATIC **
     * pause(string)
     * Prints a string to the console, then waits for a key press before continuing.
     * NOTE ON WSL/LINUX: Since getch isn't available on linux, only ENTER will be accepted to continue!
     *
     * @param show_message  - A message to prompt the user for a key press
     */
    static inline void pause(std::string show_message = "Press any key to continue....")
    {
        std::cout << show_message; // print message to console
        (void)(_getch()); // wait for a keypress, cast to void to prevent compiler warning
        std::cout << std::endl; // print newline character
    }

    /** STATIC **
     * cls()
     * Clears the terminal window.
     */
    static void cls()
    {
        HANDLE                     hStdOut;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD                      count;
        DWORD                      cellCount;
        COORD                      homeCoords = { 0, 0 };

        hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hStdOut == INVALID_HANDLE_VALUE) return;

        /* Get the number of cells in the current buffer */
        if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;

        /* Fill the entire buffer with spaces */
        if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count
        )) return;

        /* Fill the entire buffer with the current colors and attributes */
        if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count
        )) return;

        /* Move the cursor home */
        SetConsoleCursorPosition(hStdOut, homeCoords);
    }

#endif
#if defined(ARCH_UNIX)

    /** STATIC **
     * exec(const char*)
     * Executes a system call, and returns the result as a string.
     *
     * @param cmd       - The command to execute
     * @returns string
     */
    static inline std::string exec(const char* cmd)
    {
        // create an array of 256 chars to receive output
        std::array<char, 256> buffer;
        // create a stringstream to store results
        std::string result = "";
        // create pipe to terminal
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) // if pipe failed to open, throw exception
            throw std::runtime_error("popen() failed!");
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    /** STATIC **
     * exec(const char*, int&)
     * Executes a system call, returns the result as string, and the return code to a referenced int.
     *
     * @param cmd        - The command to execute
     * @param returnCode - Ref to an int to hold the return code from process
     * @returns string
     */
    static inline std::string exec(const char* cmd, int& returnCode)
    {
        // create an array of 256 chars to receive output
        std::array<char, 256> buffer;
        // create a stringstream to store results
        std::string result = "";
        // create pipe to terminal
        auto pipe = popen(cmd, "r");
        if (!pipe) // if pipe failed to open, throw exception
            throw std::runtime_error("_popen() failed!");
        while (feof(pipe)) {
            if (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
                result += buffer.data();
        }
        returnCode = pclose(pipe);

        return result;
    }

    /** STATIC **
     * pause(string)
     * Prints a string to the console, then waits for a key press before continuing.
     * NOTE ON WSL/LINUX: Since getch isn't available on linux, only ENTER will be accepted to continue!
     *
     * @param show_message  - A message to prompt the user for a key press
     */
    static inline void pause(std::string show_message = "Enter any character to continue....")
    {
        std::stringstream ss;
        std::cout << show_message << std::endl; // print message to console
        std::cin.ignore();
        std::cout << std::endl; // print newline character
    }

    /** STATIC **
     * cls()
     * Clears the terminal window.
     */
    static inline void cls()
    {
        std::system("clear");
    }

#endif
}; // struct sys