/**
 * info.h
 * Contains the static info struct for displaying log information to the console window.
 * by radj307
 */
#pragma once
#include <iostream>

/**
 * struct info (STATIC)
 * Container for console logging functions
 * Call with: 'info::msg(info::t::<type>, <string>)'
 */
struct info {
    // valid types
    const enum {
        log,
        error,
        message
    };
    // type shortcut
   // typedef info::type t;

    /**
     * info::msg(const type, const string)
     * Prints an info message to the console
     * 
     * @param msg_type          - The type of message, displayed in a tag at the beginning of the line
     * @param msg_content       - The message to display
     * @param pressKeyPrompt    - Message to display while waiting for user input to continue, don't include this when calling to disable waiting for input.
     */
    template <class EnumType>
    static void msg(const EnumType msg_type, const std::string msg_content, const std::string pressKeyPrompt = "")
    {
        if (msg_content != "")
            std::cout << type_to_str(msg_type) << msg_content << std::endl;
        // check if a string was included for the wait prompt
        if (pressKeyPrompt != "") {
            wait_for_input(pressKeyPrompt);
        } // if no wait prompt message was included, don't wait
    }

private:
    // Wait for user to press a key before continuing
    static inline void wait_for_input(std::string show_message)
    {
        // display message & _getch(), cast to void to suppress warning as the return val is not used.
        std::cout << show_message; (void)(std::cin.get());
    }

    // converts enum class type into a string
    template <class EnumType>
    static std::string type_to_str(const EnumType type_to_convert)
    {
        switch (type_to_convert) {
        case log:
            return "[LOG]\t";
        case error:
            return "[ERROR]\t";
        case message:
            return "[MSG]\t";
        default:
            return "[???]\t";
        }
    }
};