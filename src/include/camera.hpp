/*
 * Orthographic Camera
 *
 * Based on javidx9's video on panning and zooming (https://www.youtube.com/watch?v=ZQ8qtAizis4)
 * and Joey De Vries' OpenGL tutorial on Cameras   (https://learnopengl.com/Getting-started/Camera)
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
public:
    // Used in Camera::move()
    enum Direction
    {
        Left,
        Right,
        Down,
        Up
    };

    /*
     *
     * Constructors
     *
     */

    // Vector constructor
    Camera(const glm::vec2 &_position,
           float _speed = 1.0f,
           float _zoom  = 1.0f)
           : position(_position),
             speed(_speed),
             zoom(_zoom) {}

    // Scalar constructor
    Camera(float posX,
           float posY,
           float _speed = 1.0f,
           float _zoom  = 1.0f)
            : position(posX, posY),
              speed(_speed),
              zoom(_zoom) {}

    /*
     *
     * Methods
     *
     */

    void move (Camera::Direction direction, float deltaTime);
    void updateProjectionMatrix (GLuint screenW, GLuint screenH);

    // Zooming
    inline void zoom_by (float yOffset);
           void zoom_on_position (float screenX, float screenY, float yOffset);
    inline void zoom_on_position (const glm::vec2& pos, float yOffset);

    // Panning
    inline void start_pan (float screenX, float screenY);
           void update_pan (float screenX, float screenY);
    inline void start_pan (const glm::vec2& screen);
    inline void update_pan (const glm::vec2& screen);

    // Coordinate system conversion
           glm::vec2 ScreenToWorld (float screenX, float screenY) const;
    inline glm::vec2 ScreenToWorld (const glm::vec2 &screen) const;
           glm::vec2 WorldToScreen (float worldX, float worldY) const;
    inline glm::vec2 WorldToScreen (const glm::vec2 &world) const;

    /*
     *
     * Getters
     *
     */

    inline float getZoom  () const;
    inline float getSpeed () const;

    inline const glm::vec2& getPosition         () const;
    inline const glm::mat4& getProjectionMatrix () const;

    /*
     *
     * Setters
     *
     */

    inline void setZoom  (float newZoom);
    inline void setSpeed (float newSpeed);

    inline void setPosition (float posX, float posY);
    inline void setPosition (const glm::vec2& newPosition);

private:
    float zoom;
    float speed;
    glm::mat4 projection = {};
    glm::vec2 position;

    glm::vec2 offset = {0.0f, 0.0f};
    glm::vec2 startPan = {0.0f, 0.0f};
};

// Panning
inline void Camera::start_pan  (float screenX, float screenY) { startPan = {screenX, screenY};  }
inline void Camera::start_pan  (const glm::vec2& screen)      { startPan = screen;              }
inline void Camera::update_pan (const glm::vec2& screen)      { update_pan(screen.x, screen.y); }

// Zooming
inline void Camera::zoom_by          (float yOffset)                       { zoom += yOffset;                         }
inline void Camera::zoom_on_position (const glm::vec2& pos, float yOffset) { zoom_on_position(pos.x, pos.y, yOffset); }

// Coordinate system conversion
inline glm::vec2 Camera::WorldToScreen (const glm::vec2 &world)  const { return WorldToScreen(world.x, world.y);   }
inline glm::vec2 Camera::ScreenToWorld (const glm::vec2 &screen) const { return ScreenToWorld(screen.x, screen.y); }

/*
 *
 * Getters
 *
 */

inline       float      Camera::getZoom             () const { return zoom;       }
inline       float      Camera::getSpeed            () const { return speed;      }
inline const glm::vec2& Camera::getPosition         () const { return position;   }
inline const glm::mat4& Camera::getProjectionMatrix () const { return projection; }

/*
 *
 * Setters
 *
 */

inline void Camera::setZoom     (float newZoom)                { zoom     = newZoom;      }
inline void Camera::setSpeed    (float newSpeed)               { speed    = newSpeed;     }
inline void Camera::setPosition (float posX, float posY)       { position = {posX, posY}; }
inline void Camera::setPosition (const glm::vec2& newPosition) { position = newPosition;  }

#endif /* CAMERA_H */
