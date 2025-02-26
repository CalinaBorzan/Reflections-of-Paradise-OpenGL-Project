#include "Camera.hpp"

namespace gps {

    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    void Camera::move(MOVE_DIRECTION direction, float speed) {
        switch (direction) {
        case MOVE_FORWARD:
            this->cameraPosition += this->cameraFrontDirection * speed;
            break;
        case MOVE_BACKWARD:
            this->cameraPosition -= this->cameraFrontDirection * speed;
            break;
        case MOVE_RIGHT:
            this->cameraPosition += this->cameraRightDirection * speed;
            break;
        case MOVE_LEFT:
            this->cameraPosition -= this->cameraRightDirection * speed;
            break;
        }
    }

    void Camera::moveUp(float speed) {
        this->cameraPosition += glm::vec3(0.0f, speed, 0.0f); // Move upward in the Y direction
    }

    void Camera::moveDown(float speed) {
        this->cameraPosition -= glm::vec3(0.0f, speed, 0.0f); // Move downward in the Y direction
    }


    void Camera::rotate(float pitch, float yaw) {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->cameraFrontDirection = glm::normalize(front);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));
        // Normalize the up direction if necessary (typically not needed unless roll is introduced)
        this->cameraUpDirection = glm::normalize(glm::cross(this->cameraRightDirection, this->cameraFrontDirection));
    }
}
