//-----------------------------------------------------------------------------
// File:			Camera.cpp
// Original Author:	Ryan Campbell
//
// class to represent a camera to move around the world
//-----------------------------------------------------------------------------
#include "camera.h"

// constructor
Camera::Camera(int width, int height)
    : Node(glm::vec3(0, 5, 10), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 1.0f)
    , m_camFront(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_width(width)
    , m_height(height)
    , m_pApp((wolf::App*)(glfwGetWindowUserPointer(glfwGetCurrentContext())))
    , m_upDirection(WORLD_UP)
    , m_rightDirection(glm::vec3(0.0f, 0.0f, 1.0f))
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_cameraSens(0.1f)
    , m_mProj(glm::mat4(1.0f))
    , m_mView(glm::mat4(1.0f))
    , m_velocity(10.0f)
    , m_fov(60.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(500.0f)
{
    glfwSetInputMode(m_pApp->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Camera::Camera()
    : Node(glm::vec3(0, 5, 10), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 1.0f)
    , m_camFront(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_width(1280)
    , m_height(720)
    , m_pApp((wolf::App*)(glfwGetWindowUserPointer(glfwGetCurrentContext())))
    , m_upDirection(WORLD_UP)
    , m_rightDirection(glm::vec3(0.0f, 0.0f, 1.0f))
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_cameraSens(0.1f)
    , m_mProj(glm::mat4(1.0f))
    , m_mView(glm::mat4(1.0f))
    , m_velocity(10.0f)
    , m_fov(60.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(500.0f)
{
}

Camera::~Camera()
{
}

// screen center
float lastXPos = 640.0f;
float lastYPos = 360.0f;
bool shiftIsUp = true;
void Camera::update(float dt)
{
    Node::update(dt);
    // get scrollwheel input
    auto mouseScroll = m_pApp->getMouseScroll().y * 3;
    m_velocity = std::max(m_velocity + mouseScroll, 0.1f);

    updateCamAxisVectors();

    bool isSlowCamera = m_pApp->isKeyDown(GLFW_KEY_LEFT_SHIFT);
    float camMovement = dt * m_velocity / (isSlowCamera ? 2.0f : 1.0f);

    // get keyboard input
    if (m_pApp->isKeyDown('w'))
    {
        translate(m_camFront * camMovement);
    }
    if (m_pApp->isKeyDown('a'))
    {
        translate(-glm::normalize(glm::cross(m_camFront, m_upDirection)) * camMovement);
    }
    if (m_pApp->isKeyDown('s'))
    {
        translate(-m_camFront * camMovement);
    }
    if (m_pApp->isKeyDown('d'))
    {
        translate(glm::normalize(glm::cross(m_camFront, m_upDirection)) * camMovement);
    }
    if (m_pApp->isKeyDown('q'))
    {
        translate(glm::normalize(m_upDirection) * camMovement);
    }
    if (m_pApp->isKeyDown('e'))
    {
        translate(-glm::normalize(m_upDirection) * camMovement);
    }
    m_mView = glm::lookAt(getWorldPosition(), m_camFront + getWorldPosition(), m_upDirection);
    // fov,aspect ratio, near plane, far plane
    m_mProj = glm::perspective(glm::radians(m_fov), (float)m_width / (float)m_height, m_nearPlane, m_farPlane);
}

void Camera::setWidth(int width)
{
    m_width = width;
}

void Camera::setHeight(int height)
{
    m_height = height;
}

glm::mat4 Camera::getProjection() const
{
    return m_mProj;
}

glm::mat4 Camera::getView() const
{
    return m_mView;
}

float Camera::getFOV() const
{
    return m_fov;
}

float Camera::getFarPlane() const
{
    return m_nearPlane;
}

float Camera::getNearPlane() const
{
    return m_farPlane;
}

// in degrees
void Camera::setFOV(float fov)
{
    m_fov = fov;
}

void Camera::setNearPlane(float near)
{
    m_nearPlane = near;
}

void Camera::setFarPlane(float far)
{
    m_farPlane = far;
}

void Camera::updateBoundingVolume()
{
    getBoundingVolume().setPosition(getWorldPosition());
    getBoundingVolume().setRadius(calcRadius(getWorldScale()));
}

// todo
float Camera::calcRadius(const glm::vec3& scale)
{
    return 1.0f;
}

// https://stackoverflow.com/questions/13665932/calculating-the-viewing-frustum-in-a-3d-space
// https://stackoverflow.com/questions/12836967/extracting-view-frustum-planes-gribb-hartmann-method
Utility::Frustum Camera::getFrustum()
{
    float aspectRatio = (float)m_width / m_height;

    glm::vec3 nearCenter = getWorldPosition() + m_camFront * m_nearPlane;
    glm::vec3 farCenter = getWorldPosition() + m_camFront * m_farPlane;

    // vars needed for last 4 planes
    float halfNearHeight = tan(glm::radians(m_fov) / 2.0f) * m_nearPlane;
    float halfNearWidth = halfNearHeight * aspectRatio;
    float halfFarHeight = tan(glm::radians(m_fov) / 2.0f) * m_farPlane;
    float halfFarWidth = halfFarHeight * aspectRatio;

    // near
    glm::vec3 nearTopCenter = nearCenter + m_upDirection * halfNearHeight;
    glm::vec3 nearTopLeft = nearTopCenter - m_rightDirection * halfNearWidth;
    glm::vec3 nearTopRight = nearTopCenter + m_rightDirection * halfNearWidth;
    glm::vec3 nearBottomCenter = nearCenter - m_upDirection * halfNearHeight;
    glm::vec3 nearBottomRight = nearBottomCenter + m_rightDirection * halfNearWidth;
    glm::vec3 nearBottomLeft = nearBottomCenter - m_rightDirection * halfNearWidth;

    // far
    glm::vec3 farTopCenter = farCenter + m_upDirection * halfFarHeight;
    glm::vec3 farBottomCenter = farCenter - m_upDirection * halfFarHeight;
    glm::vec3 farTopLeft = farTopCenter - m_rightDirection * halfFarWidth;
    glm::vec3 farTopRight = farTopCenter + m_rightDirection * halfFarWidth;
    glm::vec3 farBottomLeft = farBottomCenter - m_rightDirection * halfFarWidth;
    glm::vec3 farBottomRight = farBottomCenter + m_rightDirection * halfFarWidth;

    return Utility::Frustum(
        nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight, farTopLeft, farTopRight, farBottomLeft,
        farBottomRight);
}

// updates the unit vectors the represent the axis of the camera
void Camera::updateCamAxisVectors()
{
    glm::vec2 mousePos = m_pApp->getMousePos();
    float xOffset = mousePos.x - lastXPos;
    float yOffset = lastYPos - mousePos.y;
    lastXPos = mousePos.x;
    lastYPos = mousePos.y;
    xOffset *= m_cameraSens;
    yOffset *= m_cameraSens;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_camFront = glm::normalize(front);
    // pretty sure this does what I think
    // std::cout << m_camFront.x << " " << m_camFront.y << " " << m_camFront.z << std::endl;
    m_rightDirection = glm::normalize(glm::cross(m_camFront, WORLD_UP));
    m_upDirection = glm::normalize(glm::cross(m_rightDirection, m_camFront));
}