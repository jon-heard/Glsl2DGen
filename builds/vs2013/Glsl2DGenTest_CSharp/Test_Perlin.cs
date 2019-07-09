using System;
using g2dg;
using System.Collections.Generic;

class Test_Perlin : Test
{
  public const uint DATA_SIZE = 5;

  public Test_Perlin() :
    base("Perlin", "My description")
  {}

  const string SHADER_NOISE =
      "varying vec2 textureCoordinates;\n"
    + "uniform float seed;\n"
    + "float rand(vec2 co) {\n"
    + "  return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);\n"
    + "}\n"
    + "void main() {\n"
    + "  gl_FragColor.a = 1.0;\n"
    + "  gl_FragColor.r = rand(textureCoordinates + vec2(seed, seed));\n"
    + "}\n";

  const string SHADER_PERLIN =
      "varying vec2 textureCoordinates;\n"
    + "uniform sampler2D texture1;\n"
    + "void main() {\n"
    + "  gl_FragColor.a = 1.0;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/128.0) * 0.5;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/064.0) * 0.25;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/032.0) * 0.125;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/016.0) * 0.0625;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/008.0) * 0.03125;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/004.0) * 0.03125;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/002.0) * 0.0078125;\n"
    + "  gl_FragColor.r += texture2D(texture1, textureCoordinates/001.0) * 0.0078125;\n"
    + "}\n";

  public override string execute()
  {
    // Data initialize
    Dictionary<String, float> args = new Dictionary<string, float>()
    {
      { "seed", (DateTime.Now.Ticks % 10000) / 10000.0f }
    };

    // Run test
    Glsl2DGen g2dg = new Glsl2DGen();
    string err = g2dg.GetErrorMessageAndDispose();
    if (err != "") { return err; }
    if (!g2dg.AddShader("noise", SHADER_NOISE)) { return g2dg.GetErrorMessageAndDispose(); }
    if (!g2dg.AddShader("perlin", SHADER_PERLIN)) { return g2dg.GetErrorMessageAndDispose(); }

    if (!g2dg.RunShader("noise", 1, null, args)) { return g2dg.GetErrorMessageAndDispose(); }
    g2dg.IsFiltered = true;
    if (!g2dg.RunShader("perlin", 0, new uint[] {1})) { return g2dg.GetErrorMessageAndDispose(); }
    g2dg.IsFiltered = false;

    g2dg.DebugWindowVisible = true;
    while (g2dg.DebugWindowVisible)
    {
      if (!g2dg.Tick()) { return g2dg.GetErrorMessageAndDispose(); }
    }

    if (!g2dg.Dispose()) { return "Failed to dispose"; }
    return "";
  }
}
