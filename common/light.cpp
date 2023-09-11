#include <glfw3.h>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include "light.h"

using namespace glm;

Light::Light(GLFWwindow* window, 
             glm::vec4 init_La,
             glm::vec4 init_Ld,
             glm::vec4 init_Ls,
             glm::vec3 init_position,
             float init_power) : window(window) {
    La = init_La;
    Ld = init_Ld;
    Ls = init_Ls;
    power = init_power;
    lightPosition_worldspace = init_position;

    // setting near and far plane affects the detail of the shadow
    nearPlane = 0.1;
    farPlane = 100.0;

    direction = normalize(targetPosition - lightPosition_worldspace);

    lightSpeed = 5.0f;
    targetPosition = glm::vec3(3.0135, 0.0, -3.0135);

    projectionMatrix = perspective(radians(90.0f), 1.0f, nearPlane, farPlane);
    allowMovement = false;
}



void Light::update() {
    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);

    if (allowMovement) {
        // Move across z-axis
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            lightPosition_worldspace += deltaTime * lightSpeed * vec3(0.0, 0.0, 1.0);
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            lightPosition_worldspace -= deltaTime * lightSpeed * vec3(0.0, 0.0, 1.0);
        }
        // Move across x-axis
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            lightPosition_worldspace += deltaTime * lightSpeed * vec3(1.0, 0.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            lightPosition_worldspace -= deltaTime * lightSpeed * vec3(1.0, 0.0, 0.0);
        }
        // Move across y-axis
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            lightPosition_worldspace += deltaTime * lightSpeed * vec3(0.0, 1.0, 0.0);
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            lightPosition_worldspace -= deltaTime * lightSpeed * vec3(0.0, 1.0, 0.0);
        }
    }


    // We have the direction of the light and the point where the light is looking at
    // We will use this information to calculate the "up" vector, 
    // just like we did with the camera

    direction = normalize(targetPosition - lightPosition_worldspace);


    // converting direction to cylidrical coordinates
    float x = direction.x;
    float y = direction.y;
    float z = direction.z;

    // We don't need to calculate the vertical angle
    
    float horizontalAngle;
    if (z > 0.0) horizontalAngle = atan(x/z);
    else if (z < 0.0) horizontalAngle = atan(x/z) + 3.1415f;
    else horizontalAngle = 3.1415f / 2.0f;

    // Right vector
    vec3 right(
        sin(horizontalAngle - 3.14f / 2.0f),
        0,
        cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    vec3 up = cross(right, direction);

    shadowTransforms.clear();
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(projectionMatrix * lookAt(lightPosition_worldspace, lightPosition_worldspace + vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f)));
   
    viewMatrix = lookAt(
        lightPosition_worldspace,
        targetPosition,
        up 
    );
    //*/

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}