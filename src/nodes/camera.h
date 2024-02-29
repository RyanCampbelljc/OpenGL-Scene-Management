//-----------------------------------------------------------------------------
// File:			Camera.h
// Original Author:	Ryan Campbell
//
// class to represent a camera to move around the world
// https://learnopengl.com/Getting-started/Camera
//-----------------------------------------------------------------------------
#pragma once
#include "../wolf/wolf.h"
#include "node.h"
#include "utility.h"
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera : public Node
{
public:
    //------------------------------------
    // PUBLIC INTERFACE
    //------------------------------------
    Camera(int width, int height);
    Camera();
    ~Camera();

    void update(float dt);
    void setWidth(int width);
    void setHeight(int height);
    glm::vec3 getCamPos() const;
    glm::mat4 getProjection() const;
    glm::mat4 getView() const;
    float getFOV() const;
    float getFarPlane() const;
    float getNearPlane() const;
    void setFOV(float fov);
    void setNearPlane(float near);
    void setFarPlane(float far);
    void updateBoundingVolume() override;
    float calcRadius(const glm::vec3& scale) override;
    Utility::Frustum getFrustum();

    //-------------------------------------
private:
    //------------------------------------
    // PRIVATE INTERFACE
    //------------------------------------
    wolf::App* m_pApp;
    int m_width;
    int m_height;
    glm::vec3 m_camFront;
    glm::vec3 m_upDirection;
    glm::vec3 m_rightDirection;
    float m_yaw;
    float m_pitch;
    float m_cameraSens;
    glm::mat4 m_mProj;
    glm::mat4 m_mView;
    float m_velocity;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;

    void updateCamAxisVectors();
    //---------------------------------------
};
