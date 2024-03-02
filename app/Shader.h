pragma
#include <GL/glew.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

    class Shader {
 public:
  Shader();

  void CreateFromString(const char* vertexCode, const char* fragmentCode);
  void CreateFromFiles(const char* vertexLocation,
                       const char* fragmentLocation);

  std::string ReadFile(const char* fileLocation);

  GLuint GetProjectionLocation();
  GLuint GetModeLocation();

  void UseShader();
  void ClearShader();

  ~Shader();

 private:
  GLuint shaderID, uniformProjection, uniformModel;

  void CompileShader(const char* vertexCode, const char* fragmentCode);
  void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};
