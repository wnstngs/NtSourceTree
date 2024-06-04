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
    const NODE &Node,
    std::ofstream &TxtFile,
    int Indent
)
{
    for (const auto &[Name, Child] : Node.Children) {
		
        for (int i = 0; i < Indent; ++i) {
            TxtFile << " ";
        }

        TxtFile << Name << "\n";
        TxtView(Child, 
				TxtFile, 
				Indent + 4);
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
}

void
SOURCE_TREE::BuildJsonHierarchy()
{
    for (const std::string &Path : SourceFiles_) {
        JSON_NODE Node = ContructJsonNode(Path);
        AddJsonNodeToJsonHierarchy(JsonHierarchy_, Node);
    }
}

SOURCE_TREE::JSON_NODE
SOURCE_TREE::ContructJsonNode(
    const std::string &Path
)
{
    JSON_NODE JsonNode;

    // Extract the file/directory name
    size_t Position = Path.find_last_of("/\\");
    JsonNode.Name = (Position != std::string::npos) ? Path.substr(Position + 1) : Path;

    // Check if it's a directory or a file
    if (std::filesystem::is_directory(Path)) {
        JsonNode.Type = "directory";

        // Recursively construct children nodes for directories
        for (const auto &entry : std::filesystem::directory_iterator(Path)) {
            if (entry.is_directory()) {
                JsonNode.Children.push_back(ContructJsonNode(entry.path().string()));
            }
        }
    } else {
        JsonNode.Type = "file";
    }

    return JsonNode;
}

void
SOURCE_TREE::AddJsonNodeToJsonHierarchy(
    nlohmann::json &Parent,
    const JSON_NODE &Node
)
{
    nlohmann::json JsonNode;
    JsonNode["Name"] = Node.Name;
    JsonNode["Type"] = Node.Type;

    if (Node.Type == "directory") {
        for (const auto &child : Node.Children) {
            nlohmann::json childJson;
            AddJsonNodeToJsonHierarchy(childJson, child);
            JsonNode["Children"].push_back(childJson);
        }
    }

    Parent.push_back(JsonNode);
}
