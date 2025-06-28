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
        None,   // ������
        Rotate, // ����
        Orbit,  // �y������
        Pan     // ����
    };

    float fov;
    float aspect;
    float clipNear;
    float clipFar;

    glm::vec3 pos;
    glm::vec3 dir;
    float yaw;   // ���k����
    float pitch; // �W�U����
    float radius;// �y������

    // �Ȧs�ƹ����y��
    double mouseX;
    double mouseY;

    float sensitivity; // �F�ӫ�
    float flySpeed;
    Mode mode;
    bool isFly;
    bool keys[4]; // w�Ba�Bs�Bd�O�_�Q���U
};

