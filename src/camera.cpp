#include "camera.h"

#include <math.h>
#include <iostream>

Camera::Camera(float& time, const Vector3& cameraPosition, const Vector3& cameraForward, const Vector3& cameraUpside) :
    movementSpeed(5.0f),
    mouseSensitivity(0.1f),
    first_mouse(true),
    deltaTime(time)
{
    cameraPos = cameraPosition;
    cameraFront = cameraForward;
    cameraUp = cameraUpside;
    yaw = YAW;
    pitch = PITCH;
    std::cout << "camera init" << std::endl;
    update_vectors();
}

Camera::~Camera() {

}

void Camera::moveForward() {
    cameraPos += cameraFront * (movementSpeed*deltaTime);
}

void Camera::moveBackward() {
    cameraPos -= cameraFront * (movementSpeed*deltaTime);
}

void Camera::moveRight() {
    cameraPos += cameraRight * (movementSpeed*deltaTime);
}

void Camera::moveLeft() {
    cameraPos -= cameraRight * (movementSpeed*deltaTime);
}

void Camera::cameraMouseInput(float xpos, float ypos, GLboolean constrainPitch) {

    if (first_mouse) {
        lastx = xpos;
        lasty = ypos;
        first_mouse = false;
    }

    float xoffset = (xpos - lastx)*mouseSensitivity;
    float yoffset = (lasty - ypos)*mouseSensitivity;
    lastx = xpos;
    lasty = ypos;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    //std::cout << "mouse handling // yaw = " << yaw << "; pitch = " << pitch << std::endl;

    update_vectors();
}

void Camera::update_vectors() {

    Vector3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, Vector3(0.0, 1.0, 0.0)));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

    //std::cout << cameraFront.x << ' ' << cameraFront.y << ' ' << cameraFront.z << std::endl;
    //std::cout << "updating vectors // yaw = " << yaw << "; pitch = " << pitch << std::endl;
}

glm::mat4 Camera::getView() {

    //std::cout << cameraFront.x << ' ' << cameraFront.y << ' ' << cameraFront.z << std::endl;
    //std::cout << "getting view // yaw = " << yaw << "; pitch = " << pitch << std::endl;
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);;
}