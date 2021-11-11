/*
 *
 * Graph
 *
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "object.hpp"
#include "camera.hpp"
#include "shader.hpp"
#include "line.hpp"
#include "textrenderer/textrenderer.hpp"
#include "muParser/muParser.h"
#include <glm/glm.hpp>
#include <imgui.h>


class Graph : public Object
{
public:
    // scalar constructor
    Graph(Shader& shader,
          Shader& _graphShader,
          Shader& _glyphShader,
          const Camera& camera,
          float posX, float posY,
          float szX, float szY,
          int _range, double _step);

    void destroy ();

    void updateLines ();
    void updateVertices ();
    void updateRange (int _range);
    void render (const TextRenderer& textRenderer, GLuint fontID, float colorR, float colorG, float colorB, float alpha);
    void render (const TextRenderer& textRenderer, GLuint fontID, const ImVec4& color);
    void render (const TextRenderer& textRenderer, GLuint fontID, const glm::vec4& color);


    /*
     *
     * Function
     *
     */

    inline void setFunction (const char* func);
    inline void testFunction ();

    /*
     *
     * Setters
     *
     */

           void setAxisSize (float szX, float szY);
    inline void setAxisSize (float sz);

    /*
     *
     * Getters
     *
     */

    inline       int        getRange        () const;
    inline       int        getLineCount    () const;
    inline       int        getPointsCount  () const;
    inline       double     getX            () const;
    inline       double     getStep         () const;
    inline       GLsizei    getLineBuffSize () const;
    inline const Shader&    getGraphShader  () const;
    inline const Shader&    getGlyphShader  () const;
    inline const Object*    getLines        () const;
    inline const Object&    getAxisX        () const;
    inline const Object&    getAxisY        () const;
    inline const Container& getLineX        () const;
    inline const Container& getLineY        () const;
    inline       glm::vec2 getAxisSize      () const;

private:
    void initializeAxes ();

    void generateLineContainers ();
    void updateLineBuffer ();
    void updateGlyphModel (float posX, float posY);

    void renderLines(const TextRenderer& textRenderer, GLuint fontID, float colorR, float colorG, float colorB, float alpha);

    Shader& graphShader;
    Shader& glyphShader;

    Container lineX;
    Container lineY;
    int lineCount;
    GLint lineBuffSize = -1;
    Object* lines = nullptr;

    double step;
    double X = 0.0;
    mu::Parser parser;

    int pointsCount;

    int range;
    Object axisX;
    Object axisY;
};

/*
 *
 * Overloads
 *
 */

inline void Graph::render (const TextRenderer& textRenderer, GLuint fontID, const ImVec4& color)    { renderLines(textRenderer, fontID, color.x, color.y, color.z, color.w); }
inline void Graph::render (const TextRenderer& textRenderer, GLuint fontID, const glm::vec4& color) { renderLines(textRenderer, fontID, color.x, color.y, color.z, color.w); }

/*
 *
 * Function
 *
 */

inline void Graph::testFunction ()                 { parser.Eval();        }
inline void Graph::setFunction  (const char* func) { parser.SetExpr(func); }

/*
 *
 * Setters
 *
 */

inline void Graph::setAxisSize (float sz)
{
    setAxisSize(sz, sz);
}

/*
 *
 * Getters
 *
 */

inline       int        Graph::getRange        () const { return range;        }
inline       int        Graph::getLineCount    () const { return lineCount;    }
inline       int        Graph::getPointsCount  () const { return pointsCount;  }
inline       double     Graph::getX            () const { return X;            }
inline       double     Graph::getStep         () const { return step;         }
inline       GLsizei    Graph::getLineBuffSize () const { return lineBuffSize; }
inline const Shader&    Graph::getGraphShader  () const { return graphShader;  }
inline const Shader&    Graph::getGlyphShader  () const { return glyphShader;  }
inline const Object*    Graph::getLines        () const { return lines;        }
inline const Object&    Graph::getAxisX        () const { return axisX;        }
inline const Object&    Graph::getAxisY        () const { return axisY;        }
inline const Container& Graph::getLineX        () const { return lineX;        }
inline const Container& Graph::getLineY        () const { return lineY;        }
inline       glm::vec2  Graph::getAxisSize     () const { return {axisX.size.x, axisY.size.y}; }

#endif /* GRAPH_H */
