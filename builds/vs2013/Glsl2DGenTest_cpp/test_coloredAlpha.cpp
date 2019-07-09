
#include <Glsl2DGen_fncs.h>
#include <string>
#include <iostream>
#include "helpers.h"

static const std::string TITLE = "Colored alpha";
static const std::string DESCRIPTION =
  "steps:\n"
  "\t1) Create a 'Glsl2DGen' instance\n"
  "\t2) Put arbitrary data into data texture 0\n"
  "\t3) Read data back from data texture 0\n"
  "\t4) Compare input data and output data\n"
  "\t5) Pass if matching, fail if not matching\n";
static const unsigned int DATA_SIZE = 5;

std::string testColoredAlpha();
Test test_coloredAlpha(TITLE, DESCRIPTION, testColoredAlpha);

std::string testColoredAlpha()
{
  // Data initialize
  static const unsigned int DATA_SQUARE_SIZE = DATA_SIZE * DATA_SIZE * 4;
  float sourceData[DATA_SQUARE_SIZE] = {};
  float destinationData[DATA_SQUARE_SIZE] = {};
  for (int y = 0; y < DATA_SIZE; ++y)
  {
    for (int x = 0; x < DATA_SIZE; ++x)
    {
      sourceData[(x + y * DATA_SIZE) * 4 + 3] = (float)y / DATA_SIZE;
    }
  }

  // Run test
  Glsl2DGen* g2dg = g2dg_create();
  std::string err = g2dg_getErrorMessageAndDispose(g2dg);
  if (err != "") { return err; }
  if (!g2dg_setDataType(g2dg, 4)) { return g2dg_getErrorMessageAndDispose(g2dg); }

  /////////
  // Red //
  /////////
  for (int y = 0; y < DATA_SIZE; ++y)
  {
    for (int x = 0; x < DATA_SIZE; ++x)
    {
      sourceData[(x + y * DATA_SIZE) * 4 + 0] = (float)x / DATA_SIZE;
    }
  }
  if (!g2dg_setData(g2dg, 0, sourceData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  if (!g2dg_getData(g2dg, 0, destinationData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  // Output I/O data
  std::cout << "Input array: " << std::endl;
  print2DFloatArray(sourceData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << "output array: " << std::endl;
  print2DFloatArray(destinationData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << std::endl;
  // Evaluate results
  for (int i = 0; i < DATA_SQUARE_SIZE; ++i)
  {
    if (sourceData[i] != destinationData[i])
    {
      return "Input array and output array don't match";
    }
  }

  ///////////
  // Green //
  ///////////
  for (int y = 0; y < DATA_SIZE; ++y)
  {
    for (int x = 0; x < DATA_SIZE; ++x)
    {
      sourceData[(x + y * DATA_SIZE) * 4 + 0] = 0.0f;
      sourceData[(x + y * DATA_SIZE) * 4 + 1] = (float)x / DATA_SIZE;
    }
  }
  if (!g2dg_setData(g2dg, 1, sourceData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  if (!g2dg_getData(g2dg, 1, destinationData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  // Output I/O data
  std::cout << "Input array: " << std::endl;
  print2DFloatArray(sourceData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << "output array: " << std::endl;
  print2DFloatArray(destinationData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << std::endl;
  // Evaluate results
  for (int i = 0; i < DATA_SQUARE_SIZE; ++i)
  {
    if (sourceData[i] != destinationData[i])
    {
      return "Input array and output array don't match";
    }
  }

  //////////
  // Blue //
  //////////
  for (int y = 0; y < DATA_SIZE; ++y)
  {
    for (int x = 0; x < DATA_SIZE; ++x)
    {
      sourceData[(x + y * DATA_SIZE) * 4 + 1] = 0.0f;
      sourceData[(x + y * DATA_SIZE) * 4 + 2] = (float)x / DATA_SIZE;
    }
  }
  if (!g2dg_setData(g2dg, 2, sourceData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  if (!g2dg_getData(g2dg, 2, destinationData, DATA_SIZE))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }
  // Output I/O data
  std::cout << "Input array: " << std::endl;
  print2DFloatArray(sourceData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << "output array: " << std::endl;
  print2DFloatArray(destinationData, DATA_SIZE * 4, DATA_SIZE, 4);
  std::cout << std::endl;
  // Evaluate results
  for (int i = 0; i < DATA_SQUARE_SIZE; ++i)
  {
    if (sourceData[i] != destinationData[i])
    {
      return "Input array and output array don't match";
    }
  }

  // Display debug window, quit when window is closed
  if (!g2dg_setDebugWindowVisible(g2dg, true)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  while (g2dg_getDebugWindowVisible(g2dg))
  {
    if (!g2dg_tick(g2dg)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  }

  if (!g2dg_dispose(g2dg)) { return "Failed to dispose"; }
  return "";
}

