#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cmath>
#include <cstdio>
#include <cstring>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.01f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

// vertex shader
static const char* vShader =
    "                                                                       \n\
#version 330                                                            \n\
                                                                        \n\
layout (location = 0) in vec3 pos;                                      \n\
                                                                        \n\
out vec4 vColour;                                                       \n\
                                                                        \n\
uniform mat4 model;                                                     \n\
uniform mat4 projection;                                                                        \n\
                                                                        \n\
void main()                                                             \n\
{                                                                       \n\
  gl_Position = projection * model * vec4(pos, 1.0);                    \n\
  vColour = vec4(clamp(pos, 0.0f, 1.0f), 1.0);                         \n\
}";

// Fragment Shader
static const char* fShader =
    "                                                           \n\
#version 330                                                \n\
                                                            \n\
in vec4 vColour;                                            \n\
out vec4 colour;                                            \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
  colour = vColour;                                          \n\
}";

void CreateTriangle() {
  unsigned int indices[]{
      0, 3, 1,  //
      1, 3, 2,  //
      2, 3, 0,  //
      0, 1, 2,  //
  };

  GLfloat vertices[] = {
      -1.0f, -1.0,  0.0f,  //
      0.0f,  -1.0f, 1.0f,  //
      1.0f,  -1.0f, 0.0f,  //
      0.0f,  1.0f,  0.0f   //
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);  // unbind

  glBindVertexArray(0);  // unbind
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
  GLuint theShader = glCreateShader(shaderType);

  const GLchar* theCode[1];
  theCode[0] = shaderCode;

  GLint codeLength[1];
  codeLength[0] = strlen(shaderCode);

  glShaderSource(theShader, 1, theCode, codeLength);
  glCompileShader(theShader);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if (!result) {
    glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
    printf("Error compiling the %d shader:  '%s'\n", shaderType, eLog);
    return;
  }

  glAttachShader(theProgram, theShader);
}

void CompileShaders() {
  shader = glCreateProgram();
  if (!shader) {
    printf("Error initialising GLFW");
    return;
  }

  AddShader(shader, vShader, GL_VERTEX_SHADER);
  AddShader(shader, fShader, GL_FRAGMENT_SHADER);

  GLint result = 0;
  GLchar eLog[1024] = {0};

  glLinkProgram(shader);
  glGetProgramiv(shader, GL_LINK_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error linking program: '%s'\n", eLog);
    return;
  }

  glValidateProgram(shader);
  glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
  if (!result) {
    glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
    printf("Error validating program: '%s'\n", eLog);
  }

  uniformModel = glGetUniformLocation(shader, "model");
  uniformProjection = glGetUniformLocation(shader, "projection");
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Create a GLFW window
  GLFWwindow* main_window =
      glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
  if (!main_window) {
    glfwTerminate();
    return -1;
  }

  int bufferWidth, bufferHeight;
  glfwGetFramebufferSize(main_window, &bufferWidth, &bufferHeight);

  // Make the window's context current
  glfwMakeContextCurrent(main_window);

  // allow modern extension features
  glewExperimental = GL_TRUE;

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    glfwDestroyWindow(main_window);
    glfwTerminate();
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, bufferWidth, bufferHeight);

  CreateTriangle();
  CompileShaders();

  // glm::mat4 projection = glm::perspective(45.0f,
  // (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.0f);
  glm::mat4 projection = glm::perspective(
      glm::radians(45.0f), (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f,
      100.0f);

  // Main loop
  while (!glfwWindowShouldClose(main_window)) {
    // Get + handle user input events
    glfwPollEvents();

    if (direction) {
      triOffset += triIncrement;
    } else {
      triOffset -= triIncrement;
    }

    if (abs(triOffset) >= triMaxOffset) {
      direction = !direction;
    }

    curAngle += 0.1f;
    if (curAngle >= 360) {
      curAngle -= 360;
    }

    if (sizeDirection) {
      curSize += 0.001f;
    } else {
      curSize -= 0.001f;
    }
    if (curSize >= maxSize || curSize <= minSize) {
      sizeDirection = !sizeDirection;
    }

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader);

    // what makes the model move
    // glm::mat4 model(0.1f);
    glm::mat4 model = glm::mat4(1.0f);  // Initialize to identity matrix

    model = glm::translate(model, glm::vec3(0.0f, triOffset, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians,
    // glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);  // clean up
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);  // clean up

    // Render here
    glfwSwapBuffers(main_window);
  }

  glfwTerminate();
  return 0;
}
