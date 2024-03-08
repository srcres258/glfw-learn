//
// Created by srcres on 3/8/24.
//

#include "camera.h"

static glm::vec3 axisUp = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera()
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
    this->front = -cameraDirection;
    this->right = glm::normalize(glm::cross(axisUp, cameraDirection));
    // Since cameraDirection and cameraRight are both normalized, the result of this cross is normalized by default,
    // hence it needn't be normalized again.
    this->up = glm::cross(cameraDirection, this->right);

    this->pitch = 0.0f;
    this->yaw = 0.0f;
}

void Camera::moveFront(float amount)
{
    this->pos += amount * this->front;
}

void Camera::moveBack(float amount)
{
    this->pos -= amount * this->front;
}

void Camera::moveLeft(float amount)
{
    this->pos -= amount * this->right;
}

void Camera::moveRight(float amount)
{
    this->pos += amount * this->right;
}

void Camera::moveUp(float amount)
{
    this->pos += amount * this->up;
}

void Camera::moveDown(float amount)
{
    this->pos -= amount * this->up;
}

void Camera::changePitch(float amount)
{
    this->pitch += amount;
    if (this->pitch > 89.0f)
        this->pitch = 89.0f;
    if (this->pitch < -89.0f)
        this->pitch = -89.0f;
    this->syncFromEulerAngles();
}

void Camera::changeYaw(float amount)
{
    this->yaw += amount;
    this->syncFromEulerAngles();
}

void Camera::syncFromEulerAngles()
{
    glm::vec3 cameraFrontNew(1.0f);
    cameraFrontNew.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
    cameraFrontNew.y = sin(glm::radians(this->pitch));
    cameraFrontNew.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
    this->front = glm::normalize(cameraFrontNew);
    this->right = glm::normalize(glm::cross(this->front, axisUp));
    // Since cameraDirection and cameraRight are both normalized, the result of this cross is normalized by default,
    // hence it needn't be normalized again.
    this->up = glm::cross(this->right, this->front);
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}
