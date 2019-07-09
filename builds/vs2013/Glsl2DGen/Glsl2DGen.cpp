
#include "Glsl2DGen.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include "ShaderHelpers.h"

#include <iostream>

static const unsigned int DEFAULT_DATATEXTURE_SIZE = 512;
static const unsigned int NUM_DATA_TEXTURES = 10;

static const char* DEBUG_WINDOW_TITLE =
"Glsl 2D Generation   [data texture #1]   (Num keys to set view)";
static const unsigned int DEBUG_WINDOW_TITLE_VAR = 36;

static const char* SHADER_SAMPLER_NAME = "textureX";
static const unsigned int SHADER_SAMPLER_NAME_VAR = 7;

unsigned int Glsl2DGen::_instanceCount = 0;

void Glsl2DGen::testErrorMessage()
{
  _errorMessage = "haha";
}

///////////////////////
// Setup / breakdown //
///////////////////////
Glsl2DGen::Glsl2DGen()
  // Rendering options
  : _size(DEFAULT_DATATEXTURE_SIZE)
  , _isClamped(true)
  , _isFiltered(false)
  , _dataType(DataType::FLOAT1)
  // Errors
  , _isReady(false)
  , _errorMessage("")
  // Textures
  , _bufferTexture(0)
  , _dataTextures(new unsigned int[NUM_DATA_TEXTURES])
  // Shaders
  , _genericShader(0)
  , _genericShaderTextureLoc(0)
  , _shaders()
  // System
  , _window(nullptr)
  , _vaoId(0)
  , _vboId(0)
  // Debugging
  , _debugWindow_curTexture(0)
{
  // GLFW init (calling multiple times is harmless)
  if (!glfwInit())
  {
    _errorMessage = "init: Unable to initialize glfw";
    return;
  }

  // Window & opengl initialize
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  glfwWindowHint(GLFW_VISIBLE, false);
  _window = glfwCreateWindow(512, 512, DEBUG_WINDOW_TITLE, nullptr, nullptr);
  if (!_window)
  {
    _errorMessage = "init: Failed to create window";
    return;
  }
  glfwMakeContextCurrent(_window);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  // Static initialization
  _errorMessage = staticInit();
  if (_errorMessage != "")
  {
    return;
  }

  // VAO & VBO initialize
  GLfloat vboData[] = {
    -1.0f, +1.0f, 0.0, 1.0,
    +1.0f, +1.0f, 1.0, 1.0,
    -1.0f, -1.0f, 0.0, 0.0,
    +1.0f, -1.0f, 1.0, 0.0
  };
  glGenVertexArrays(1, &_vaoId);
  glBindVertexArray(_vaoId);
  glGenBuffers(1, &_vboId);
  glBindBuffer(GL_ARRAY_BUFFER, _vboId);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), vboData, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // FBO initialize
  glGenFramebuffers(2, _fboIds);

  // Buffer texture initialize
  glGenTextures(1, &_bufferTexture);
  glActiveTexture(NUM_DATA_TEXTURES + GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _bufferTexture);

  // Data texture initialize (and put into their respective ogl texture units)
  glGenTextures(NUM_DATA_TEXTURES, _dataTextures);
  for (unsigned int i = 0; i < NUM_DATA_TEXTURES; ++i)
  {
    glActiveTexture(i + GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _dataTextures[i]);
  }

  // Debug window shader initialize
  _errorMessage = compileShader(
    GENERIC_VERTEX_SHADER_SOURCE, GENERIC_FRAGMENT_SHADER_SOURCE, _genericShader);
  if (_errorMessage != "")
  {
    return;
  }
  _genericShaderTextureLoc = glGetUniformLocation(_genericShader, "texture");
  if (_genericShaderTextureLoc == -1)
  {
    _errorMessage = "init: Couldn't get texture location for screen shader";
    return;
  }

  // Set _isReady to true to call setters below succesfully
  _isReady = true;

  // Sizing, clamping, filtering initialize
  _isClamped = !_isClamped;
  if (!setIsClamped(!_isClamped))
  {
    _errorMessage = "init: Unable to set the clamped flag";
    _isReady = false;
    return;
  }
  _isFiltered = !_isFiltered;
  if (!setIsFiltered(!_isFiltered))
  {
    _errorMessage = "init: Unable to set the filtered flag";
    _isReady = false;
    return;
  }
  _size = _size + 1;
  if (!setSize(_size - 1))
  {
    _errorMessage = "init: Unable to set the size";
    _isReady = false;
  }
}
Glsl2DGen::~Glsl2DGen()
{
  if (_isReady)
  {
    dispose();
  }
}
bool Glsl2DGen::dispose()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "dispose: Glsl2DGen not ready";
    return false;
  }

  // Shader cleanup
  glUseProgram(0);
  for (std::unordered_map<std::string, unsigned int>::iterator i = _shaders.begin();
    i != _shaders.end(); ++i)
  {
    glDeleteProgram((*i).second);
  }

  // Texture cleanup
  glDeleteTextures(1, &_bufferTexture);
  glDeleteTextures(NUM_DATA_TEXTURES, _dataTextures);
  delete[] _dataTextures;

  // OpenGL cleanup
  glDeleteBuffers(1, &_vboId);
  glDeleteVertexArrays(1, &_vaoId);
  glDeleteFramebuffers(2, _fboIds);

  // Window cleanup
  glfwDestroyWindow(_window);

  // Static cleanup
  _errorMessage = staticDispose();
  if (_errorMessage != "")
  {
    return false;
  }

  // Finish
  _isReady = false;
  return true;
}

