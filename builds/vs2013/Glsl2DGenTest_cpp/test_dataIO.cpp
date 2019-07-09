
#include <Glsl2DGen_fncs.h>
#include <string>
#include <iostream>
#include "helpers.h"

static const std::string TITLE = "Data I/O";
static const std::string DESCRIPTION =
  "Steps:\n"
  "\t1) Create a 'Glsl2DGen' instance\n"
  "\t2) Put arbitrary data into data texture 0\n"
  "\t3) Read data back from data texture 0\n"
  "\t4) Compare input data and output data\n"
  "\t5) Pass if matching, fail if not matching";
static const unsigned int DATA_SIZE = 5;

std::string testDataIO();
Test test_dataIO(TITLE, DESCRIPTION, testDataIO);

std::string testDataIO()
{
  // Data initialize
  const unsigned int DATA_SQUARE_SIZE = DATA_SIZE * DATA_SIZE;
  float sourceData[DATA_SQUARE_SIZE] = {
    0.5f, 0.23f, 0.34f, 0.45f, 0.56f,
    0.67f, 0.78f, 0.89f, 0.91f, 0.13f,
    0.24f, 0.35f, 0.46f, 0.57f, 0.68f,
    0.79f, 0.81f, 0.92f, 0.14f, 0.25f,
    0.36f, 0.47f, 0.58f, 0.69f, 0.71f
  };
  float destinationData[DATA_SQUARE_SIZE] = {};

  // Run test
  Glsl2DGen* g2dg = g2dg_create();
  std::string err = g2dg_getErrorMessageAndDispose(g2dg);
  if (err != "") { return err; }
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
  print2DFloatArray(sourceData, DATA_SIZE, DATA_SIZE);
  std::cout << "output array: " << std::endl;
  print2DFloatArray(destinationData, DATA_SIZE, DATA_SIZE);
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
