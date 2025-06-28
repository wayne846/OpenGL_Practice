#include "Camera.h"

#include <GLFW/glfw3.h>
#include "../Utilty/MathHelper.h"

Camera::Camera() {
    projection = glm::mat4(1.0);
    view = glm::mat4(1.0);

    pos = glm::vec3(2, 0, 0);
    dir = -pos;
    radius = glm::length(pos);
    SetAxesByDir();

    fov = 80.0f;
    aspect = 1280.0f / 720.0f;
    clipNear = 0.01f;
    clipFar = 1000.0f;

    sensitivity = 0.1f;
    flySpeed = 0.1f;

    for (int i = 0; i < 4; i++) {
        keys[i] = false;
    }

    UpdateProjectionMatrix();
}

Camera::~Camera() {

}

#pragma region Getter and Setter
glm::mat4 Camera::GetProjectionMatrix() const {
    return projection;
}

glm::mat4 Camera::GetViewMatrix() const {
    return view;
}

glm::vec3 Camera::GetPos() const {
    return pos;
}

glm::vec3 Camera::GetDir() const {
    return dir;
}

void Camera::SetFov(float degree) {
    fov = degree;

    UpdateProjectionMatrix();
}

void Camera::SetAspect(float ratio) {
    aspect = ratio;

    UpdateProjectionMatrix();
}

void Camera::SetClip(float near1, float far1) {
    clipNear = near1;
    clipFar = far1;

    UpdateProjectionMatrix();
}

void Camera::SetDir(glm::vec3 d) {
    dir = glm::normalize(d);
    SetAxesByDir();

    UpdateViewMatrix(pos, pos + dir, MathHelper::VEC3_UP);
}

void Camera::SetPos(glm::vec3 p) {
    pos = p;

    UpdateViewMatrix(pos, pos + dir, MathHelper::VEC3_UP);
}

/// <summary>
/// 將dir設為看向point
/// </summary>
void Camera::LookAt(glm::vec3 point) {
    SetDir(point - pos);
    radius = glm::length(point - pos);

}
#pragma endregion

#pragma region Function
/// <summary>
/// 更新相機
/// </summary>
void Camera::Update() {
    UpdateFly();

    UpdateViewMatrix(pos, pos + dir, MathHelper::VEC3_UP);
}

/// <summary>
/// 更新View Matrix
/// </summary>
void Camera::UpdateViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    view = glm::lookAt(eye, center, up);
}

/// <summary>
/// 更新Projection Matrix
/// </summary>
void Camera::UpdateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspect, clipNear, clipFar);
}

/// <summary>
/// 右鍵按住時wasd第一人稱飛行移動
/// </summary>
void Camera::UpdateFly() {
    glm::vec3 right = glm::normalize(glm::cross(dir, MathHelper::VEC3_UP)); // 右方向

    if (!isFly) {
        return;
    }

    if (keys[0]) { //w
        pos += dir * flySpeed;
    }
    if (keys[1]) { //a
        pos -= right * flySpeed;
    }
    if (keys[2]) { //s
        pos -= dir * flySpeed;
    }
    if (keys[3]) { //d
        pos += right * flySpeed;
    }
}

/// <summary>
/// 根據pitch和yaw重新計算dir
/// </summary>
void Camera::SetDirByAxes() {
    dir.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    dir.y = sin(glm::radians(pitch));
    dir.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    dir = glm::normalize(dir);
}

/// <summary>
/// 根據dir重新計算pitch和yaw
/// </summary>
void Camera::SetAxesByDir() {
    yaw = glm::degrees(atan2(dir.z, dir.x));
    pitch = glm::degrees(asin(dir.y));
}
#pragma endregion

#pragma region Event
/// <summary>
/// 鍵盤按鈕事件，紀錄按下的狀態
/// </summary>
void Camera::OnKeyboard(int key, int action) {
    bool isPress = (action == GLFW_PRESS || action == GLFW_REPEAT);
    switch (key) {
    case GLFW_KEY_W:
        keys[0] = isPress;
        break;

    case GLFW_KEY_A:
        keys[1] = isPress;
        break;

    case GLFW_KEY_S:
        keys[2] = isPress;
        break;

    case GLFW_KEY_D:
        keys[3] = isPress;
        break;
    }
}

/// <summary>
/// 滑鼠按鈕事件，左鍵旋轉，右鍵旋轉和飛行，中鍵平移
/// </summary>
void Camera::OnMouseButton(int button, int action) {
    if (action == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_1: //左鍵
            mode = Mode::Orbit;
            isFly = false;
            break;

        case GLFW_MOUSE_BUTTON_2: //右鍵
            mode = Mode::Rotate;
            isFly = true;
            break;

        case GLFW_MOUSE_BUTTON_3: //中鍵
            mode = Mode::Pan;
            isFly = false;
            break;

        default:
            mode = Mode::None;
            isFly = false;
            break;
        }
    }
    else {
        mode = Mode::None;
        isFly = false;
    }
}

/// <summary>
 /// 滑鼠移動事件
 /// </summary>
void Camera::OnMouseMove(double x, double y) {
    double deltaX = x - mouseX;
    double deltaY = y - mouseY;
    mouseX = x;
    mouseY = y;

    switch (mode) {
    case Mode::Rotate:
        yaw += deltaX * sensitivity;
        pitch += -deltaY * sensitivity;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        SetDirByAxes();
        break;

    case Mode::Orbit: {
        glm::vec3 target = pos + dir * radius;
        glm::vec3 offset = pos - target;
        float orbitPitch = glm::degrees(asin(offset.y / radius));
        float orbitYaw = glm::degrees(atan2(offset.z, offset.x));

        orbitYaw += deltaX * sensitivity;
        orbitPitch += deltaY * sensitivity;
        orbitPitch = glm::clamp(orbitPitch, -89.0f, 89.0f);
        
        float radPitch = glm::radians(orbitPitch);
        float radYaw = glm::radians(orbitYaw);
        pos.x = target.x + radius * cos(radPitch) * cos(radYaw);
        pos.y = target.y + radius * sin(radPitch);
        pos.z = target.z + radius * cos(radPitch) * sin(radYaw);

        dir = glm::normalize(target - pos);
        SetAxesByDir();
        break;
    }

    case Mode::Pan:
        glm::vec3 right = glm::normalize(glm::cross(dir, MathHelper::VEC3_UP)); // 右方向
        glm::vec3 up = glm::normalize(glm::cross(right, dir));      // 上方向，通常跟 worldUp 差不多

        pos += right * -(float)deltaX * sensitivity * 0.15f;
        pos += up * (float)deltaY * sensitivity * 0.15f;
        break;
    }
}

/// <summary>
/// 滑鼠滾輪滾動事件，前進和後退
/// </summary>
void Camera::OnScroll(double xoffset, double yoffset) {
    pos += dir * (float)yoffset * sensitivity * 5.0f;
    radius -= (float)yoffset * sensitivity * 5.0f;
}
#pragma endregion