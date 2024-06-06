/*!
 * @file    basesup.cpp
 * @details Basic support routines for the NtSourceTree project. These may include
 *          common general purpose algorithms and data structures, logging, exception
 *          and assertion infrastructure, and more.
 */

#include "basesup.h"

#include <format>
#include <typeinfo>

BUF_EXCEPTION::BUF_EXCEPTION()
{
}

BUF_EXCEPTION::BUF_EXCEPTION(
    std::string Message
) : Message_(std::move(Message))
{
}

BUF_EXCEPTION::BUF_EXCEPTION(
    const std::wstring &Message
) : Message_(Common::Util::WstringToString(Message))
{
}

const char *
BUF_EXCEPTION::what() const
{
    using namespace std::string_literals;

    Buffer_ = "["s + typeid(const_cast<BUF_EXCEPTION &>(*this)).name() + "]"s;

    if (!Message_.empty()) {
        Buffer_ += ": ";
        Buffer_ += Message_;
    }

    return Buffer_.c_str();
}
