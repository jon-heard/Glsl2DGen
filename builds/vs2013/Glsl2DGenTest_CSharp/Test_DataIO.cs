using System;
using g2dg;

class Test_DataIO : Test
{
  public const uint DATA_SIZE = 5;

  public Test_DataIO() :
    base("Data I/O", "My description")
  {}

  public override string execute()
  {
    // Data initialize
 	  const uint DATA_SQUARE_SIZE = DATA_SIZE * DATA_SIZE;
    float[] sourceData = new float[] {
      0.5f, 0.23f, 0.34f, 0.45f, 0.56f,
      0.67f, 0.78f, 0.89f, 0.91f, 0.13f,
      0.24f, 0.35f, 0.46f, 0.57f, 0.68f,
      0.79f, 0.81f, 0.92f, 0.14f, 0.25f,
      0.36f, 0.47f, 0.58f, 0.69f, 0.71f
    };
    float[] destinationData = new float[DATA_SQUARE_SIZE];

    // Run test
    Glsl2DGen g2dg = new Glsl2DGen();
    string err = g2dg.GetErrorMessageAndDispose();
    if (err != "") { return err; }
    if (!g2dg.SetData(0, sourceData)) { return g2dg.GetErrorMessageAndDispose(); }
    if (!g2dg.GetData(0, destinationData)) { return g2dg.GetErrorMessageAndDispose(); }

    Console.WriteLine("Source array:");
    print2DFloatArray(sourceData, DATA_SIZE, DATA_SIZE);
    Console.WriteLine();
    Console.WriteLine("Destination array:");
    print2DFloatArray(sourceData, DATA_SIZE, DATA_SIZE);
    Console.WriteLine();

    g2dg.DebugWindowVisible = true;
    while (g2dg.DebugWindowVisible)
    {
      if (!g2dg.Tick()) { return g2dg.GetErrorMessageAndDispose(); }
    }

    if (!g2dg.Dispose()) { return "Failed to dispose"; }
    return "";
  }
}
