/*!
 * @file    pdbdump.h
 * @details This is a header file for the PDB_DUMP class, which allows
 *          users to access data from a specific PDB file.
 *          The constructor of the class takes the path to the PDB file as an argument.
 *          Loading data from the PDB file occurs automatically. Public methods of the
 *          class allow to directly retrieve necessary information, such as a list
 *          of all functions or modules.
 */

#pragma once

#include <string>
#include <vector>

#include "basesup.h"
#include "dia2.h"

class PDB_EXCEPTION : public Rtl::BUF_EXCEPTION {
public:
    using BUF_EXCEPTION::BUF_EXCEPTION;
};

class PDB_DUMP {
public:
    explicit
    PDB_DUMP(std::wstring PdbFilePath);

    /*!
     * @brief This function returns the paths of all dumped files from the PDB.
     * @return A vector of all the paths from the PDB.
     */
    std::vector<std::string>
    GetFiles() const;

    /*!
     * @brief This function retrieves the public symbols from the PDB.
     * @return A list of public symbols.
     * @bug Not implemented.
     */
    std::vector<std::string>
    GetPublics();

    /*!
     * @brief This function retrieves all symbols from the PDB.
     * @return A vector of all symbols.
     */
    std::vector<std::string>
    GetSymbols();

    /*!
     * @brief This function retrieves all types from the PDB.
     * @return A vector of all types.
     * @bug Not implemented.
     */
    std::vector<std::string>
    GetTypes();

private:
    /*!
     * @brief This function initializes DIA for the provided PDB file.
     */
    void
    LoadDiaForPdb();

    std::wstring PdbFilePath_;
    IDiaDataSource *DiaDataSource_{};
    IDiaSession *DiaSession_{};
    IDiaSymbol *DiaGlobalSymbol_{};
};
