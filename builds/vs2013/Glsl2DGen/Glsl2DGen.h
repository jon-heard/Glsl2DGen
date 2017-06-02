#ifndef GLSL2DGEN_H
#define GLSL2DGEN_H

#include <string>
#include <map>

struct GLFWwindow;

class Glsl2DGen
{
public:
  bool init();
  bool dispose();

  bool addShader(std::string name, std::string source);
  bool runShader(std::string name);
  bool getOutput(float* toFill, int toFillSize);

  bool getIsReady() const;
  unsigned int getSizeX() const;
  unsigned int getSizeY() const;
  bool setSize(unsigned int sizeX, unsigned int sizeY);
  bool getIsVisible() const;
  bool setIsVisible(bool isVisible);
private:
  static int instanceCount;

  GLFWwindow* window;
  unsigned int sizeX, sizeY;
  bool isVisible = true;
  std::map<std::string, unsigned int> shaders;
  bool isReady = false;
  static const char* vertexShaderSource;
  unsigned int fboId;
  unsigned int outTextureId;
};

#endif // GLSL2DGEN_H