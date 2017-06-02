
#include "Glsl2DGen_fncs.h"
#include "Glsl2DGen.h"

Glsl2DGen* g2dg_init() {
    Glsl2DGen* result = new Glsl2DGen;
    if (!result->init() || !result->getIsReady()) {
        result->dispose();
        return NULL;
    }
    return result;
}
bool g2dg_dispose(Glsl2DGen* instance) {
  if (instance == NULL) { return false; }
  return instance->dispose();
}

bool g2dg_addShader(Glsl2DGen* instance, const char* name, const char* source) {
  if (instance == NULL) { return false; }
  return instance->addShader(name, source);
}
bool g2dg_runShader(Glsl2DGen* instance, const char* name) {
  if (instance == NULL) { return false; }
  return instance->runShader(name);
}
bool g2dg_getOutput(Glsl2DGen* instance, float* toFill, int toFillSize) {
  if (instance == NULL) { return NULL; }
  return instance->getOutput(toFill, toFillSize);
}

int g2dg_getIsReady(Glsl2DGen* instance) {
  if (instance == NULL) { return 0; }
  return instance->getIsReady();
}


int g2dg_getSizeX(Glsl2DGen* instance) {
  if (instance == NULL) { return 0; }
  return instance->getSizeX();
}
int g2dg_getSizeY(Glsl2DGen* instance) {
  if (instance == NULL) { return 0; }
  return instance->getSizeY();
}
bool g2dg_setSize(Glsl2DGen* instance, int sizeX, int sizeY) {
  if (instance == NULL) { return false; }
  return instance->setSize(sizeX, sizeY);
}

bool g2dg_getIsVisible(Glsl2DGen* instance) {
  if (instance == NULL) { return false; }
  return instance->getIsVisible();
}
bool g2dg_setIsVisible(Glsl2DGen* instance, bool isVisible) {
  if (instance == NULL) { return false; }
  return instance->setIsVisible(isVisible);
}
