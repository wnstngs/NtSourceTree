/*!
 * @file    pdbdump.cpp
 * @details This module contains the implementation of the PDB_DUMP class, which allows
 *          users to access data from a specific PDB file.
 *          The constructor of the class takes the path to the PDB file as an argument.
 *          Loading data from the PDB file occurs automatically. Public methods of the
 *          class allow to directly retrieve necessary information, such as a list
 *          of all functions or modules.
 */

#include "pdbdump.h"

#include <format>

#include "util.h"

PDB_DUMP::PDB_DUMP(std::wstring PdbFilePath)
    : PdbFilePath_(std::move(PdbFilePath))
{
    LoadDiaForPdb();
}

std::vector<std::string>
PDB_DUMP::GetFiles() const
{
    std::vector<std::string> sourceFiles;

    //
    // In order to find the source files, we have to look at the image's compilands/modules.
    //

    IDiaEnumSymbols *enumSymbols;
    IDiaSymbol *compiland;
    //
    // celt - The number of symbols in the enumerator to be retrieved.
    //
    constexpr ULONG celt = 1;
    //
    // fetchedCelt - The number of symbols in the fetched enumerator.
    //
    ULONG fetchedCelt = 0;

    auto status = DiaGlobalSymbol_->findChildren(SymTagCompiland, nullptr, nsNone, &enumSymbols);

    if (FAILED(status)) {
        throw std::runtime_error(std::format("[PDB_DUMP::GetSourceFiles] DiaGlobalSymbol_->findChildren failed: {0}",
                                             Util::ErrorMessage(status)));
    }

    while (SUCCEEDED(enumSymbols->Next(celt, &compiland, &fetchedCelt)) &&
        fetchedCelt == celt) {

        auto symbol = Util::DiaSymbolToString(&compiland);

        if (symbol.find("minkernel") != std::string::npos) {

            //
            // Only consider the path if it contains "minkernel".
            //
            sourceFiles.emplace_back(Util::DiaSymbolToString(&compiland));
        }

        //
        // Every compiland could contain multiple references to the source files which
        // were used to build it. Retrieve all source files by compiland by passing NULL
        // for the name of the source file.
        //

        IDiaEnumSourceFiles *enumSourceFiles;

        if (SUCCEEDED(DiaSession_->findFile(compiland, nullptr, nsNone, &enumSourceFiles))) {

            IDiaSourceFile *sourceFile;
            while (SUCCEEDED(enumSourceFiles->Next(celt, &sourceFile, &fetchedCelt)) &&
                fetchedCelt == celt) {

                symbol = Util::DiaSourceFileToString(&sourceFile);

                if (symbol.find("minkernel") != std::string::npos) {

                    //
                    // Only consider the path if it contains "minkernel".
                    //
                    sourceFiles.emplace_back(Util::DiaSourceFileToString(&sourceFile));
                }

                sourceFile->Release();
            }
            enumSourceFiles->Release();
        }
        compiland->Release();
    }
    enumSymbols->Release();

    return sourceFiles;
}

std::vector<std::string>
PDB_DUMP::GetPublics()
{
    // TODO: Not implemented.
    return std::vector<std::string>{};
}

std::vector<std::string>
PDB_DUMP::GetSymbols()
{
    std::vector<std::string> symbols;

    IDiaEnumSymbols *enumSymbols;

    auto status =
        DiaGlobalSymbol_->findChildren(SymTagCompiland,
                                       nullptr,
                                       nsNone,
                                       &enumSymbols);

    if (FAILED(status)) {
        throw std::runtime_error(std::format("[PDB_DUMP::GetSymbols] DiaGlobalSymbol_->findChildren failed: {0}",
                                             Util::ErrorMessage(status)));
    }

    IDiaSymbol *compiland;
    ULONG celt = 0;

    while (SUCCEEDED(enumSymbols->Next(1, &compiland, &celt)) && (celt == 1)) {

        //
        // Find all the symbols defined in this compiland and print their info.
        //

        IDiaEnumSymbols *enumChildren;

        if (SUCCEEDED(compiland->findChildren(SymTagNull, NULL, nsNone, &enumChildren))) {
            IDiaSymbol *diaSymbol;
            ULONG celtChildren = 0;

            while (SUCCEEDED(enumChildren->Next(1, &diaSymbol, &celtChildren)) &&
                (celtChildren == 1)) {

                symbols.emplace_back(Util::DiaSymbolToString(&diaSymbol));

                diaSymbol->Release();
            }

            enumChildren->Release();
        }

        compiland->Release();
    }

    enumSymbols->Release();

    return symbols;
}

std::vector<std::string>
PDB_DUMP::GetTypes()
{
    // TODO: Not implemented.
    return std::vector<std::string>{};
}

void
PDB_DUMP::LoadDiaForPdb()
{
    //
    // Obtain access to the provider.
    //

    auto result = CoInitialize(nullptr);

    result = CoCreateInstance(__uuidof(DiaSource),
                              nullptr,
                              CLSCTX_INPROC_SERVER,
                              __uuidof(IDiaDataSource),
                              reinterpret_cast<void **>(&DiaDataSource_));

    if (FAILED(result)) {
        throw std::runtime_error(std::format("[PDB_DUMP::LoadDiaForPdb] CoCreateInstance failed: {0}",
                                             Util::ErrorMessage(result)));
    }

    //
    // Open and prepare a program database (.pdb) file as a debug data source.
    //

    result = DiaDataSource_->loadDataFromPdb(PdbFilePath_.c_str());

    if (FAILED(result)) {
        throw std::runtime_error(std::format("[PDB_DUMP::LoadDiaForPdb] DiaDataSource_->loadDataFromPdb failed: {0}",
                                             Util::ErrorMessage(result)));
    }

    //
    // Open a session for querying symbols.
    //

    result = DiaDataSource_->openSession(&DiaSession_);

    if (FAILED(result)) {
        throw std::runtime_error(std::format("[PDB_DUMP::LoadDiaForPdb] DiaDataSource_->openSession failed: {0}",
                                             Util::ErrorMessage(result)));
    }

    //
    // Retrieve a reference to the global scope.
    //

    result = DiaSession_->get_globalScope(&DiaGlobalSymbol_);

    if (result != S_OK) {
        throw std::runtime_error(std::format("[PDB_DUMP::LoadDiaForPdb] DiaSession_->get_globalScope failed: {0}",
                                             Util::ErrorMessage(result)));
    }
}
