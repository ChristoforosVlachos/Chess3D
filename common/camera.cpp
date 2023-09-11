#include <glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

Camera::Camera(GLFWwindow* window) : window(window)
{
    position = glm::vec3(3.0135, 6, 3.0135);
    horizontalAngle = 3.14f;
    verticalAngle = -0.79f;
    FoV = 45.0f;
    speed = 5.0f;
    mouseSpeed = 0.001f;
    fovSpeed = 2.0f;
    roll = 0.0f;
    peekingLeft = false;
    peekingRight = false;
    cameraLock = true;
    changeSide = false;
    requestViewFromBelow = false;
    allowLookAround = false;
}

void Camera::update()
{
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (allowLookAround) {
        // Reset mouse position for next frame
        if (height == 0) height = 1;
        glfwSetCursorPos(window, width / 2, height / 2);

        // Task 5.3: Compute new horizontal and vertical angles, given windows size
        //*/
        // and cursor position

        // Compensate for roll on mouse movement
        double hDistance = (width / 2 - xPos) * cos(glm::radians(roll)) - (height / 2 - yPos) * sin(glm::radians(roll));
        double vDistance = (width / 2 - xPos) * sin(glm::radians(roll)) + (height / 2 - yPos) * cos(glm::radians(roll));

        horizontalAngle += mouseSpeed * hDistance;
        //verticalAngle += mouseSpeed * vDistance;
        verticalAngle = min(max(verticalAngle + mouseSpeed * vDistance, radians(-90.0)), radians(90.0)); // clamp

        //horizontalAngle += mouseSpeed * float(width/2 - xPos) * cos(glm::radians(roll));
        //verticalAngle += mouseSpeed * float(height / 2 - yPos) * cos(glm::radians(roll));
    }

    if (changeSide) {
        horizontalAngle += (float) radians(180.);
        changeSide = false;
    }

    // Task 5.4: right and up vectors of the camera coordinate system
    // use spherical coordinates
    vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    // Right vector
    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    vec3 up = cross(right, direction);

    if (!cameraLock) {
        // Task 5.5: update camera position using the direction/right vectors
        // Move forward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position -= right * deltaTime * speed;
        }

        //// Task 5.6: handle zoom in/out effects
        //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        //    FoV -= fovSpeed * 0.01f;
        //    if (FoV < 1.0) FoV = 1.0f;
        //}
        //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        //    FoV += fovSpeed * 0.01f;
        //    if (FoV > 45.0) FoV = 45.0f;
        //}

        // Move up
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position += vec3(0, 1, 0) * deltaTime * speed;
        }
        // Move down
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !peekingLeft && !peekingRight) {
            position -= vec3(0, 1, 0) * deltaTime * speed;
        }

        // Peek left
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            if (!peekingLeft && !peekingRight) {
                roll -= 10.0;
                position -= right * deltaTime * (speed * 10);
                peekingLeft = true;
            }
        }
        // Stop peeking left
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
            if (peekingLeft) {
                roll = 0.0;
                position += right * deltaTime * (speed * 10);
                peekingLeft = false;
            }
        }
        // Peek right
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
            if (!peekingRight && !peekingLeft) {
                roll += 10.0;
                position += right * deltaTime * (speed * 10);
                peekingRight = true;
            }
        }
        // Stop peeking right
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
            if (peekingRight) {
                roll = 0.0;
                position -= right * deltaTime * (speed * 10);
                peekingRight = false;
            }
        }
    }

    glm::mat4 roll_mat = glm::rotate(glm::mat4(), (float)glm::radians(roll), direction);

    // Task 5.7: construct projection and view matrices
    if (height == 0) height = 1;
    projectionMatrix = perspective(FoV, static_cast<float>(width) / height, 0.1f, 100.0f);
    viewMatrix = lookAt(
        position,
        position + direction,
        glm::mat3(roll_mat) * up
    );
    //*/

    if (requestViewFromBelow) {

        direction = vec3(
            cos(-verticalAngle) * sin(horizontalAngle),
            sin(-verticalAngle),
            cos(-verticalAngle) * cos(horizontalAngle)
        );

        right = vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
        );

        up = cross(right, direction);

        vec3 invPos = vec3(position.x, -position.y, position.z);
        
        viewMatrix = lookAt(
            invPos,
            invPos + direction,
            glm::mat3(roll_mat) * up
        );

        requestViewFromBelow = false;
    }

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

void Camera::viewFromBelow() {
    requestViewFromBelow = true;
}