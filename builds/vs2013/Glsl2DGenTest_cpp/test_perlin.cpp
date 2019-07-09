
#include <Glsl2DGen_fncs.h>
#include <string>
#include <iostream>
#include "helpers.h"

#include <time.h>


static const std::string TITLE = "Perlin";
static const std::string DESCRIPTION =
  "Steps:\n"
  "\t1) Create a 'Glsl2DGen' instance\n"
  "\t2) Put arbitrary data into data texture 0\n"
  "\t3) Read data back from data texture 0\n"
  "\t4) Compare input data and output data\n"
  "\t5) Pass if matching, fail if not matching";

std::string testPerlin();
Test test_perlin(TITLE, DESCRIPTION, testPerlin);

static const char* SHADER_NOISE =
  "varying vec2 textureCoordinates;\n"
  "uniform float seed;\n"
  "float rand(vec2 co) {\n"
  "  return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);\n"
  "}\n"
  "void main() {\n"
  "  gl_FragColor.a = 1.0;\n"
  "  gl_FragColor.r = rand(textureCoordinates + vec2(seed, seed));\n"
  "}\n";

static const char* SHADER_PERLIN =
  "varying vec2 textureCoordinates;\n"
  "uniform sampler2D texture1;\n"
  "void main() {\n"
  "  gl_FragColor.a = 1.0;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/128.0) * 0.5;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/064.0) * 0.25;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/032.0) * 0.125;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/016.0) * 0.0625;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/008.0) * 0.03125;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/004.0) * 0.03125;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/002.0) * 0.0078125;\n"
  "  gl_FragColor.r += texture2D(texture1, textureCoordinates/001.0) * 0.0078125;\n"
  "}\n";

std::string testPerlin()
{
  // Data initialize
  char* names[] = { "seed" };
  float values[] = { time(nullptr) % 10000 / 10000.0f };

  // Setup test
  Glsl2DGen* g2dg = g2dg_create();
  std::string err = g2dg_getErrorMessageAndDispose(g2dg);
  if (err != "") { return err; }
  if (!g2dg_addShader(g2dg, "noise", SHADER_NOISE)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  if (!g2dg_addShader(g2dg, "perlin", SHADER_PERLIN)) { return g2dg_getErrorMessageAndDispose(g2dg); }

  if (!g2dg_runShader(g2dg, "noise", 1, 0, nullptr, 1, (const char**)names, values))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
  }

  if (!g2dg_setIsFiltered(g2dg, true)) { return g2dg_getErrorMessageAndDispose(g2dg); }
  unsigned int texIn = 1;
  if (!g2dg_runShader(g2dg, "perlin", 0, 1, &texIn))
  {
    return g2dg_getErrorMessageAndDispose(g2dg);
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
