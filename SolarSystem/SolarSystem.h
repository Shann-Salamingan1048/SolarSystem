#pragma once
#include "global.h"
#include "Camera.h"
#include "frameRate.h"
#include "Shader.h"
#include "Texture.h"


class SolarSystem
{
public:
	SolarSystem();
	SolarSystem(int width, int height);
    SolarSystem(bool fullScreen);
    ~SolarSystem();
    void Run();

private:
    SolarSystem(const SolarSystem&) = delete;
    SolarSystem& operator=(const SolarSystem&) = delete;
    SolarSystem(SolarSystem&&) = delete;
    SolarSystem& operator=(SolarSystem&&) = delete;

private:
	void initGLFW();
    void glfwSets();
	void processInput(GLFWwindow* window);


private:
	GLFWwindow* m_window;
	int m_height, m_width;

private:
    void initSphere();
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;
    const float RADIUS = 1.0f;

private:
    // defaults
	constexpr static int m_defaultSize[2] = { 800, 800 }; // Width , Height
	constexpr static const char* m_defaultTitle = "Solar System";

private:
   bool firstMouse = true;
   float lastX;
   float lastY;
   void initLastXY();

private:
    Camera cam;
    frameRate fr;

private:
    // callbacks
    void framebuffer_size_callback(int width, int heigth);
    void scroll_callback(double xoffset, double yoffset);
    void mouse_callback(double xpos, double ypos);

private:
    // Static wrapper functions
    static void framebuffer_size_callback_wrapper(GLFWwindow* win, int width, int height);
    static void scroll_callback_wrapper(GLFWwindow* win, double xoffset, double yoffset);
    static void mouse_callback_wrapper(GLFWwindow* win, double xpos, double ypos);

private:
    void printMat4(const glm::mat4& mat, const std::string& name) const;
};
