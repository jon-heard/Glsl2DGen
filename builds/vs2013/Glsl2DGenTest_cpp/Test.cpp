
#include <iostream>
#include <Glsl2DGen_fncs.h>

using namespace std;

// Render a 2d grid of float values to the console
void print2DFloatArray(float* toPrint, int sizeX, int sizeY) {
  for (int i = 0; i < sizeY; ++i) {
      for (int j = 0; j < sizeX; ++j) {
          cout << toPrint[j + i * sizeX] << " ";
      }
      cout << endl;
  }
}

int main() {
  bool success = true;
  const char* shader1Source =
    "varying vec3 v;\n"
    "uniform sampler2D previous;\n"
    "void main(void) {\n"
    "  gl_FragColor = vec4(v.y, 0, 0, 1.0);\n"
    "}";
  const char* shader2Source =
    "varying vec3 v;\n"
    "uniform sampler2D previous;\n"
    "void main(void) {\n"
    "  float value = floor(texture2D(previous, v.xy).r * 10.0) / 10.0;\n"
    "  gl_FragColor = vec4(value, 0, 0, 1.0);\n"
    "}";

  // Start up
  cout << "Base test C++: running..." << endl;
  
  // Initialize
  Glsl2DGen* gen = g2dg_init();
  if (gen == NULL) { success = false; }
  g2dg_setSize(gen, 5, 10);

  // Setup shader
  if (success && !g2dg_addShader(gen, "shader1", shader1Source)) { success = false; }
  if (success && !g2dg_addShader(gen, "shader2", shader2Source)) { success = false; }

  // Setup output buffer
  int outputSize = g2dg_getSizeX(gen) * g2dg_getSizeY(gen);
  float* output = new float[outputSize];

  // Render and output values
  if (success && !g2dg_runShader(gen, "shader1")) { success = false; }
  g2dg_getOutput(gen, output, outputSize);
  if (success) { print2DFloatArray(output, g2dg_getSizeX(gen), g2dg_getSizeY(gen)); }

  cout << endl;

  // Render and output values
  if (success && !g2dg_runShader(gen, "shader1")) { success = false; }
  if (success && !g2dg_runShader(gen, "shader2")) { success = false; }
  g2dg_getOutput(gen, output, outputSize);
  if (success) { print2DFloatArray(output, g2dg_getSizeX(gen), g2dg_getSizeY(gen)); }

  // Wrap up
  if (!success) {
    cout << "Base test C++: failed" << endl;
  } else {
    cout << "Base test C++: Success" << endl;
  }

  // Shut down
  g2dg_dispose(gen);
  system("pause");
}
