#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader.hpp"
#include "include/camera.hpp"
#include "include/quadliteral.hpp"
#include "include/textrenderer/textrenderer.hpp"
#include "include/line.hpp"
#include "include/muParser/muParser.h"
#include "include/graph.hpp"
#include "include/debug/ClassManager.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_opengl3_loader.h>

#include <new>


constexpr float PI_F = 3.14159265358979f;

#define SCR_WIDTH  1920
#define SCR_HEIGHT 1080
#define WINDOW_NAME "Graph"
float screenWidth = SCR_WIDTH;
float screenHeight = SCR_HEIGHT;

// GLFW callbacks
void framebuffer_size_callback (GLFWwindow* window, int width, int height);
void key_callback              (GLFWwindow *window, int key, int scancode, int action, int mods);
void scroll_callback           (GLFWwindow* window, double xOffset, double yOffset);
void mouse_button_callback     (GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback  (GLFWwindow* window, double xpos, double ypos);

float deltaTime;
Camera camera(-screenWidth / 2.0f, -screenHeight / 2.0f, 100.0f, 1.0f);

void glfwErrorCallback (int err, const char* desc)
{
    std::cout << "GLFW Error (" << err << "): " << desc << std::endl;
}

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else
    {
             if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.move(Camera::Direction::Up,    deltaTime);
        else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.move(Camera::Direction::Down,  deltaTime);
             if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.move(Camera::Direction::Left,  deltaTime);
        else if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.move(Camera::Direction::Right, deltaTime);
    }
}

