#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <string.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 1920, HEIGHT = 1080;

GLuint VAO, VBO, shader;

// vertex shader
static const char* vShader = "                              \n\
#version 330                                                \n\
                                                            \n\
layout (location = 0) in vec3 pos;                          \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
  gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); \n\
}";

// Fragment Shader 
static const char* fShader = "                              \n\
#version 330                                                \n\
                                                            \n\
out vec4 colour;                                            \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
  colour = vec4(1.0, 0.0, 0.0, 1.0);                        \n\
}";



void CreateTriangle() {
  GLfloat vertices[] = {
    -1.0f,  -1.0, 0.0f,
    1.0f, -1.0f,  0.0f,
    0.0f, 1.0f, 0.0f
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind

  glBindVertexArray(0); // unbind
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
  GLchar eLog[1024] = { 0 };

  glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
  if(!result) {
      glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
      printf("Error compiling the %d shader:  '%s'\n", shaderType, eLog);
      return;
  }

  glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if(!shader) {
      printf("Error initialising GLFW");
      return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

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
    GLFWwindow *main_window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);
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

    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Main loop
    while (!glfwWindowShouldClose(main_window)) {
        // Get + handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // clean up

        glUseProgram(0); // clean up

        // Render here
        glfwSwapBuffers(main_window);
    }

    glfwTerminate();
    return 0;
}
