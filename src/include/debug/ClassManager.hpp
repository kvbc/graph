/*
 *
 * ClassManager
 *
 */

#ifndef DEBUG_CLASSMANAGER_H
#define DEBUG_CLASSMANAGER_H

#include "../shader.hpp"
#include "../camera.hpp"
#include "../graph.hpp"
#include "../object.hpp"
#include "../container.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>


class ClassManager
{
public:
    static void ImGui_printLabel (const ImVec4& color, const char* label, const char* content);

    #pragma region ClassManager_ImGui_printClassData_Declaration
    static inline void ImGui_printClassData (const Shader& shader);
    static inline void ImGui_printClassData (const Camera& camera);
    static inline void ImGui_printClassData (const Graph& graph);
    static inline void ImGui_printClassData (const Object& object);
    static inline void ImGui_printClassData (const Container* container);
    static inline void ImGui_printClassData (const char *nodelabel, const Shader& shader);
    static inline void ImGui_printClassData (const char *nodelabel, const Camera& camera);
    static inline void ImGui_printClassData (const char *nodelabel, const Graph& graph);
    static inline void ImGui_printClassData (const char *nodelabel, const Object& object);
    static inline void ImGui_printClassData (const char *nodelabel, const Container* container);
    static void ImGui_printClassData (const char* nodelabel, const char* type, const Shader& shader);
    static void ImGui_printClassData (const char* nodelabel, const char* type, const Camera& camera);
    static void ImGui_printClassData (const char* nodelabel, const char* type, const Graph& graph);
    static void ImGui_printClassData (const char* nodelabel, const char* type, const Object& object);
    static void ImGui_printClassData (const char* nodelabel, const char* type, const Container* container);
    #pragma endregion

private:
    static bool ImGui_treeNode (const char* nodelabel, const char* type);
};

#pragma region ClassManager_ImGui_printClassData_Definition
inline void ClassManager::ImGui_printClassData (const Shader&    shader)    { ImGui_printClassData("         ", "Shader",    shader);    }
inline void ClassManager::ImGui_printClassData (const Camera&    camera)    { ImGui_printClassData("         ", "Camera",    camera);    }
inline void ClassManager::ImGui_printClassData (const Graph&     graph)     { ImGui_printClassData("         ", "Graph",     graph);     }
inline void ClassManager::ImGui_printClassData (const Object&    object)    { ImGui_printClassData("         ", "Object",    object);    }
inline void ClassManager::ImGui_printClassData (const Container* container) { ImGui_printClassData("         ", "Container", container); }
inline void ClassManager::ImGui_printClassData (const char* nodelabel, const Shader&    shader)    { ImGui_printClassData(nodelabel, "Shader   ", shader);    }
inline void ClassManager::ImGui_printClassData (const char* nodelabel, const Camera&    camera)    { ImGui_printClassData(nodelabel, "Camera   ", camera);    }
inline void ClassManager::ImGui_printClassData (const char* nodelabel, const Graph&     graph)     { ImGui_printClassData(nodelabel, "Graph    ", graph);     }
inline void ClassManager::ImGui_printClassData (const char* nodelabel, const Object&    object)    { ImGui_printClassData(nodelabel, "Object   ", object);    }
inline void ClassManager::ImGui_printClassData (const char* nodelabel, const Container* container) { ImGui_printClassData(nodelabel, "Container", container); }
#pragma endregion


#endif /* DEBUG_CLASSMANAGER_H */
