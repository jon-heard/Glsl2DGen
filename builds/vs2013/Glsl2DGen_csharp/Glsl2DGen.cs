
using System;
using System.Runtime.InteropServices;

public class Glsl2DGen {
  [DllImport("Glsl2DGen.dll")]
  private static extern IntPtr g2dg_init();
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_dispose(IntPtr instance);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_addShader(IntPtr instance, string name, string source);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_runShader(IntPtr instance, string name);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_getOutput(IntPtr instance, float[] toFill, int toFillSize);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_getIsReady(IntPtr instance);
  [DllImport("Glsl2DGen.dll")]
  private static extern int g2dg_getSizeX(IntPtr instance);
  [DllImport("Glsl2DGen.dll")]
  private static extern int g2dg_getSizeY(IntPtr instance);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_setSize(IntPtr instance, int sizeX, int sizeY);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_getIsVisible(IntPtr instance);
  [DllImport("Glsl2DGen.dll")]
  private static extern bool g2dg_setIsVisible(IntPtr instance, bool isVisible);

  public bool Init() {
    _cInstance = g2dg_init();
    _isReady = (_cInstance != null);
    return getIsReady();
  }
  public bool Dispose() {
    return g2dg_dispose(_cInstance);
  }

  public bool addShader(string name, string source) {
    return g2dg_addShader(_cInstance, name, source);
  }
  public bool runShader(string name) {
    return g2dg_runShader(_cInstance, name);
  }

  public float[] getOutput() {
    int size = getSizeX() * getSizeY();
    float[] result = new float[size];
    g2dg_getOutput(_cInstance, result, size);
    return result;
  }

  public bool getIsReady()
  {
    if (!_isReady) { return false; }
    return g2dg_getIsReady(_cInstance);
  }

  public int getSizeX() {
      return g2dg_getSizeX(_cInstance);
  }
  public int getSizeY()
  {
      return g2dg_getSizeY(_cInstance);
  }

  public bool setSize(int sizeX, int sizeY)
  {
    return g2dg_setSize(_cInstance, sizeX, sizeY);
  }

  public bool getIsVisible() {
    return g2dg_getIsVisible(_cInstance);
  }

  public bool setIsVisible(bool visible)
  {
      return g2dg_setIsVisible(_cInstance, visible);
  }

  private IntPtr _cInstance;
  private bool _isReady = true;
}
