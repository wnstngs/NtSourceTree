﻿#pragma once

#include <unordered_map>

#include "pdbdump.h"
#include "thirdparty/json.h"

class SOURCE_TREE {
private:
    struct NODE {
        std::unordered_map<std::string, NODE> Children;
    };

    struct JSON_NODE {
        std::string Name;
        std::string Type;
        std::vector<JSON_NODE> Children;
    };

public:
    explicit
    SOURCE_TREE(
        PDB_DUMP PdbDump
    );

    nlohmann::json &
    JsonView();

    void
    HtmlView(
        const std::string &Path
    );

    void
    TxtView(
        const nlohmann::json &Node,
        const std::string &FileName,
        const std::string &Indent = ""
    );

private:
    void
    CleanupSourceFiles();

    void
    BuildJsonHierarchy();

    void
    ContructJsonNode(
        nlohmann::json &Root,
        const std::filesystem::path &Path
    );

    PDB_DUMP PdbDump_;
    std::string HtmlPath_;
    std::string TxtPath_;
    std::vector<std::string> SourceFiles_;
    nlohmann::json JsonHierarchy_;

    /*!
     * @brief "Junk" substrings to be erased from each path.
     */
    std::vector<std::string> SubstringsToErase_ = {
        "mp\\",
        "obj\\",
        "amd64fre\\",
        "objfre\\",
        "amd64\\",
        "nt.ssh5\\",
        "build_server_km\\",
        "mp_embed\\",
        "mp_noltcg\\",
        "..\\"

        //
        // Add more substrings here if needed.
        //
    };

    /*!
     * @brief Rules for which substring to replace and by which substring.
     *        For example, used to replace file extensions.
     */
    std::unordered_map<std::string, std::string> SubstringsToReplace_ = {
        {".obj", ".c"},
        {".lib", ".h"},
        {".res", ".rc"},
        {".exp", ".lib"}

        //
        // Add more substrings here if needed.
        //
    };

    /*!
     * @brief Additional substring replacement rules used for some specific files
     *        that are known to have a very specific extension.
     */
    std::unordered_map<std::string, std::string> ExtensionsForReplacement_ = {
        //
        // Files known to be C++:
        //

        //
        // STORE
        //
        {"storemgr.c", "storemgr.cpp"},
        //
        // NLS
        //
        {"localedl.c", "localedl.cpp"},
        //
        // PS
        //
        {"psscprv.c", "psscprv.cpp"}, // Based on: `?PspFinalizeScpCfgPage...`, `?PspFinalizeScpCfgPage...`

        //
        // Files known to be ASM:
        //

        //
        // KE
        //
        {"apcint.c", "apcint.asm"},
        {"callout.c", "callout.asm"},
        {"clocktick.c", "clocktick.asm"},
        {"ctxswap.c", "ctxswap.asm"},
        {"dpcint.c", "dpcint.asm"},
        {"idle.c", "idle.asm"},
        {"start.c", "start.asm"},
        {"sysstubs.c", "sysstubs.asm"},
        {"systable.c", "systable.asm"},
        {"threadbg.c", "threadbg.asm"},
        {"trap.c", "trap.asm"},
        //
        // RTL
        //
        {"slist.c", "slist.asm"},
        {"chkstk.c", "chkstk.asm"},
        {"stkwalk.c", "stkwalk.asm"},
        {"movemem.c", "movemem.asm"},
        {"xcptmisc.c", "xcptmisc.asm"},
        {"capture.c", "capture.asm"},
        {"debugstb.c", "debugstb.asm"},
        //
        // EX
        //
        {"hrderror.c", "hrderror.asm"},
        //
        // PO
        //
        {"xsum.c", "xsum.asm"},
        //
        // PS
        //
        {"psctxwrap.c", "psctxwrap.asm"}
    };
};
