#include <OGL3D/Graphics/Camera.h>

Camera::Camera()
{
    //Position = OVec3(-0.230092525f, -8.76520729f, 3.29040790f);
    Position = OVec3(0.0f, 0.0f, 5.0f);
    WorldUp = OVec3(0.0f, 1.0f, 0.0f);;//desc.up;
    updateCameraVectors();
           
}
OMat4 Camera::GetViewMatrix()
{
    OMat4 res;
    res.LookAtMatrix(Position, Position + Front, Up);
    return res;
    //operator ovrloading!!
    //
}

void Camera::ProcessKeyboard(CameraMovementDesc desc, f32 deltaTime)
{
    f32 velocity = MovementSpeed * deltaTime;
    if (desc == FORWARD)
        Position += Front * velocity;
    else if (desc == BACKWARD)
        Position -= Front * velocity;
    else if (desc == LEFT)
        Position -= Right * velocity;
    else if (desc == RIGHT)
        Position += Right * velocity;
    
    updateCameraVectors();
}
void Camera::ProcessMouseMovement(f32 xoffset, f32 yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity*1;
    yoffset *= MouseSensitivity*1;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
void Camera::ProcessMouseScroll(f32 yoffset)
{
    Zoom -= (f32)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
    //used in perspective matirx calculations
}
Camera::~Camera()
{
}
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    OVec3 front;
    front.m_x = cos(OMath::radians(Yaw)) * cos(OMath::radians(Pitch));
    front.m_y = sin(OMath::radians(Pitch));
    front.m_z = sin(OMath::radians(Yaw)) * cos(OMath::radians(Pitch));
    Front = OVec3::normalize(front);
    // also re-calculate the Right and Up vector
    Right = OVec3::normalize(OVec3::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = OVec3::normalize(OVec3::cross(Right, Front));
}

void Camera::changePitchYaw(f32 p, f32 y)
{
    Pitch = p;
    Yaw = y;
}
