#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// vertex shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {
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

  Mesh* obj1 = new Mesh();
  obj1->CreateMesh(vertices, indices, 12, 12);
  meshList.push_back(obj1);
}

void CreateShaders() {
  Shader* shader1 = new Shader();
  shader1->CreateFromFiles(vShader, fShader);
  shaderList.push_back(*shader1);
}

int main() {
  mainWindow = Window(800, 600);
  mainWindow.Initialise();

  CreateObjects();
  CreateShaders();

  GLuint uniformProjection = 0, uniformModel = 0;
  // glm::mat4 projection = glm::perspective(45.0f,
  // (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.0f);
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f),
                       (GLfloat)mainWindow.getBufferWidth() /
                           (GLfloat)mainWindow.getBufferHeight(),
                       0.1f, 100.0f);

  printf("beefore main loop... \n");
  // Main loop
  while (!mainWindow.getShouldClose()) {
    // Get + handle user input events
    glfwPollEvents();

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderList[0].UseShader();
    uniformModel = shaderList[0].GetModeLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();

    // what makes the model move
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians,
    // glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                       glm::value_ptr(projection));
    meshList[0]->RenderMesh();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
    model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE,
                       glm::value_ptr(projection));
    meshList[0]->RenderMesh();



    glUseProgram(0);  // clean up

    mainWindow.swapBuffers();
  }
  return 0;
}
