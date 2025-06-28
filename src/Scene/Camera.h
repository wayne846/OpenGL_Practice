#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:
    Camera();
    ~Camera();

    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewMatrix() const;
    glm::vec3 GetPos() const;
    glm::vec3 GetDir() const;

    void SetFov(float degree);
    void SetAspect(float ratio);
    void SetClip(float near1, float far1);
    void SetDir(glm::vec3 d);
    void SetPos(glm::vec3 p);
    void LookAt(glm::vec3 point);

    void OnKeyboard(int key, int action);
    void OnMouseButton(int button, int action);
    void OnMouseMove(double x, double y);
    void OnScroll(double xoffset, double yoffset);

    void Update();

private:
    void UpdateViewMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void UpdateProjectionMatrix();
    void UpdateFly();

    void SetDirByAxes();
    void SetAxesByDir();

private:
    glm::mat4 projection;
    glm::mat4 view;

    enum class Mode {
        None,   // 不移動
        Rotate, // 旋轉
        Orbit,  // 球面旋轉
        Pan     // 平移
    };

    float fov;
    float aspect;
    float clipNear;
    float clipFar;

    glm::vec3 pos;
    glm::vec3 dir;
    float yaw;   // 左右旋轉
    float pitch; // 上下旋轉
    float radius;// 球面旋轉

    // 暫存滑鼠的座標
    double mouseX;
    double mouseY;

    float sensitivity; // 靈敏度
    float flySpeed;
    Mode mode;
    bool isFly;
    bool keys[4]; // w、a、s、d是否被按下
};