////////////
// Errors //
////////////
const std::string& Glsl2DGen::getErrorMessage() const
{
  return _errorMessage;
}
std::string Glsl2DGen::getErrorMessageAndDispose()
{
  std::string result = _errorMessage;
  if (_errorMessage != "")
  {
    dispose();
  }
  return result;
}

//////////
// Data //
//////////
bool Glsl2DGen::getData(unsigned int dataTextureIndex, float* data, unsigned int dataSize)
{
  if (!_isReady)
  {
    _errorMessage = "getData: Glsl2DGen not ready";
    return false;
  }
  if (dataTextureIndex >= NUM_DATA_TEXTURES)
  {
    _errorMessage = "getData: dataIndex out of bounds";
    return false;
  }
  if (data == nullptr)
  {
    _errorMessage = "getData: destination is null";
    return false;
  }


  GLenum colorAttachment = GL_COLOR_ATTACHMENT0;

  // Setup texture
  glActiveTexture(NUM_DATA_TEXTURES + GL_TEXTURE0);
  GLint internalFormat = getInternalFormatForDataType(_dataType);
  GLenum format = getFormatForDataType(_dataType);
  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat, dataSize, dataSize, 0, format, GL_FLOAT, 0);

  // Source FBO
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboIds[0]);
  glFramebufferTexture(GL_READ_FRAMEBUFFER, colorAttachment, _dataTextures[dataTextureIndex], 0);
  glDrawBuffers(1, &colorAttachment);

  // Destination FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboIds[1]);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, colorAttachment, _bufferTexture, 0);
  glDrawBuffers(1, &colorAttachment);

  // Blit
  glBlitFramebuffer(
    0, 0, _size, _size, 0, 0, dataSize, dataSize, GL_COLOR_BUFFER_BIT,
    _isFiltered ? GL_LINEAR : GL_NEAREST);

  // Get pixel data into destination
  glGetTexImage(GL_TEXTURE_2D, 0, format, GL_FLOAT, data);

  // Cleanup and return
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  return true;
}
bool Glsl2DGen::setData(unsigned int dataTextureIndex, float* data, unsigned int dataSize)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setData: Glsl2DGen not ready";
    return false;
  }
  if (dataTextureIndex >= NUM_DATA_TEXTURES)
  {
    _errorMessage = "setData: dataIndex out of bounds";
    return false;
  }
  if (data == nullptr)
  {
    _errorMessage = "setData: source is null";
    return false;
  }

  GLenum colorAttachment = GL_COLOR_ATTACHMENT0;

  // Setup texture
  glActiveTexture(NUM_DATA_TEXTURES + GL_TEXTURE0);
  GLint internalFormat = getInternalFormatForDataType(_dataType);
  GLenum format = getFormatForDataType(_dataType);
  glTexImage2D(
    GL_TEXTURE_2D, 0, internalFormat, dataSize, dataSize, 0, format, GL_FLOAT, data);

  // Source FBO
  glBindFramebuffer(GL_READ_FRAMEBUFFER, _fboIds[0]);
  glFramebufferTexture(GL_READ_FRAMEBUFFER, colorAttachment, _bufferTexture, 0);
  glDrawBuffers(1, &colorAttachment);

  // Destination FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboIds[1]);
  glFramebufferTexture(GL_DRAW_FRAMEBUFFER, colorAttachment, _dataTextures[dataTextureIndex], 0);
  glDrawBuffers(1, &colorAttachment);

  // Blit
  glBlitFramebuffer(
    0, 0, dataSize, dataSize, 0, 0, _size, _size, GL_COLOR_BUFFER_BIT,
    _isFiltered ? GL_LINEAR : GL_NEAREST);

  // Cleanup and return
  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  return true;
}

