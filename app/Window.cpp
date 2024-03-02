#include "Window.h"

#include <cstdio>

Window::Window() {
  width = 800;
  height = 600;

  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
}

Window::Window(GLint windowWidth, GLint windowHeight) {
  width = windowWidth;
  height = windowHeight;
  for (size_t i = 0; i < 1024; i++) {
    keys[i] = 0;
  }
}

int Window::Initialise() {
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

  // keys and mouse inputs
  createCallbacks();

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

  glfwSetWindowUserPointer(mainWindow, this);

  return 0;
}

void Window::createCallbacks() { glfwSetKeyCallback(mainWindow, handleKeys); }

void Window::handleKeys(GLFWwindow* window, int key, int code, int action,
                        int mode) {
  Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      theWindow->keys[key] = true;
      printf("Pressed key: %d\n", key);
    } else if (action == GLFW_RELEASE) {
      theWindow->keys[key] = false;
      printf("Released key: %d\n", key);
    }
  }
}

Window::~Window() {
  glfwDestroyWindow(mainWindow);
  glfwTerminate();
}
