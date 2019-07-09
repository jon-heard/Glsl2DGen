
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace g2dg
{
  public class Glsl2DGen : IGlsl2DGen
  {
    ///////////////////////
    // Setup / breakdown //
    ///////////////////////
    [DllImport("Glsl2DGen.dll")]
    private static extern IntPtr g2dg_create();
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_dispose(IntPtr instance);
    public Glsl2DGen()
    {
      _cInstance = g2dg_create();
    }
    public bool Dispose()
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      bool result = g2dg_dispose(_cInstance);
      _cInstance = IntPtr.Zero;
      return result;
    }

    ////////////
    // Errors //
    ////////////
    [DllImport("Glsl2DGen.dll")]
    private static extern IntPtr g2dg_getErrorMessage(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern IntPtr g2dg_getErrorMessageAndDispose(IntPtr instance);
    public string ErrorMessage
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return ""; }
        return Marshal.PtrToStringAnsi(g2dg_getErrorMessage(_cInstance));
      }
    }
    public string GetErrorMessageAndDispose()
    {
      if (_cInstance == IntPtr.Zero) { return ""; }
      string result = Marshal.PtrToStringAnsi(g2dg_getErrorMessageAndDispose(_cInstance));
      if (result != "")
      {
        _cInstance = IntPtr.Zero;
      }
      return result;
    }

    //////////
    // Data //
    //////////
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_getData(
      IntPtr instance, uint dataTextureIndex, float[] data, uint dataSize);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setData(
      IntPtr instance, uint dataTextureIndex, float[] data, uint dataSize);
    public bool GetData(uint dataIndex, float[] data)
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      return g2dg_getData(_cInstance, dataIndex, data, (uint)Math.Sqrt(data.Length));
    }
    public bool SetData(uint dataIndex, float[] data)
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      return g2dg_setData(_cInstance, dataIndex, data, (uint)Math.Sqrt(data.Length));
    }

    /////////////
    // Shaders //
    /////////////
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_addShader(
      IntPtr instance, string name, string fragSource, string vertSource);
		[DllImport("Glsl2DGen.dll")]
		private static extern bool g2dg_getHasShader(IntPtr instance, string name);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_runShader(
      IntPtr instance, string name, uint outputDataTextureIndex, uint inputDataTextureIndexCount,
      uint[] inputDataTextureIndices, uint argCount, string[] argNames, float[] args);
    public bool AddShader(string name, string fragSource, string vertSource = "")
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      return g2dg_addShader(_cInstance, name, fragSource, vertSource);
    }
		public bool GetHasShader(string name)
		{
      if (_cInstance == IntPtr.Zero) { return false; }
			return g2dg_getHasShader(_cInstance, name);
		}
    public bool RunShader(
      string name, uint outputDataTextureIndex, uint[] inputDataTextureIndices = null,
      Dictionary<String, float> args = null)
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      uint inputDataTextureIndexCount =
        inputDataTextureIndices == null ? 0 : (uint)inputDataTextureIndices.Length;
      uint argCount = (args == null) ? 0 : (uint)args.Count;
      string[] argNames = null;
      float[] argValues = null;
      if (argCount > 0)
      {
        argNames = new string[argCount];
        argValues = new float[argCount];
        int i = 0;
        foreach (var pair in args)
        {
          argNames[i] = pair.Key;
          argValues[i] = pair.Value;
          ++i;
        }
      }
      return g2dg_runShader(
        _cInstance, name, outputDataTextureIndex,
        inputDataTextureIndexCount, inputDataTextureIndices, argCount, argNames, argValues);
    }

    //////////////////
    // Debug window //
    //////////////////
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_getDebugWindowVisible(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setDebugWindowVisible(IntPtr instance, bool isVisible);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_tick(IntPtr instance);
    public bool DebugWindowVisible
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return false; }
        return g2dg_getDebugWindowVisible(_cInstance);
      }
      set
      {
        if (_cInstance != IntPtr.Zero)
        {
          g2dg_setDebugWindowVisible(_cInstance, value);
        }
      }
    }
    public bool Tick()
    {
      if (_cInstance == IntPtr.Zero) { return false; }
      return g2dg_tick(_cInstance);
    }
    ///////////////////////
    // Rendering options //
    ///////////////////////
    [DllImport("Glsl2DGen.dll")]
    private static extern uint g2dg_getSize(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setSize(IntPtr instance, uint size);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_getIsClamped(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setIsClamped(IntPtr instance, bool value);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_getIsFiltered(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setIsFiltered(IntPtr instance, bool value);
    [DllImport("Glsl2DGen.dll")]
    private static extern uint g2dg_getDataType(IntPtr instance);
    [DllImport("Glsl2DGen.dll")]
    private static extern bool g2dg_setDataType(IntPtr instance, uint dataType);
    public uint Size
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return 0; }
        return g2dg_getSize(_cInstance);
      }
      set
      {
        if (_cInstance != IntPtr.Zero)
        {
          g2dg_setSize(_cInstance, value);
        }
      }
    }
    public bool IsClamped
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return true; }
        return g2dg_getIsClamped(_cInstance);
      }
      set
      {
        if (_cInstance != IntPtr.Zero)
        {
          g2dg_setIsClamped(_cInstance, value);
        }
      }
    }
    public bool IsFiltered
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return false; }
        return g2dg_getIsFiltered(_cInstance);
      }
      set
      {
        if (_cInstance != IntPtr.Zero)
        {
          g2dg_setIsFiltered(_cInstance, value);
        }
      }
    }
    public DataType DataType
    {
      get
      {
        if (_cInstance == IntPtr.Zero) { return DataType.INVALID; }
        uint result = g2dg_getDataType(_cInstance);
        switch (result)
        {
          case 1: return DataType.FLOAT1;
          case 3: return DataType.FLOAT3;
          case 4: return DataType.FLOAT4;
          default:
            return DataType.INVALID;
        }
      }
      set
      {
        if (_cInstance != IntPtr.Zero)
        {
          switch (value)
          {
            case DataType.FLOAT1: g2dg_setDataType(_cInstance, 1); break;
            case DataType.FLOAT3: g2dg_setDataType(_cInstance, 3); break;
            case DataType.FLOAT4: g2dg_setDataType(_cInstance, 4); break;
            default:
              g2dg_setDataType(_cInstance, 0);
              break;
          }
        }
      }
    }

    private IntPtr _cInstance = IntPtr.Zero;
  }
}