/////////////
// Shaders //
/////////////
bool Glsl2DGen::addShader(std::string name, std::string fragSource, std::string vertSource)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "addShader: Glsl2DGen not ready";
    return false;
  }

  if (_shaders.find(name) != _shaders.end())
  {
    _errorMessage = "addShader: shader already exists with name '" + name + "'";
    return false;
  }

  if (vertSource == "")
  {
    vertSource = GENERIC_VERTEX_SHADER_SOURCE;
  }

  GLuint programId;
  std::string errorMessage = compileShader(vertSource, fragSource, programId);
  if (errorMessage != "")
  {
    _errorMessage = errorMessage;
    return false;
  }

  // Add to shaders collection
  _shaders[name] = programId;
  return true;
}
bool Glsl2DGen::getHasShader(std::string name)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getHasShader: Glsl2DGen not ready";
    return false;
  }

  return (_shaders.find(name) != _shaders.end());
}
bool Glsl2DGen::runShader(
  std::string name,
  unsigned int outputDataTextureIndex, std::vector<unsigned int> inputDataTextureIndices,
  std::unordered_map<std::string, float> arguments)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "runShader: Glsl2DGen not ready";
    return false;
  }

  // Missing shader
  if (_shaders.find(name) == _shaders.end())
  {
    _errorMessage = "runShader: Shader not found with given name '" + name + "'";
    return false;
  }

  // bad output data texture
  if (outputDataTextureIndex >= NUM_DATA_TEXTURES)
  {
    _errorMessage = "runShader: output data texture Index is out of bounds";
    return false;
  }
  // bad input data texture
  for (std::vector<unsigned int>::iterator i = inputDataTextureIndices.begin();
       i != inputDataTextureIndices.end(); ++i)
  {
    if (*i >= NUM_DATA_TEXTURES)
    {
      _errorMessage = "runShader: input data texture Index is out of bounds";
      return false;
    }
  }

  //// Setup FBO
  glBindFramebuffer(GL_FRAMEBUFFER, _fboIds[0]);
  GLenum colorAttachment = GL_COLOR_ATTACHMENT0;
  glFramebufferTexture(GL_FRAMEBUFFER, colorAttachment, _dataTextures[outputDataTextureIndex], 0);
  glDrawBuffers(1, &colorAttachment);
  glViewport(0, 0, _size, _size);

  // Setup shader
  GLuint shaderId = _shaders[name];
  glUseProgram(shaderId);

  // Setup textures in shader
  GLint uniformLocation;
  std::string shaderSamplerName = SHADER_SAMPLER_NAME;
  int index = 0;
  for (std::vector<unsigned int>::iterator i = inputDataTextureIndices.begin();
       i != inputDataTextureIndices.end(); ++i)
  {
    // Set the variable char in SHADER_SAMPLER_NAME to 'a-z' to represent different data textures
    shaderSamplerName[SHADER_SAMPLER_NAME_VAR] = (char)(index + '1');
    uniformLocation = glGetUniformLocation(shaderId, shaderSamplerName.c_str());
    if (uniformLocation != -1)
    {
      glUniform1i(uniformLocation, (*i));
    }
    ++index;
  }

  // Setup arguments in shader
  for (std::unordered_map<std::string, float>::iterator i = arguments.begin();
       i != arguments.end(); ++i)
  {
    uniformLocation = glGetUniformLocation(shaderId, (*i).first.c_str());
    if (uniformLocation != -1)
    {
      glUniform1f(uniformLocation, (*i).second);
    }
  }


  //// Draw to the FBO using the shader
  glBindVertexArray(_vaoId);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  //// Cleanup and return success
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return true;
}

