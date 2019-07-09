
#ifndef SHADERHELPER_H
#define SHADERHELPER_H

#include <string>

extern const char* GENERIC_VERTEX_SHADER_SOURCE;
extern const char* GENERIC_FRAGMENT_SHADER_SOURCE;

std::string compileShader(std::string vertSource, std::string fragSource, unsigned int& result);

#endif // SHADERHELPER_H
