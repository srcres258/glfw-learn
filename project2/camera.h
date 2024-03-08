//
// Created by srcres on 3/8/24.
//

#ifndef GLFW_LEARN_CAMERA_H
#define GLFW_LEARN_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    float pitch; // unit: degree
    float yaw; // unit: degree

    Camera();

    void moveFront(float amount);
    void moveBack(float amount);
    void moveLeft(float amount);
    void moveRight(float amount);
    void moveUp(float amount);
    void moveDown(float amount);
    void changePitch(float amount);
    void changeYaw(float amount);
    void syncFromEulerAngles();
    glm::mat4 getViewMatrix();
};


#endif //GLFW_LEARN_CAMERA_H
