#include "Window.h"

Window::Window(){
  width = 800;
  height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight){
  width = windowWidth;
  height = windowHeight;
}

int Window::Initialise(){
  // Initialize GLFW
  if (!glfwInit()) {
    glfwTerminate();
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a GLFW window
  mainWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!mainWindow) {
    glfwTerminate();
    return 1;
  }

  glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

  // Make the window's context current
  glfwMakeContextCurrent(mainWindow);

  // allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialize GLEW
  GLenum error = glewInit();
  if (error != GLEW_OK) {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 1;
  }

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, bufferWidth, bufferHeight);
 return 0;
}

Window::~Window(){
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