//////////////////
// Debug window //
//////////////////
bool Glsl2DGen::getDebugWindowVisible()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getDebugWindowVisible: Glsl2DGen not ready";
    return 0;
  }

  return glfwGetWindowAttrib(_window, GLFW_VISIBLE) != 0;
}
bool Glsl2DGen::setDebugWindowVisible(bool isVisible)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setDebugWindowVisible: Glsl2DGen not ready";
    return 0;
  }

  if (isVisible != getDebugWindowVisible())
  {
    if (isVisible)
    {
      glfwShowWindow(_window);
    }
    else
    {
      glfwHideWindow(_window);
    }
  }
  return true;
}
bool Glsl2DGen::tick()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "tick: Glsl2DGen not ready";
    return false;
  }
  if (!getDebugWindowVisible())
  {
    return true;
  }

  if (glfwWindowShouldClose(_window) || glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    setDebugWindowVisible(false);
  }
  else
  {
    // Housekeeping
    glfwSwapBuffers(_window);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();

    // Render to the screen
    int width, height;
    glfwGetWindowSize(_window, &width, &height);
    glViewport(0, 0, width, height);
    glUseProgram(_genericShader);
    glUniform1i(_genericShaderTextureLoc, _debugWindow_curTexture);
    glBindVertexArray(_vaoId);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // React to number keys
    for (int i = 0; i < NUM_DATA_TEXTURES; ++i)
    {
      if (glfwGetKey(_window, i + GLFW_KEY_0) == GLFW_PRESS ||
        glfwGetKey(_window, i + GLFW_KEY_KP_0) == GLFW_PRESS)
      {
        int index = i - 1;
        if (index < 0) { index = 9; }
        _debugWindow_curTexture = index;
        std::string title = DEBUG_WINDOW_TITLE;
        title[DEBUG_WINDOW_TITLE_VAR] = (i + '0');
        glfwSetWindowTitle(_window, title.c_str());
      }
    }
  }

  return true;
}

