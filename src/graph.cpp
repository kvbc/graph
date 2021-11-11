#include "include/graph.hpp"
#include <new>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/debug/ClassManager.hpp"
#include <iostream>
#include <iomanip>

/*
 *
 * Graph
 *
 */

// Scalar (base) constructor
Graph::Graph
(
    Shader& _shader,
    Shader& _graphShader,
    Shader& _glyphShader,
    const Camera& _camera,
    float posX, float posY,
    float szX, float szY,
    int _range, double _step
)
    : graphShader(_graphShader),
      glyphShader(_glyphShader),
      step(_step),
      axisX (
          _shader, _camera,
          0, 0,
          0, 0,
          &lineX
      ),
      axisY (
          _shader, _camera,
          0, 0,
          0, 0,
          &lineY
      ),
      Object(_shader, _camera, 0) // base class
{
    size = {szX, szY};
    position = {posX, posY};

    initializeAxes();
    generateLineContainers();

    parser.DefineVar("x", &X);
    parser.SetExpr("x^2");

    updateRange(_range);
    getContainer()->update_VAO();
    updateVertices();
}

void Graph::setAxisSize (float szX, float szY)
{
    size = {szX, szY};
    axisX.size.x = szX;
    axisY.size.y = szY;
    axisX.updateModel();
    axisY.updateModel();
}

void Graph::initializeAxes ()
{
    axisX.size = {size.x, 10.0f};
    axisX.position.x = position.x;
    axisX.position.y = position.y - axisX.size.y / 2.0f;
    axisX.updateModel();

    axisY.size = {10.0f, size.y};
    axisY.position.x = position.x - axisY.size.x / 2.0f;
    axisY.position.y = position.y;
    axisY.updateModel();
}

void Graph::generateLineContainers ()
{
    lineX.new_vertices(4);
    lineY.new_vertices(4);

    lineX.gen_VAO();
    lineY.gen_VAO();

    lineX.gen_VBO();
    lineY.gen_VBO();

    GLfloat* const lineX_vertices = lineX.getVertices();
    GLfloat* const lineY_vertices = lineY.getVertices();

    // {-1.0, 0.5}
    // { 1.0, 0.5}
    lineX_vertices[0] = -1.0f;
    lineX_vertices[1] =  0.5f;
    lineX_vertices[2] =  1.0f;
    lineX_vertices[3] =  0.5f;

    // {0.5, -1.0}
    // {0.5,  1.0}
    lineY_vertices[0] =  0.5f;
    lineY_vertices[1] = -1.0f;
    lineY_vertices[2] =  0.5f;
    lineY_vertices[3] =  1.0f;

    lineX.update_VAO();
    lineY.update_VAO();
}

void Graph::updateLineBuffer ()
{
    ::operator delete(lines);
    lines = reinterpret_cast<Object*>(::operator new(sizeof(Object) * lineCount));
    lineBuffSize = lineCount;

    for(int i = 0; i < lineCount; i += 2)
    {
        new(lines + i) Object
        (
            shader, camera,
            0, 0,
            0, 0,
            &lineY
        );
        new(lines + i + 1) Object
        (
            shader, camera,
            0, 0,
            0, 0,
            &lineX
        );
    }
}

void Graph::updateLines () // to be optimized
{
    float rng = -range;
    for(int i = 0; i < lineCount; i += 2)
    {
        if(rng == 0)
            rng++;

        lines[i].size = {10.0f, 1.0f};
        lines[i].position.x = position.x + (rng / range * size.y) - lines[i].size.x / 2.0f;
        lines[i].position.y = position.y;
        lines[i].updateModel();

        lines[i+1].size = {1.0f, 10.0f};
        lines[i+1].position.x = position.x;
        lines[i+1].position.y = position.y + (rng / range * size.y) - lines[i+1].size.y / 2.0f;
        lines[i+1].updateModel();

        rng++;
    }
}

void Graph::updateVertices ()
{
    Container* container = getContainer();
    GLfloat* const vertices = container->getVertices();
    GLsizei verticesCount = container->getVerticesCount();

    float xRatio =  (size.x / (float)range);
    float yRatio = -(size.y / (float)range);

    X = -range;
    for(int i = 0; i < verticesCount;)
    {
        vertices[i++] = position.x + X             * xRatio;
        vertices[i++] = position.y + parser.Eval() * yRatio;
        X += step;
    }

    container->update_VBO();
}

void Graph::updateRange (int _range)
{
    range = _range;

    lineCount = range * 2 * 2;
    if(lineCount > lineBuffSize)
        updateLineBuffer();

    pointsCount = 1 + (range * 2) / step;
    const int new_verticesCount = pointsCount * 2; /* x,y attributes */

    Container* container = getContainer();
    if(new_verticesCount > container->getVBOSize())
        container->new_vertices(new_verticesCount);

    updateLines();
}

