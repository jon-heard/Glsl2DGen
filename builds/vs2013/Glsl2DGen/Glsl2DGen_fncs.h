#ifndef GLSL2DGEN_FNCS_H
#define GLSL2DGEN_FNCS_H

class Glsl2DGen;

extern "C"
{
  __declspec(dllexport) Glsl2DGen* g2dg_init();
  __declspec(dllexport) bool g2dg_dispose(Glsl2DGen* instance);

  __declspec(dllexport) bool g2dg_addShader(Glsl2DGen* instance,
      const char* name, const char* source);
  __declspec(dllexport) bool g2dg_runShader(Glsl2DGen* instance, const char* name);
  __declspec(dllexport) bool g2dg_getOutput(Glsl2DGen* instance, float* toFill, int toFillSize);

  __declspec(dllexport) int g2dg_getIsReady(Glsl2DGen* instance);
  __declspec(dllexport) int g2dg_getSizeX(Glsl2DGen* instance);
  __declspec(dllexport) int g2dg_getSizeY(Glsl2DGen* instance);
  __declspec(dllexport) bool g2dg_setSize(Glsl2DGen* instance, int sizeX, int sizeY);
  __declspec(dllexport) bool g2dg_getIsVisible(Glsl2DGen* instance);
  __declspec(dllexport) bool g2dg_setIsVisible(Glsl2DGen* instance, bool isVisible);
}

#endif // GLSL2DGEN_FNCS_H