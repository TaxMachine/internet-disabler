//
// Created by TaxMachine on 2023-09-03.
//

#ifndef INTERNET_DISABLER_WLANEXCEPTION_HPP
#define INTERNET_DISABLER_WLANEXCEPTION_HPP

#include <exception>

class WlanException : public std::exception {
    public:
        explicit WlanException(const char* message) : message(message) {}

        const char* what() const noexcept override {
            return message;
        }

    private:
        const char* message;
};

#endif //INTERNET_DISABLER_WLANEXCEPTION_HPP
