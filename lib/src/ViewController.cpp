#include <glmlv/ViewController.hpp>

#include <glmlv/glfw.hpp>

using namespace glm;

namespace glmlv 
{

bool ViewController::update(float elapsedTime) 
{

    auto m_FrontVector = -vec3(m_RcpViewMatrix[2]);
    auto m_LeftVector = -vec3(m_RcpViewMatrix[0]);
    auto m_UpVector = vec3(m_RcpViewMatrix[1]);
    auto position = vec3(m_RcpViewMatrix[3]);

    bool hasMoved = false;
    vec3 localTranslationVector(0.f);

    float lateralAngleDelta = 0.f;

    if (glfwGetKey(m_pWindow, GLFW_KEY_W)) {
        localTranslationVector += m_fSpeed * elapsedTime * m_FrontVector;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_A)) {
        localTranslationVector += m_fSpeed * elapsedTime * m_LeftVector;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_Q)) {
        lateralAngleDelta += 0.001f;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_E)) {
        lateralAngleDelta -= 0.001f;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_S)) {
        localTranslationVector -= m_fSpeed * elapsedTime * m_FrontVector;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_D)) {
        localTranslationVector -= m_fSpeed * elapsedTime * m_LeftVector;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_UP)) {
        localTranslationVector += m_fSpeed * elapsedTime * m_UpVector;
    }

    if (glfwGetKey(m_pWindow, GLFW_KEY_DOWN)) {
        localTranslationVector -= m_fSpeed * elapsedTime * m_UpVector;
    }

    position += localTranslationVector;

    if (localTranslationVector != vec3(0.f)) {
        hasMoved = true;
    }

    if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) && !m_LeftButtonPressed) {
        m_LeftButtonPressed = true;
        glfwGetCursorPos(m_pWindow, &m_LastCursorPosition.x, &m_LastCursorPosition.y);
    }
    else if (!glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_LEFT) && m_LeftButtonPressed) {
        m_LeftButtonPressed = false;
    }

    auto newRcpViewMatrix = m_RcpViewMatrix;

    if (lateralAngleDelta) {
        newRcpViewMatrix = rotate(newRcpViewMatrix, lateralAngleDelta, vec3(0, 0, 1));

        hasMoved = true;
    }

    if (m_LeftButtonPressed) {
        dvec2 cursorPosition;
        glfwGetCursorPos(m_pWindow, &cursorPosition.x, &cursorPosition.y);
        dvec2 delta = cursorPosition - m_LastCursorPosition;

        m_LastCursorPosition = cursorPosition;

        if (delta.x || delta.y) {
            newRcpViewMatrix = rotate(newRcpViewMatrix, -0.01f * float(delta.x), vec3(0, 1, 0));
            newRcpViewMatrix = rotate(newRcpViewMatrix, -0.01f * float(delta.y), vec3(1, 0, 0));

            hasMoved = true;
        }
    }

    m_FrontVector = -vec3(newRcpViewMatrix[2]);
    m_LeftVector = -vec3(newRcpViewMatrix[0]);
    m_UpVector = cross(m_FrontVector, m_LeftVector);

    if (hasMoved) {
        setViewMatrix(lookAt(position, position + m_FrontVector, m_UpVector));
    }

    return hasMoved;
}

}