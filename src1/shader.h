#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
// the program ID
GLuint handle;
// constructor reads and builds the shader
Shader(const char* vertexPath, const char* fragmentPath);
// use/activate the shader
void use();
// utility uniform functions
void setBool(const std::string &name, bool value) const;
void setInt(const std::string &name, int value) const;
void setFloat(const std::string &name, float value) const;
void setVector3(const char *name, glm::vec3 vector) const;
void setMatrix4(const char *name, glm::mat4 matrix) const;
};
#endif
