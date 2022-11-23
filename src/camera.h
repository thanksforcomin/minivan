#pragma once

#include <iostream>

#include "renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

typedef glm::vec3 Vector3;
typedef glm::mat4 Matrix4;

#define YAW -90.0f;
#define PITCH 0.0f

class Camera {
private:
    Vector3 cameraPos;
    Vector3 cameraFront;
    Vector3 cameraRight;
    Vector3 cameraUp;

    bool first_mouse;

    float yaw, pitch;
    float& deltaTime;

    float lastx, lasty;

    float movementSpeed;
    float mouseSensitivity;
public:
	Camera(float& time, const Vector3& cameraPosition = Vector3(0.0, 0.0, 5.0), const Vector3& cameraForward = Vector3(0.0, 0.0, -1.0), const Vector3& cameraUpside = Vector3(0.0, 1.0, 0.0));
	~Camera();

    glm::mat4 getView();

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();

    void cameraMouseInput(float xpos, float ypos, GLboolean constrainPitch = true);
    void update_vectors();
    void __TEST_change_yaw(const float& test) { yaw = test; }

    glm::vec3& position() { return cameraPos; }
    glm::vec3& front() { return cameraFront; }
};