#pragma region Graph_Main
int main ()
{
    /*
     *
     * Initializing GLFW
     *
     */

    glfwSetErrorCallback(glfwErrorCallback);

    if(!glfwInit())
    {
        std::cout << "Failed to initalize GLFW" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*
     *
     * Creating GLFW Context
     *
     */

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync ON

    /*
     *
     * Initializing GLAD
     *
     */

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /*
     *
     * Initializing ImGui
     *
     */

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150 core");

    ImGui::StyleColorsDark();

    /*
     *
     * Printing debug info
     *
     */

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "GLFW version: " << glfwGetVersionString() << std::endl;
    std::cout << "ImGui version: " << ImGui::GetVersion() << std::endl;

    /*
     *
     * Setting up GLFW callbacks
     *
     */

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /*
     *
     * Initializing OpenGL
     *
     */
    
    glViewport(0, 0, screenWidth, screenHeight);

    /*
     *
     * Initializing shaders
     *
     */

    Shader shader("shaders/vs.glsl", "shaders/fs.glsl");
    Shader glyph_shader("shaders/glyph.vs", "shaders/glyph.fs");
    Shader scaled_glyph_shader("shaders/scaledglyph.vs", "shaders/glyph.fs");
    Shader graph_shader("shaders/graph.vs", "shaders/graph.fs");

    shader.use();
    shader.setUniform("color", 1.0f, 0.0f, 0.0f);

    graph_shader.use();
    graph_shader.setUniform("color", 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
    glyph_shader.use();
    glyph_shader.setUniformMatrix4("projection", 1, GL_FALSE, glm::value_ptr(projection));

    /*
     *
     * Setting up text renderers
     *
     */

    TextRenderer text(glyph_shader);
    if(text.anyError()) return -1;
    GLuint font_arial = text.loadFont("fonts/arial.ttf");
    text.done();

    TextRenderer scaled_text(scaled_glyph_shader);
    if(scaled_text.anyError()) return -1;
    GLuint scaled_font_arial = scaled_text.loadFont("fonts/arial.ttf");
    scaled_text.done();

    /*
     *
     * Initalizing program data
     *
     */

    // debug
    double startTime = glfwGetTime(); // the time at which the program has started

    // updated per frame
    float currentTime;       // the time at which the frame has started
    float lastFrameTime;     // the time at which the last frame has started
    unsigned int frames = 0; // frames rendered in one second

    // updated per second
    double ms = 0.0;            // milliseconds between frames
    unsigned int fps = 0;       // frames rendered in one second
    std::string str_fps;        // print-ready FPS buffer
    std::string str_ms;         // print-ready MS/F buffer
    float secondsPassed = 0.0f; // seconds passed from the start of the program

    // graph color buffers
    float color_axis     [3] = {1.0f, 0.0f, 0.0f};          // red
    float color_function [3] = {1.0f, 1.0f, 1.0f};          // white
    float color_glyph    [4] = {0.0f, 1.0f, 0.0f, 1.0f};    // green

    /*
     *
     * Initalizing scene data
     *
     */

    Graph graph
    (
        shader, graph_shader, scaled_glyph_shader,
        camera,
        0.0f, 0.0f,
        100.0f, 100.0f,
        10,
        PI_F / 100.0f
    );

    GLuint uboProjection;
    glGenBuffers(1, &uboProjection);
    glBindBuffer(GL_UNIFORM_BUFFER, uboProjection);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboProjection, 0, sizeof(glm::mat4));

    /*
     *
     * Main loop
     *
     */

    while(!glfwWindowShouldClose(window))
    {
        /*
         *
         * Updating the pre-frame debug data
         *
         */

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        frames++;
        if(currentTime - secondsPassed >= 1.0f)
        {
            fps = frames;
            str_fps = std::to_string(fps);

            ms = 1000.0 / double(frames);
            str_ms = std::to_string(ms);

            frames = 0;
            secondsPassed += 1.0f;
        }

        /*
         *
         * New frame
         *
         */

        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // New ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /*
         *
         * Scene
         *
         */

        camera.updateProjectionMatrix(screenWidth, screenHeight);
        glBindBuffer(GL_UNIFORM_BUFFER, uboProjection);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera.getProjectionMatrix()));

        graph.render(scaled_text, scaled_font_arial, color_glyph[0], color_glyph[1], color_glyph[2], color_glyph[3]);

        /*
         *
         * Debug Window
         *
         */

#ifdef GRAPH_DEBUG
        ImGui::Begin("Debug");
        ClassManager::ImGui_printClassData(graph);
        ImGui::Text(("Average " + str_ms + " ms/frame (" + str_fps + " FPS)").c_str());
        ImGui::Text(("Time elapsed: " + std::to_string(glfwGetTime() - startTime) + 's').c_str());
        ImGui::End();
#endif

        /*
         *
         * Graph Window
         *
         */

        #pragma region ImGui_Window_Graph
        ImGui::Begin("Graph");

        static char size[10] = "100";
        ImGui::Text("Size    ");
        ImGui::SameLine();
        ImGui::InputText("##size", size, IM_ARRAYSIZE(size));

        static char range_str[10] = "10";
        ImGui::Text("Range   ");
        ImGui::SameLine();
        ImGui::InputText("##range", range_str, IM_ARRAYSIZE(range_str));

        static char func[128] = "x^2";
        ImGui::Text("Function");
        ImGui::SameLine();
        ImGui::InputText("##function", func, IM_ARRAYSIZE(func));

        if(ImGui::Button("Submit"))
        {
            bool parsererr = false;
            try
            {
                graph.setFunction(func);
                graph.testFunction();
            }
            catch (mu::Parser::exception_type &e)
        	{
        		std::cout << "[MuParser] ERROR (" << e.GetCode() << "): " << e.GetMsg() << std::endl;
                parsererr = true;
        	}

            if(!parsererr)
            {
                int new_range = atoi(range_str);
                if(new_range <= 0)
                {
                    std::cout << "ERROR: invalid range" << std::endl;
                }
                else if(new_range != graph.getRange())
                {
                    graph.updateRange(new_range);
                }

                float new_size = atof(size);
                if(new_size <= 0.0f)
                {
                    std::cout << "ERROR: invalid size" << std::endl;
                }
                else if(new_size != graph.getAxisSize().x) // both axes are the same size
                {
                    graph.setAxisSize(new_size);
                    graph.updateLines();
                }

                graph.updateVertices();
            }
        }

        ImGui::Text("Function");
        ImGui::SameLine();
        if(ImGui::ColorEdit3("Function Color", color_function, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
        {
            graph_shader.use();
            graph_shader.setUniform("color", color_function[0], color_function[1], color_function[2]);
        }

        ImGui::Text("Axis    ");
        ImGui::SameLine();
        if(ImGui::ColorEdit3("Axis Color", color_axis, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel))
        {
            shader.use();
            shader.setUniform("color", color_axis[0], color_axis[1], color_axis[2]);
        }

        ImGui::Text("Glyph   ");
        ImGui::SameLine();
        ImGui::ColorEdit4("Glyph Color", color_glyph, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

        ImGui::End();
        #pragma endregion

        /*
         *
         * Render ImGui frame
         *
         */

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /*
         *
         * Debug
         *
         */

        text.render(font_arial, str_fps, 5.0f, 5.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);

        /*
         *
         * End of frame
         *
         */

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*
     *
     * Destroying the objects
     *
     */

    graph.destroy();

    shader.destroy();
    glyph_shader.destroy();
    scaled_glyph_shader.destroy();
    graph_shader.destroy();

    /*
     *
     * Shutting down ImGui
     *
     */

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /*
     *
     * Debug
     *
     */

    double endTime = glfwGetTime();       // the time at which the program has ended
    double runTime = endTime - startTime; // the time that the program has been running
    std::cout << "The program has been running for " << runTime << " seconds" << std::endl;

    /*
     *
     * End of the program
     *
     */

    glfwTerminate();
    return 0;
}
#pragma endregion

/*
 *
 * GLFW Callbacks
 *
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void scroll_callback (GLFWwindow* window, double xOffset, double yOffset)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    camera.zoom_on_position(xpos, ypos, yOffset / 10.0f);
}

void key_callback (GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE)
    {
        if(action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }
    }
    else
    {
        // pass the data to ImGui's key callback
        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
    }
}

bool pressed = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(!ImGui::GetIO().WantCaptureMouse)
        {
            if(action == GLFW_PRESS)
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                camera.start_pan(xpos, ypos);
                pressed = true;
            }
            else if(action == GLFW_RELEASE)
            {
                pressed = false;
            }
        }
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(!pressed)
        return;

    camera.update_pan(xpos, ypos);
}
