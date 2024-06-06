/*!
 * @file    basesup.cpp
 * @details Basic support routines for the NtSourceTree project. These may include
 *          common general purpose algorithms and data structures, logging, exception
 *          and assertion infrastructure, and more.
 *
 *          RTL, Rtl = Run-Time Library.
 */

#include "basesup.h"

#include <format>
#include <typeinfo>

std::string
Rtl::WstringToString(
    const std::wstring &Wstring
)
{
    if (Wstring.empty()) {
        return {};
    }

    size_t Actual;
    std::string String;
    String.resize(Wstring.size() * 2);

    auto Status = wcstombs_s(&Actual,
                             String.data(),
                             String.size(),
                             Wstring.c_str(),
                             _TRUNCATE);
    if (Status > 0) {
        String.resize(Actual - 1);

        return String;

    } else {
        return {};
    }
}

Rtl::BUF_EXCEPTION::BUF_EXCEPTION()
{
}

Rtl::BUF_EXCEPTION::BUF_EXCEPTION(
    std::string Message
) : Message_(std::move(Message))
{
}

Rtl::BUF_EXCEPTION::BUF_EXCEPTION(
    const std::wstring &Message
) : Message_(WstringToString(Message))
{
}

const char *
Rtl::BUF_EXCEPTION::what() const
{
    using namespace std::string_literals;

    Buffer_ = "["s + typeid(const_cast<BUF_EXCEPTION &>(*this)).name() + "]"s;

    if (!Message_.empty()) {
        Buffer_ += ": ";
        Buffer_ += Message_;
    }

    return Buffer_.c_str();
}