///////////////////////
// Rendering options //
///////////////////////
unsigned int Glsl2DGen::getSize()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getSize: Glsl2DGen not ready";
    return 0;
  }

  return _size;
}
bool Glsl2DGen::setSize(unsigned int size)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setSize: Glsl2DGen not ready";
    return false;
  }

  if (_size != size)
  {
    _size = size;
    updateTextureSettings();
  }
  return true;
}
bool Glsl2DGen::getIsClamped()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getIsClamped: Glsl2DGen not ready";
    return false;
  }
  return _isClamped;
}
bool Glsl2DGen::setIsClamped(bool value)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setIsClamped: Glsl2DGen not ready";
    return false;
  }
  if (_isClamped != _isClamped)
  {
    _isClamped = value;
    GLint clampType = _isClamped ? GL_CLAMP_TO_EDGE : GL_REPEAT;
    for (unsigned int i = 0; i <= NUM_DATA_TEXTURES; ++i)
    {
      glActiveTexture(i + GL_TEXTURE0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clampType);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clampType);
    }
  }
  return true;
}
bool Glsl2DGen::getIsFiltered()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getIsFiltered: Glsl2DGen not ready";
    return false;
  }
  return _isFiltered;
}
bool Glsl2DGen::setIsFiltered(bool value)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setIsFiltered: Glsl2DGen not ready";
    return false;
  }
  if (value != _isFiltered)
  {
    _isFiltered = value;
    GLint filterType = _isFiltered ? GL_LINEAR : GL_NEAREST;
    for (unsigned int i = 0; i <= NUM_DATA_TEXTURES; ++i)
    {
      glActiveTexture(i + GL_TEXTURE0);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
    }
  }
  return true;
}
Glsl2DGen::DataType Glsl2DGen::getDataType()
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "getDataType: Glsl2DGen not ready";
    return Glsl2DGen::DataType::INVALID;
  }

  return _dataType;
}
bool Glsl2DGen::setDataType(DataType dataType)
{
  _errorMessage = "";
  if (!_isReady)
  {
    _errorMessage = "setDataType: Glsl2DGen not ready";
    return false;
  }
  if (dataType == Glsl2DGen::DataType::INVALID)
  {
    _errorMessage = "setDataType: Invalid DataType given";
    return false;
  }

  if (dataType != _dataType)
  {
    _dataType = dataType;
    updateTextureSettings();
  }
  return true;
}
bool Glsl2DGen::updateTextureSettings()
{
  GLint internalFormat = getInternalFormatForDataType(_dataType);
  GLenum format = getFormatForDataType(_dataType);
  for (int i = 0; i < NUM_DATA_TEXTURES; ++i)
  {
    glActiveTexture(i + GL_TEXTURE0);
    glTexImage2D(
      GL_TEXTURE_2D, 0, internalFormat, _size, _size, 0, format, GL_FLOAT, nullptr);
  }
  return true;
}
int Glsl2DGen::getInternalFormatForDataType(DataType dataType)
{
  switch (dataType)
  {
    case Glsl2DGen::DataType::FLOAT1: return GL_LUMINANCE32F_ARB;
    case Glsl2DGen::DataType::FLOAT3: return GL_RGB32F;
    case Glsl2DGen::DataType::FLOAT4: return GL_RGBA32F;
    default:
      return GL_LUMINANCE32F_ARB;
  }
}
unsigned int Glsl2DGen::getFormatForDataType(DataType dataType)
{
  switch (dataType)
  {
    case Glsl2DGen::DataType::FLOAT1: return GL_RED;
    case Glsl2DGen::DataType::FLOAT3: return GL_RGB;
    case Glsl2DGen::DataType::FLOAT4: return GL_RGBA;
    default:
      return GL_RED;
  }
}

////////////////////////////
// Static setup/breakdown //
////////////////////////////
std::string Glsl2DGen::staticInit()
{
  if (Glsl2DGen::_instanceCount == 0)
  {
    // GLEW init
    GLenum glewResult = glewInit();
    if (glewResult != GLEW_OK)
    {
      std::stringstream result;
      result << "Unable to initialize glew: " << glewResult << std::endl;
      return result.str();
    }
  }
  // Keep track of instances
  ++Glsl2DGen::_instanceCount;
  return "";
}
std::string Glsl2DGen::staticDispose()
{
  // Keep track of instances
  --Glsl2DGen::_instanceCount;
  if (_instanceCount == 0)
  {
    // GLFW cleanup
    glfwTerminate();
  }
  return "";
}

