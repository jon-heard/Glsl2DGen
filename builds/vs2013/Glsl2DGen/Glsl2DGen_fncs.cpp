
#include "Glsl2DGen_fncs.h"
#include "Glsl2DGen.h"

const int TMP_BUFFER_SIZE = 1024;
char TMP_BUFFER[TMP_BUFFER_SIZE];

///////////////////////
// Setup / breakdown //
///////////////////////
Glsl2DGen* g2dg_create()
{
  Glsl2DGen* result = new Glsl2DGen;
  return result;
}
bool g2dg_dispose(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  bool result = instance->dispose();
  delete instance;
  return result;
}

////////////
// Errors //
////////////
const char* g2dg_getErrorMessage(Glsl2DGen* instance)
{
  if (instance == nullptr) { return ""; }
  return instance->getErrorMessage().c_str();
}
const char* g2dg_getErrorMessageAndDispose(Glsl2DGen* instance)
{
  if (instance == nullptr) { return ""; }
  strcpy_s(TMP_BUFFER, TMP_BUFFER_SIZE, instance->getErrorMessageAndDispose().c_str());
  if (TMP_BUFFER[0] != '\0')
  {
    delete instance;
  }
  return TMP_BUFFER;
}

//////////
// Data //
//////////
bool g2dg_getData(
  Glsl2DGen* instance, unsigned int dataTextureIndex, float* data, unsigned int dataSize)
{
  if (instance == nullptr) { return false; }
  return instance->getData(dataTextureIndex, data, dataSize);
}
bool g2dg_setData(
  Glsl2DGen* instance, unsigned int dataTextureIndex, float* data, unsigned int dataSize)
{
  if (instance == nullptr) { return false; }
  return instance->setData(dataTextureIndex, data, dataSize);
}

/////////////
// Shaders //
/////////////
bool g2dg_addShader(
  Glsl2DGen* instance, const char* name, const char* fragSource, const char* vertSource)
{
  if (instance == nullptr) { return false; }
  return instance->addShader(name, fragSource, vertSource);
}
bool g2dg_getHasShader(Glsl2DGen* instance, const char* name)
{
  if (instance == nullptr) { return false; }
  return instance->getHasShader(name);
}
bool g2dg_runShader(
  Glsl2DGen* instance, const char* name, unsigned int outputDataTextureIndex,
  unsigned int inputDataTextureIndexCount, unsigned int* inputDataTextureIndices,
  unsigned int argCount, const char** argNames, float* args)
{
  if (instance == nullptr) { return false; }

  std::vector<unsigned int> lInputDataTextureIndices;
  lInputDataTextureIndices.assign(
    inputDataTextureIndices, inputDataTextureIndices + inputDataTextureIndexCount);

  std::unordered_map<std::string, float> lArgs;
  for (unsigned int i = 0; i < argCount; ++i)
  {
    lArgs[argNames[i]] = args[i];
  }
  return instance->runShader(name, outputDataTextureIndex, lInputDataTextureIndices, lArgs);
}

//////////////////
// Debug window //
//////////////////
bool g2dg_getDebugWindowVisible(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  return instance->getDebugWindowVisible();
}
bool g2dg_setDebugWindowVisible(Glsl2DGen* instance, bool isVisible)
{
  if (instance == nullptr) { return false; }
  return instance->setDebugWindowVisible(isVisible);
}
bool g2dg_tick(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  return instance->tick();
}

///////////////////////
// Rendering options //
///////////////////////
unsigned int g2dg_getSize(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  return instance->getSize();
}
bool g2dg_setSize(Glsl2DGen* instance, unsigned int size)
{
  if (instance == nullptr) { return false; }
  return instance->setSize(size);
}
bool g2dg_getIsClamped(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  return instance->getIsClamped();
}
bool g2dg_setIsClamped(Glsl2DGen* instance, bool value)
{
  if (instance == nullptr) { return false; }
  return instance->setIsClamped(value);
}
bool g2dg_getIsFiltered(Glsl2DGen* instance)
{
  if (instance == nullptr) { return false; }
  return instance->getIsFiltered();
}
bool g2dg_setIsFiltered(Glsl2DGen* instance, bool value)
{
  if (instance == nullptr) { return false; }
  return instance->setIsFiltered(value);
}
unsigned int g2dg_getDataType(Glsl2DGen* instance)
{
  if (instance == nullptr) { return 0; }
  Glsl2DGen::DataType dataType = instance->getDataType();
  switch (dataType)
  {
  case Glsl2DGen::DataType::FLOAT1: return 1;
  case Glsl2DGen::DataType::FLOAT3: return 3;
  case Glsl2DGen::DataType::FLOAT4: return 4;
  default:
    return 0;
  }
}
bool g2dg_setDataType(Glsl2DGen* instance, unsigned int dataType)
{
  if (instance == nullptr) { return false; }
  switch (dataType)
  {
  case 1: instance->setDataType(Glsl2DGen::DataType::FLOAT1); break;
  case 3: instance->setDataType(Glsl2DGen::DataType::FLOAT3); break;
  case 4: instance->setDataType(Glsl2DGen::DataType::FLOAT4); break;
  default:
    instance->setDataType(Glsl2DGen::DataType::INVALID);
  }
  return true;
}

