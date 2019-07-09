
#include <string>
#include <GL/glew.h>

const char* GENERIC_VERTEX_SHADER_SOURCE =
  "attribute vec2 vertex_position;\n"
  "attribute vec2 vertex_texCoord;\n"
  "varying vec2 textureCoordinates;\n"
  "void main() {\n"
  "  gl_Position = vec4(vertex_position, 0.0, 1.0);\n"
  "  textureCoordinates = vertex_texCoord;\n"
  "}\n";

const char* GENERIC_FRAGMENT_SHADER_SOURCE =
  "uniform sampler2D texture;\n"
  "varying vec2 textureCoordinates;\n"
  "void main() {\n"
  "  gl_FragColor = texture2D(texture, textureCoordinates);\n"
  "}\n";

std::string compileShader(std::string vertSource, std::string fragSource, unsigned int& result)
{
  GLint status;

  // Compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const char *cVertSource = vertSource.c_str();
  glShaderSource(vertexShader, 1, &cVertSource, nullptr);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    // Get message
    GLint infoLogLength;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* cErrorMessage = new GLchar[infoLogLength];
    glGetShaderInfoLog(vertexShader, infoLogLength, 0, cErrorMessage);
    std::string errorMessage = "compileShader: vertex shader error: ";
    errorMessage += cErrorMessage;
    // Cleanup
    delete cErrorMessage;
    glDeleteShader(vertexShader);
    return errorMessage;
  }

  // Compile the fragment shader
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *cFragSource = fragSource.c_str();
  glShaderSource(fragShader, 1, &cFragSource, nullptr);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
  if (!status)
  {
    // Get message
    GLint infoLogLength;
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* cErrorMessage = new GLchar[infoLogLength];
    glGetShaderInfoLog(fragShader, infoLogLength, 0, cErrorMessage);
    std::string errorMessage = "compileShader: fragment shader error: ";
    errorMessage += cErrorMessage;
    // Cleanup
    delete cErrorMessage;
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return errorMessage;
  }

  // Link the shader program
  GLuint programId = glCreateProgram();
  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragShader);
  glBindAttribLocation(programId, 0, "vertex_position");
  glBindAttribLocation(programId, 1, "vertex_color");
  glLinkProgram(programId);
  glGetProgramiv(programId, GL_LINK_STATUS, &status);
  if (!status)
  {
    // Get message
    GLint infoLogLength;
    glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar* cErrorMessage = new GLchar[infoLogLength];
    glGetShaderInfoLog(programId, infoLogLength, 0, cErrorMessage);
    std::string errorMessage = "compileShader: Shader program error: ";
    errorMessage += cErrorMessage;
    // Cleanup
    delete cErrorMessage;
    glDeleteProgram(programId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    return errorMessage;
  }
  result = programId;

  // Cleanup
  glDeleteShader(vertexShader);
  glDeleteShader(fragShader);
  return "";
}
