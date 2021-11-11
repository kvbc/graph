#include <glad/glad.h>
#include <GL/gl.h>
#include "include/camera.hpp"
#include "include/debug/ClassManager.hpp"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
 *
 * Camera
 *
 */

// Update the projection matrix
void Camera::updateProjectionMatrix (GLuint screenW, GLuint screenH)
{
    glm::vec2 left_top     = ScreenToWorld(0, 0)             + position;
    glm::vec2 right_bottom = ScreenToWorld(screenW, screenH) + position;

    projection = glm::ortho(left_top.x, right_bottom.x, right_bottom.y, left_top.y, -1.0f, 1.0f);
}

/*
 *
 * Coordinate system conversion
 *
 */

// Transform world-space coordinates to screen-space
glm::vec2 Camera::WorldToScreen (float worldX, float worldY) const
{
    return glm::vec2
    (
        (worldX - offset.x) * zoom,
        (worldY - offset.y) * zoom
    );
}

// Transform screen-space coordinates to world-space
glm::vec2 Camera::ScreenToWorld (float screenX, float screenY) const
{
    return glm::vec2
    (
        (screenX / zoom) + offset.x,
        (screenY / zoom) + offset.y
    );
}

/*
 *
 * Panning
 *
 */

// Update camera pan
void Camera::update_pan (float screenX, float screenY)
{
    offset.x -= (screenX - startPan.x) / zoom;
    offset.y -= (screenY - startPan.y) / zoom;

    start_pan(screenX, screenY);
}

/*
 *
 * Zooming
 *
 */

// Zoom the camera on screen-space coordinates by the given offset
void Camera::zoom_on_position (float screenX, float screenY, float yOffset)
{
    glm::vec2 before_zoom = ScreenToWorld(screenX, screenY);

    zoom_by(yOffset);

    glm::vec2 after_zoom = ScreenToWorld(screenX, screenY);

    offset.x += (before_zoom.x - after_zoom.x);
    offset.y += (before_zoom.y - after_zoom.y);
}

/*
 *
 * Movement
 *
 */

// Move the camera in the specified direction, taking the speed and deltaTime into account
void Camera::move (Camera::Direction direction, float deltaTime)
{
    float velocity = speed * deltaTime;

    switch(direction)
    {
    case Camera::Direction::Left:
        position.x -= velocity;
        break;
    case Camera::Direction::Right:
        position.x += velocity;
        break;
    case Camera::Direction::Up:
        position.y -= velocity;
        break;
    case Camera::Direction::Down:
        position.y += velocity;
        break;
    }
}

/*
 *
 * ClassManager
 *
 */

void ClassManager::ImGui_printClassData (const char* nodelabel, const char* type, const Camera& camera)
{
    static const ImVec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    ImGui::PushID(&camera);

    if(ImGui_treeNode(nodelabel, type))
    {
        const glm::vec2& position = camera.getPosition();

        ImGui_printLabel(color, "X", std::to_string(position.x).c_str());
        ImGui_printLabel(color, "Y", std::to_string(position.y).c_str());
        ImGui_printLabel(color, "zoom", std::to_string(camera.getZoom()).c_str());

        ImGui::TreePop();
    }
    ImGui::PopID();
}
