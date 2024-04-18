#include "srctree.h"

#include <algorithm>
#include <fstream>

#include "util.h"

SOURCE_TREE::SOURCE_TREE(
    PDB_DUMP PdbDump
    )
    : PdbDump_(std::move(PdbDump))
{
    SourceFiles_ = PdbDump_.GetFiles();
}

void
SOURCE_TREE::HtmlView(
    const std::string &Path
    )
{
}

void
SOURCE_TREE::TxtView(
    const NODE &Node,
    std::ofstream &TxtFile,
    int Indent
    )
{
    for (const auto &[name, child] : Node.Children) {
        for (int i = 0; i < Indent; ++i) {
            TxtFile << " ";
        }
        TxtFile << name << "\n";
        TxtView(child, TxtFile, Indent + 4);
    }

}

void
SOURCE_TREE::CleanupSourceFiles()
{
    if (SourceFiles_.empty()) {
        return;
    }

    //
    // Keep only unique paths and sort.
    //
    auto unique = std::ranges::unique(SourceFiles_);
    SourceFiles_.erase(unique.begin(), unique.end());

    std::ranges::sort(unique);

    std::ranges::for_each(SourceFiles_,
                          [&](std::string &Str) {
                              //
                              // Removing unnecessary substrings.
                              //
                              for (const auto &substring : SubstringsToErase_) {
                                  size_t pos = 0;
                                  while ((pos = Str.find(substring, pos)) != std::string::npos) {
                                      Str.replace(pos, substring.length(), "");
                                      ++pos;
                                  }
                              }

                              //
                              // Replacing file extension substrings.
                              //
                              for (const auto &[target, replacement] : SubstringsToReplace_) {
                                  size_t pos = 0;
                                  while ((pos = Str.find(target, pos)) != std::string::npos) {
                                      Str.replace(pos, target.length(), replacement);
                                      pos += replacement.length();
                                  }
                              }

                              // TODO: Move to the substringsToReplace_ loop?
                              for (const auto &[target, replacement] : ExtensionsForReplacement_) {
                                  size_t pos = 0;
                                  while ((pos = Str.find(target, pos)) != std::string::npos) {
                                      Str.replace(pos, target.length(), replacement);
                                      pos += replacement.length();
                                  }
                              }
                          });
}

void
SOURCE_TREE::BuildHierarchy(
    NODE &Root,
    const std::vector<std::string> &Paths
    )
{
    for (const auto &path : Paths) {
        auto current = &Root;
        for (auto pathComponents = Util::SplitPath(path);
             const auto &component : pathComponents) {
            current = &current->Children[component];
        }
    }
}
