
using System;

class Test
{
  static void Main(string[] args)
  {
    var app = new Test();
    app.Run();
  }

  public const string Shader1Source =
    "varying vec3 v;\n" +
    "void main(void) {\n" +
    "  gl_FragColor = vec4(v.y, 0, 0, 1.0);\n" +
    "}";
  public const string Shader2Source =
    "varying vec3 v;\n" +
    "uniform sampler2D previous;\n" +
    "void main(void) {\n" +
    "  float value = floor(texture2D(previous, v.xy).r * 10.0) / 10.0;\n" +
    "  gl_FragColor = vec4(value, 0, 0, 1.0);\n" +
    "}";

  // Render a 2d grid of float values to the console
  void print2DFloatArray(float[] toPrint, int sizeX, int sizeY)
  {
      for (int i = 0; i < sizeY; ++i)
      {
          for (int j = 0; j < sizeX; ++j)
          {
              Console.Write(toPrint[j + i * sizeX] + " ");
          }
          Console.WriteLine();
      }
  }

  void Run() {
    Console.WriteLine("Base Test C#: running...");

    // Initialize
    var gen = new Glsl2DGen();
    gen.Init();
    if (!gen.getIsReady()) { return; }
    gen.setSize(5, 10);

    // Setup shader
    if (!gen.addShader("shader1", Shader1Source)) { return; }
    if (!gen.addShader("shader2", Shader2Source)) { return; }

    // Render and output values
    if (!gen.runShader("shader1")) { return; }
    print2DFloatArray(gen.getOutput(), gen.getSizeX(), gen.getSizeY());

    Console.WriteLine();

    // Render and output values
    if (!gen.runShader("shader1")) { return; }
    if (!gen.runShader("shader2")) { return; }
    print2DFloatArray(gen.getOutput(), gen.getSizeX(), gen.getSizeY());

    gen.Dispose();

    Console.WriteLine("Base Test C#: Success");
    Console.WriteLine("Press any key to continue . . .");
    Console.ReadKey();
  }
}
