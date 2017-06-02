
#include "Glsl2DGen.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

int Glsl2DGen::instanceCount = 0;
const char* Glsl2DGen::vertexShaderSource =
  "varying vec3 v;\n"
  "void main(void) {\n"
  "  gl_Position = gl_Vertex;\n"
  "  v = (gl_Position.xyz + 1) / 2;\n"
  "}";

bool Glsl2DGen::init() {
  if (Glsl2DGen::instanceCount == 0) {
    if (!glfwInit()) {
      return false;
    }
  }

  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  window = glfwCreateWindow(640, 480, "Glsl 2D Gen", NULL, NULL);
  if (!window) {
    return false;
  }
  glfwMakeContextCurrent(window);

  if (Glsl2DGen::instanceCount == 0) {
      if (glewInit() != GLEW_OK)
      {
          return false;
      }
  }

  unsigned int idBuffer[1];
  glGenFramebuffers(1, idBuffer);
  fboId = idBuffer[0];
  glBindFramebuffer(GL_FRAMEBUFFER, fboId);
  glGenTextures(1, idBuffer);
  outTextureId = idBuffer[0];
  glBindTexture(GL_TEXTURE_2D, outTextureId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, outTextureId, 0);
  idBuffer[0] = GL_COLOR_ATTACHMENT0;
  glDrawBuffers(1, idBuffer);

  setSize(5, 5);

  ++Glsl2DGen::instanceCount;

  isReady = true;
  return true;
}

bool Glsl2DGen::dispose() {
  if (!isReady) { return false; }
  unsigned int idBuffer[1];
  idBuffer[0] = fboId;
  glDeleteFramebuffers(1, idBuffer);
  idBuffer[0] = outTextureId;
  glDeleteTextures(1, idBuffer);
  for (map<string, unsigned int>::iterator i = shaders.begin();
       i != shaders.end(); ++i) {
    glDeleteProgram((*i).second);
  }
  glfwDestroyWindow(window);
  isReady = false;
  --Glsl2DGen::instanceCount;
  if (instanceCount == 0) {
    glfwTerminate();
  }
  return true;
}

bool Glsl2DGen::addShader(string name, string source) {
  if (!isReady) { return false; }

  GLint result;

  // Compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
      glDeleteShader(vertexShader);
      return false;
  }

  // Compile the fragment shader
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *cSource = source.c_str();
  glShaderSource(fragShader, 1, &cSource, NULL);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
  if (!result)
  {
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return false;
  }

  // Link the shader program
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragShader);
  glLinkProgram(programId);
  glGetProgramiv(programId, GL_LINK_STATUS, &result);
  if (!result)
  {
    glDeleteProgram(programId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return false;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);

  // Add to shaders collection
  shaders[name] = programId;
  return true;
}

bool Glsl2DGen::runShader(string name) {
  if (!isReady) { return false; }

  glBindFramebuffer(GL_FRAMEBUFFER, fboId);
  glViewport(0, 0, sizeX, sizeY);

  glUseProgram(shaders[name]);
  glBegin(GL_TRIANGLE_STRIP);
  glTexCoord2f(0, 0);
  glVertex2f(-1, 1);
  glTexCoord2f(1, 0);
  glVertex2f(1, 1);
  glTexCoord2f(0, 1);
  glVertex2f(-1, -1);
  glTexCoord2f(1, 1);
  glVertex2f(1, -1);
  glEnd();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}

bool Glsl2DGen::getOutput(float* toFill, int toFillSize) {
  glBindTexture(GL_TEXTURE_2D, outTextureId);
  int requiredSize = sizeX * sizeY;
  if (toFillSize < requiredSize) {
    float* result = new float[sizeX * sizeY];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_FLOAT, result);
    std::copy(result, result + toFillSize, toFill);
    delete result;
    return true;
  } else {
      glBindTexture(GL_TEXTURE_2D, outTextureId);
      glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_FLOAT, toFill);
      return true;
  }
}

bool Glsl2DGen::getIsReady() const {
  return isReady;
}

unsigned int Glsl2DGen::getSizeX() const {
  return sizeX;
}

unsigned int Glsl2DGen::getSizeY() const {
  return sizeY;
}

bool Glsl2DGen::setSize(unsigned int sizeX, unsigned int sizeY) {
  glBindTexture(GL_TEXTURE_2D, outTextureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, sizeX, sizeY, 0, GL_LUMINANCE, GL_FLOAT, NULL);
  this->sizeX = sizeX;
  this->sizeY = sizeY;
  return true;
}

bool Glsl2DGen::getIsVisible() const {
  if (!isReady) { return false;  }
  return isVisible;
}

bool Glsl2DGen::setIsVisible(bool isVisible) {
  if (!isReady) { return false; }
  this->isVisible = isVisible;
  return true;
}
