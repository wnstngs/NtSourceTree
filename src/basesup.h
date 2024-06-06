/*!
 * @file    basesup.h
 * @details Basic support routines for the NtSourceTree project. These may include
 *          common general purpose algorithms and data structures, logging, exception
 *          and assertion infrastructure, and more.
 */

#pragma once

#include <exception>
#include <string>

class EXCEPTION_BASE : public std::exception {
};

class BUF_EXCEPTION : public EXCEPTION_BASE {
public:
    BUF_EXCEPTION();

    BUF_EXCEPTION(
        std::string Message
    );

    BUF_EXCEPTION(
        const std::wstring &Message
    );

    const char *
    what() const override;

private:
    std::string Message_;
    mutable std::string Buffer_;
};
