
using System;

abstract class Test
{
  public readonly string Title;
  public readonly string Description;

  public Test(string name, string description)
  {
    Title = name;
    Description = description;
  }

  public abstract string execute();

  // Render a 2d grid of float values to the console
  public static void print2DFloatArray(
    float[] toPrint, uint sizeX, uint sizeY, uint clusterSize = 1)
  {
    uint cluster = 0;
    for (uint i = 0; i < sizeY; ++i)
    {
      for (uint j = 0; j < sizeX; ++j)
      {
        Console.Write(toPrint[j + i * sizeX] + " ");
        if (clusterSize > 1) { ++cluster; }
        if (cluster >= clusterSize)
        {
          cluster = 0;
          Console.Write("  ");
        }
      }
      cluster = 0;
      Console.WriteLine();
    }
  }
}
