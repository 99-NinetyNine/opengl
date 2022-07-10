#pragma once
#include <vector>
#include <OGL3D/Math/Point.h>
#include <OGL3D/Math/OVec3.h>
#include <OGL3D/Math/OMat4.h>
#include <OGL3D/Math/MathUtils.h>
#include <OGL3D/OPrerequisities.h>


class Camera
{

public:
    //Camera(OVec3 x,OVec3 y);
    Camera();
    OMat4 GetViewMatrix();
    //OMat4 GetCameraMatrix();
    void ProcessKeyboard(CameraMovementDesc desc, f32 deltaTime);
    void ProcessMouseMovement(f32 xoffset, f32 yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(f32 yoffset);
    ~Camera();
public:
    void changePitchYaw(f32 p, f32 y);
private:
    void updateCameraVectors();

public:
    OVec3 Position = OVec3(0.0f, 0.0f, 3.0f);
    OVec3 Front = OVec3(-1.0f);//target..camera ley kaha herirakhya xa

    //OVec3 Up= OVec3(0.0456304736f, 0.480769873f, 0.875658691f);
    
    OVec3 Up = OVec3(0.0f, 1.0f, 0.0f); 
    OVec3 Right;
    OVec3 WorldUp;


    f32 Yaw = 0.0f;
    f32 Pitch = 0.0f;


    f32 MovementSpeed = 15.5f;
    f32 MouseSensitivity = 0.03f;
    f32 Zoom = 45.0f;

};

