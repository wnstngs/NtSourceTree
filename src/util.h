#pragma once

#include <string>
#include <vector>

#include "dia2.h"

namespace Util {
std::string
ErrorMessage(HRESULT Hresult);

std::string
BstrToString(BSTR Bstr);

/*!
 * @brief This function returns the supplied IDiaSourceFile as a string
 *        file name.
 * @param SourceFile The IDiaSourceFile to be converted into a string.
 * @return A file name as a string retrieved from the source file.
 */
std::string
DiaSourceFileToString(IDiaSourceFile **SourceFile);

/*!
 * @brief This function returns the supplied IDiaSymbol as a string.
 * @param Compiland The IDiaSymbol compiland to be converted into a string.
 * @return A string retrieved from the compiland.
 */
std::string
DiaSymbolToString(IDiaSymbol **Compiland);

bool
DirectoryExists(const std::string &Path);

std::vector<std::string>
SplitPath(const std::string &Path);
}