void Graph::updateGlyphModel (float posX, float posY)
{
    glyphShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, {posX, posY, 0.0f});
    model = glm::scale(model, {0.05f, -0.05f, 0.0f});
    glyphShader.setUniformMatrix4("model", 1, GL_FALSE, glm::value_ptr(model));
}

void Graph::renderLines(const TextRenderer& textRenderer, GLuint fontID, float colorR, float colorG, float colorB, float alpha)
{
    for(int i = 0; i < lineCount; i += 2)
    {
        const Object& _lineX = lines[i];
        const Object& _lineY = lines[i+1];
        _lineX.drawArrays(GL_LINES, 2);
        _lineY.drawArrays(GL_LINES, 2);

        // text rendering (very inefficient)

        int stepIdx = i / 2 - range; // to be optimized
        if(stepIdx >= 0) stepIdx++;

        std::string num = std::to_string(stepIdx);
        updateGlyphModel(_lineX.position.x + _lineX.size.x / 2.0f, _lineX.position.y - _lineX.size.y * 2.0f);
        glm::vec2 text_size = textRenderer.fontTextSize(fontID, num, 0.5f, 0.5f);
        textRenderer.render(fontID, num, -text_size.x, 0.0f, 1.0f, 1.0f, colorR, colorG, colorB, alpha);

        num = std::to_string(-stepIdx);
        updateGlyphModel(_lineY.position.x + _lineY.size.x * 2.0f, _lineY.position.y + _lineY.size.y / 2.0f);
        text_size = textRenderer.fontTextSize(fontID, num, 0.5f, 0.5f);
        textRenderer.render(fontID, num, 0.0f, -text_size.y / 2.0f, 1.0f, 1.0f, colorR, colorG, colorB, alpha);
    }
}

void Graph::render (const TextRenderer& textRenderer, GLuint fontID, float colorR, float colorG, float colorB, float alpha)
{
    axisX.drawArrays(GL_LINES, 2);
    axisY.drawArrays(GL_LINES, 2);

    renderLines(textRenderer, fontID, colorR, colorG, colorB, alpha);

    graphShader.use();
    getContainer()->bind_VAO();
    glDrawArrays(GL_LINE_STRIP, 0, pointsCount);
}

void Graph::destroy ()
{
    Object::destroy();
    ::operator delete(lines);
}

/*
 *
 * ClassManager
 *
 */

void ClassManager::ImGui_printClassData (const char* nodelabel, const char* type, const Graph& graph)
{
    static const ImVec4 color = {1.0f, 0.5f, 0.0f, 1.0f};

    ImGui::PushID(&graph);

    if(ImGui_treeNode(nodelabel, type))
    {
        double step             = graph.getStep();
        int range               = graph.getRange();
        int lineCount           = graph.getLineCount();
        int pointsCount         = graph.getPointsCount();
        GLsizei lineBuffSize    = graph.getLineBuffSize();
        const Container& lineX  = graph.getLineX();
        const Container& lineY  = graph.getLineY();
        const Object& axisX     = graph.getAxisX();
        const Object& axisY     = graph.getAxisY();
        const Object* lines     = graph.getLines();

        std::string str_Xstep   = std::to_string(step);
        std::string str_range   = std::to_string(range);
        std::string str_steps   = std::to_string(lineCount) + " [MAX " + std::to_string(lineBuffSize) + "]";
        std::string str_fpoints = std::to_string(pointsCount);

        const Object& object = dynamic_cast<const Object&>(graph);
        ImGui_printClassData(object);

        ImGui_printClassData("lineX", &lineX);
        ImGui_printClassData("lineY", &lineY);

        ImGui_printClassData("axisX", axisX);
        ImGui_printClassData("axisY", axisY);

        ImGui::PushID(lines);
        if(ImGui_treeNode("lines", "Object*  "))
        {
            for(int i = 0; i < lineCount; i++)
            {
                ImGui::PushID(i);
                ImGui_printClassData(lines[i]);
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();

        ImGui_printClassData("Graph Shader", graph.getGraphShader());
        ImGui_printClassData("Glyph Shader", graph.getGlyphShader());

        ImGui_printLabel(color, "X-step", str_Xstep.c_str());
        ImGui_printLabel(color, "range",  str_range.c_str());
        ImGui_printLabel(color, "steps",  str_steps.c_str());
        ImGui_printLabel(color, "points", str_fpoints.c_str());

        ImGui::TreePop();
    }
    ImGui::PopID();
}
