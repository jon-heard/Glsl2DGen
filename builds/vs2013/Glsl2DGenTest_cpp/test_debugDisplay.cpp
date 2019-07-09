
#include <Glsl2DGen_fncs.h>
#include <string>
#include <iostream>
#include "helpers.h"

static const std::string TITLE = "Debug display";
static const std::string DESCRIPTION =
  "Steps:\n"
  "\t1) Create a 'Glsl2DGen' instance\n"
  "\t2) Put data into each data texture\n"
  "\t3) Get data from each data texture\n"
  "\t3) Print input data and output data\n"
  "\t4) Display the 'Glsl2DGen' instances debug console\n"
  "\t5) User checks each data texture (by pressing keys '1' to '0').\n"
  "\t\tData textures should be increasing shades of gray from black to white\n";
static const unsigned int DATA_SIZE = 2;

std::string testDebugDisplay();
Test test_debugDisplay(TITLE, DESCRIPTION, testDebugDisplay);

std::string testDebugDisplay()
{
  // Data initialize
  const int DATA_SQUARE_SIZE = DATA_SIZE * DATA_SIZE;
  float sourceData[DATA_SQUARE_SIZE] = {};
  float destData[DATA_SQUARE_SIZE] = {};

  // Setup test
  Glsl2DGen* g2dg = g2dg_create();
  std::string err = g2dg_getErrorMessageAndDispose(g2dg);
  if (err != "") { return err; }
  for (int i = 0; i < 10; ++i)
  {
    for (int y = 0; y < DATA_SIZE; ++y)
    {
      for (int x = 0; x < DATA_SIZE; ++x)
      {
        sourceData[x + y*DATA_SIZE] = (float)i / 10;
      }
    }
    if (!g2dg_setData(g2dg, i, sourceData, DATA_SIZE))
    {
      return g2dg_getErrorMessage(g2dg);
    }
    if (!g2dg_getData(g2dg, i, destData, DATA_SIZE))
    {
      return g2dg_getErrorMessage(g2dg);
    }

    std::cout << "Input array: " << i << std::endl;
    print2DFloatArray(sourceData, DATA_SIZE, DATA_SIZE);
    std::cout << "Output array: " << i << std::endl;
    print2DFloatArray(destData, DATA_SIZE, DATA_SIZE);
    std::cout << std::endl;
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
