
#include <Glsl2DGen_fncs.h>
#include <string>
#include <iostream>
#include "helpers.h"

static const std::string TITLE = "Shaders";
static const std::string DESCRIPTION =
  "Steps:\n"
  "\t1) Create a 'Glsl2DGen' instance\n"
  "\t2) Put data into all textures\n"
  "\t3) Run shader with output to texture 1\n"
  "\t4) Display the 'Glsl2DGen' instances debug console\n"
  "\t5) User checks each data texture (by pressing keys '1' to '0').\n"
  "\t\tData texture 3 is blurred version of Data texture 1.\n"
  "\t\tData texture 5 is Data texture 1 and Data texture 3 added together.\n";
static const unsigned int DATA_SIZE = 16;

std::string testShaders();
Test test_shaders(TITLE, DESCRIPTION, testShaders);

static const char* SHADER_BLUR =
  "varying vec2 textureCoordinates;\n"
  "uniform sampler2D texture1;\n"
  "void main() {\n"
  "  gl_FragColor = \n"
  "    texture2D(texture1, textureCoordinates + vec2(+0.00, +0.00)) +\n"
  "    texture2D(texture1, textureCoordinates + vec2(+0.00, +0.05)) +\n"
  "    texture2D(texture1, textureCoordinates + vec2(+0.00, -0.05)) +\n"
  "    texture2D(texture1, textureCoordinates + vec2(+0.05, +0.00)) +\n"
  "    texture2D(texture1, textureCoordinates + vec2(-0.05, +0.00));\n"
  "  gl_FragColor /= 5.0;\n"
  "}\n";

static const char* SHADER_ADD =
  "varying vec2 textureCoordinates;\n"
  "uniform sampler2D texture1;\n"
  "uniform sampler2D texture2;\n"
  "void main() {\n"
  "  gl_FragColor = \n"
  "    texture2D(texture1, textureCoordinates) +\n"
  "    texture2D(texture2, textureCoordinates);\n"
  "}\n";

std::string testShaders()
{
  // Data initialize
  const int DATA_SQUARE_SIZE = DATA_SIZE * DATA_SIZE;
  float sourceData[DATA_SQUARE_SIZE] = {};
  sourceData[100] = 1.0f;
  sourceData[200] = 1.0f;

  // Setup test
  Glsl2DGen* g2dg = g2dg_create();
  std::string err = g2dg_getErrorMessageAndDispose(g2dg);
  if (err != "") { return err; }
  if (!g2dg_setSize(g2dg, DATA_SIZE)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  for (int i = 0; i < 10; ++i)
  {
    sourceData[i] = 1.0f;
    if (!g2dg_setData(g2dg, i, sourceData, DATA_SIZE))
    {
      return g2dg_getErrorMessage(g2dg);
    }
    sourceData[i] = 0.0f;
  }
  unsigned int texIn[] = { 0, 2 };
  if (!g2dg_addShader(g2dg, "blur", SHADER_BLUR)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  if (!g2dg_addShader(g2dg, "add", SHADER_ADD)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  if (!g2dg_runShader(g2dg, "blur", 2, 1, texIn)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  if (!g2dg_runShader(g2dg, "add", 4, 2, texIn)) { return g2dg_getErrorMessageAndDispose(g2dg); }

  // Display debug window, quit when window is closed
  if (!g2dg_setDebugWindowVisible(g2dg, true)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  while (g2dg_getDebugWindowVisible(g2dg))
  {
    if (!g2dg_tick(g2dg)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  }

  if (!g2dg_dispose(g2dg)) { return "Failed to dispose"; }
  return "";
}
