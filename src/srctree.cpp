#include "srctree.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "util.h"

SOURCE_TREE::SOURCE_TREE(
    PDB_DUMP PdbDump
)
    : PdbDump_(std::move(PdbDump))
{
    SourceFiles_ = PdbDump_.GetFiles();
    CleanupSourceFiles();

    std::cout << "\n\n\t\tAfter CleanupSourceFiles:\n\n";

    for (const auto &i : SourceFiles_) {
        std::cout << i << '\n';
    }
}

nlohmann::json &
SOURCE_TREE::JsonView()
{
    //
    // Construct the JSON hierarchy object.
    //
    BuildJsonHierarchy();

    return JsonHierarchy_;
}

void
SOURCE_TREE::HtmlView(
    const std::string &Path
)
{
}

void
SOURCE_TREE::TxtView(
    const nlohmann::json &Node,
    const std::string &FileName,
    const std::string &Indent
)
{
    std::ofstream File(FileName, std::ios_base::trunc);
    File << Indent << Node["Name"];
    if (Node["Type"] == "Directory") {
        File << "\\";
    }
    File << "\n";
    if (Node["Type"] == "Directory") {
        for (const auto &Child : Node["Children"]) {
            TxtView(Child, FileName, Indent + "    ");
        }
    }
}

void
SOURCE_TREE::CleanupSourceFiles()
{
    if (SourceFiles_.empty()) {
        return;
    }

    //
    // Keep only unique paths.
    //
    auto unique = std::ranges::unique(SourceFiles_);
    SourceFiles_.erase(unique.begin(), unique.end());
    
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

                              //
                              // Fixing file extensions for the known files.
                              //
                              for (const auto &[target, replacement] : ExtensionsForReplacement_) {
                                  size_t pos = 0;
                                  while ((pos = Str.find(target, pos)) != std::string::npos) {
                                      Str.replace(pos, target.length(), replacement);
                                      pos += replacement.length();
                                  }
                              }
                          });

    //
    // ...And sort.
    //
    std::ranges::sort(unique);
}

void
SOURCE_TREE::BuildJsonHierarchy()
{
    JsonHierarchy_["Name"] = "root";
    JsonHierarchy_["Type"] = "Directory";
    JsonHierarchy_["Children"] = nlohmann::json::array();
    for (const auto &i : SourceFiles_) {
        std::filesystem::path Path(i);
        ContructJsonNode(JsonHierarchy_, Path);
    }

}

void
SOURCE_TREE::ContructJsonNode(
    nlohmann::json &Root,
    const std::filesystem::path &Path
)
{
    nlohmann::json *Node = &Root;
    for (auto Iter = Path.begin(); Iter != Path.end(); ++Iter) {
        bool Found = false;
        
        for (auto &Child : (*Node)["Children"]) {
            if (Child["Name"] == Iter->string()) {
                Node = &Child;
                Found = true;
                break;
            }
        }
        
        if (!Found) {
            nlohmann::json NewNode;
            NewNode["Name"] = Iter->string();
            NewNode["Type"] = (std::next(Iter) == Path.end() && Iter->has_extension())
                                  ? "File"
                                  : "Directory";
            NewNode["Children"] = nlohmann::json::array();
            (*Node)["Children"].insert((*Node)["Children"].begin(), NewNode);
            Node = &(*Node)["Children"].at((*Node)["Children"].size() - 1);
        }
    }
}
