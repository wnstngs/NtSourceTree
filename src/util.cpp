//++
//
// util.cpp
//
// This module implements auxiliary run-time functions. All of them are
// free and located in the 'Util' namespace.
//
//--

#include "util.h"

#include <comdef.h>
#include <filesystem>
#include <stdexcept>

namespace Util {
//++
//
// This function returns the status from the supplied HRESULT as a text.
//
//--
std::string
ErrorMessage(HRESULT Hresult)
{
    return std::system_category().message(Hresult);
}

//++
//
// This function converts the supplied BSTR to std::string.
//
//--
std::string
BstrToString(BSTR Bstr)
{
    if (!Bstr) {
        return {};
    }

    auto length = SysStringLen(Bstr);
    const auto size = WideCharToMultiByte(CP_UTF8,
                                          0,
                                          Bstr,
                                          length,
                                          nullptr,
                                          0,
                                          nullptr,
                                          nullptr);

    if (size <= 0) {
        return {};
    }

    std::string result(size, 0);

    WideCharToMultiByte(CP_UTF8,
                        0,
                        Bstr,
                        length,
                        result.data(),
                        size,
                        nullptr,
                        nullptr);

    return result;
}

std::string
DiaSourceFileToString(IDiaSourceFile **SourceFile)
{
    BSTR bstrName{};

    if ((*SourceFile)->get_fileName(&bstrName) != S_OK) {
        throw
            std::runtime_error("DiaSourceFileToString: (*SourceFile)->get_fileName failed");
    }

    return BstrToString(bstrName);
}

std::string
DiaSymbolToString(IDiaSymbol **Compiland)
{
    BSTR bstrName{};

    if ((*Compiland)->get_name(&bstrName) != S_OK) {
        throw
            std::runtime_error("DiaSymbolToString: (*Compiland)->get_name failed");
    }

    return BstrToString(bstrName);
}

//++
//
// This function examines whether a directory exists at the supplied path.
//
//--
bool
directoryExists(const std::wstring &path)
{
    return std::filesystem::is_directory(path);
}

//++
//
// This function splits the supplied path into its components.
//
//--
std::vector<std::string>
SplitPath(const std::string &Path)
{
    std::vector<std::string> result;
    std::stringstream ss(Path);
    std::string token;

    while (std::getline(ss, token, '\\')) {
        result.push_back(token);
    }

    return result;
}
}
