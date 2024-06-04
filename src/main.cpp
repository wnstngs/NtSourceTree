//++
//
// This module is responsible for the command-line interface for the end user and the
// initial initialization.
//
// The way it works:
// 1) Upon startup, the program checks the default location for kernel symbols:
// "C:\Symbols\ntkrnlmp.pdb" for existence and the presence of PDB files at that path.
// If the files exist and a PDB file is present, the program offers to generate a tree for it.
// 2) If there are multiple files at the default path, the program prompts the user to choose
// one of them.
// 3) If the default search path does not exist at all, the end - user of the program
// can specify the path where the PDB file is located.
//
//--

#include <iostream>

#include "pdbdump.h"
#include "srctree.h"

static const std::string DefaultSymbolsLocation = R"(C:\Symbols\ntkrnlmp.pdb\)";
static const std::string UsageString {
    "\x1b[32m"
    "When launching the NtSourceTree program, provide the path to ntkrnlmp.pdb as "
    "a command line argument:\n\n\tNtSourceTree.exe \"C:\\Symbols\\ntkrnlmp.pdb\\\\"
    "\033[0m\n"
};

int
wmain(
    int argc,
    wchar_t *argv[]
    )
{
    try {
        std::wstring filePath = L"C:\\Symbols\\ntkrnlmp.pdb\\B6CDAA3A3EF13C3B2799115B0470342B1\\ntkrnlmp.pdb";

        /*
        if (argc == 2) {
            filePath = argv[1];
        } else {
            //
            // Print the usage information and keep console open.
            //
            std::cout << UsageString;
            std::cin.get();
            return 1;
        }
        */

        //
        // Create a PDB dump from the .pdb file path.
        //
        const PDB_DUMP dump(filePath);
        for (const auto sourceFiles = dump.GetFiles(); 
             const auto &i : sourceFiles) {
            std::cout << i << '\n';
        }

        //
        // Create a source tree for the PDB.
        //
        SOURCE_TREE sourceTree(dump);

        //
        // Get JSON view and print it.
        //
        const nlohmann::json &jsonView = sourceTree.JsonView();
        std::cout << "\n\n\t\tJSON View:\n\n\n";
        std::cout << jsonView.dump(2) << '\n';

        CoUninitialize();

        return 0;

    } catch (std::exception &exception) {

        std::cout << "An exception occurred:\n\t";
        std::cout << "\033[31m" << exception.what() << "\033[0m\n";
    }
}
