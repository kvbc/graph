#include "../include/debug/ClassManager.hpp"
#include <string>

void ClassManager::ImGui_printLabel (const ImVec4& color, const char* label, const char* content)
{
    ImGui::TextColored(color, label);
    ImGui::SameLine();
    ImGui::Text(content);
}

bool ClassManager::ImGui_treeNode (const char* nodelabel, const char* type)
{
    std::string label = (std::string("[") + type) + "] " + std::string(nodelabel);
    return ImGui::TreeNode(label.c_str());
}
