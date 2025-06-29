#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window/ControlWindow.h"
#include "Scene/Scene.h"

class App
{
public:
    App();
    ~App();

    auto Initialize() -> bool;
    void Loop();
    void Terminate();

private:
    void Update(double dt);
    void Render();

private:
    GLFWwindow* mainWindow;
    glm::vec2 mainWindowDefautSize = glm::vec2(1280, 720);

    ControlWindow* controlWindow;
    bool showControlWindow;

    Scene* currentScene;

    double timeNow = 0;
    double timeLast = 0;
    double timeDelta = 0;

public:
    Scene* GetCurrentScene() const
    {
        return currentScene;
    }
};


