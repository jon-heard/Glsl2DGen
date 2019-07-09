#ifndef GLSL2DGEN_FNCS_H
#define GLSL2DGEN_FNCS_H

class Glsl2DGen;

extern "C"
{
  ///////////////////////
  // Setup / breakdown //
  ///////////////////////
  /// Create new Glsl2DGen instance
  /// Note: g2dg_getIsReady returns true on a successfully created instance
  ///     \return    The created instance
  __declspec(dllexport) Glsl2DGen* g2dg_create();
  /// Shuts down the given Glsl2DGen instance and deletes the pointer
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          True = This call was successful
  __declspec(dllexport) bool g2dg_dispose(Glsl2DGen* instance);

  ////////////
  // Errors //
  ////////////
  /// The error output from the previous g2dg call.  Empty string means no error
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          A string with any error messages from the previous g2dg call
  __declspec(dllexport) const char* g2dg_getErrorMessage(Glsl2DGen* instance);
  /// Executes 'getErrorMessage()' for return value.  If return isn't "" 'dispose()' is also called
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          A string with any error messages from the previous method call
  __declspec(dllexport) const char* g2dg_getErrorMessageAndDispose(Glsl2DGen* instance);

  //////////
  // Data //
  //////////
  /// Fill a float array with values from the given data texture
  ///     \param instance          Pointer to the Glsl2DGen instance
  ///     \param dataTextureIndex  An index representing which data texture to copy from
  ///     \param data              A 1d array to copy to.  Represents a 2d array of float values
  ///     \param dataSize          The size of the 'data' array argument (both width and height)
  ///     \return                  True = This call was successful
  __declspec(dllexport) bool g2dg_getData(
    Glsl2DGen* instance, unsigned int dataTextureIndex, float* data, unsigned int dataSize);
  /// Fill a data texture with values from the given float array
  ///     \param instance          Pointer to the Glsl2DGen instance
  ///     \param dataTextureIndex  An index representing which data texture to copy to
  ///     \param data              A 1d array to copy from.  Represents a 2d array of float values
  ///     \param dataSize          The size of the 'data' array argument (both width and height)
  ///     \return                  True = This call was successful
  __declspec(dllexport) bool g2dg_setData(
    Glsl2DGen* instance, unsigned int dataTextureIndex, float* data, unsigned int dataSize);

  /////////////
  // Shaders //
  /////////////
  /// Setup a new shader based on the given fragment shader source and vertex shader source
  /// Note: If no vertex shader source is given then a generic vertex shader is used
  ///     \param instance    Pointer to the Glsl2DGen instance
  ///     \param name        The name used to identify this shader
  ///     \param fragSource  The source code to compile into the fragment shader
  ///     \param vertSource  The source code to compile into the vertex shader
  ///     \return            True = This call was successful
  __declspec(dllexport) bool g2dg_addShader(
    Glsl2DGen* instance, const char* name, const char* fragSource, const char* vertSource = "");
  /// Called to determine if there is a shader registered with the given name
  ///   \param instance    Pointer to the Glsl2DGen instance
  ///   \param name  The name to check for
  ///   \return      True: Shader found with given name.  False: No shader was found with name
  __declspec(dllexport) bool g2dg_getHasShader(Glsl2DGen* instance, const char* name);
  /// Run a shader, storing the results in the given data texture
  ///     \param instance                    Pointer to the Glsl2DGen instance
  ///     \param name                        The name of the shader to run
  ///     \param outputDataTextureIndex      The index of the data texture to render results into
  ///     \param inputDataTextureIndexCount  The number of data textures to pass to the shader
  ///     \param inputDataTextureIndex       The indices of the data textures to pass to the shader
  ///     \param argCount                    The number of arguments to pass to this shader
  ///     \param argNames                    The names of the arguments to pass to this shader
  ///     \param args                        The arguments to pass to this shader
  ///     \return                            True = This call was successful
  __declspec(dllexport) bool g2dg_runShader(
    Glsl2DGen* instance, const char* name, unsigned int outputDataTextureIndex,
    unsigned int inputDataTextureIndexCount = 0, unsigned int* inputDataTextureIndices = nullptr,
    unsigned int argCount = 0, const char** argNames = nullptr, float* args = nullptr);

  //////////////////
  // Debug window //
  //////////////////
  /// True = A window is displayed to render the data textures.  Useful for debugging
  /// Note: Hitting a number key in the window displays the associated dataTexture
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          True = this Glsl2DGen instance displays a debug window
  __declspec(dllexport) bool g2dg_getDebugWindowVisible(Glsl2DGen* instance);
  /// True = A window is displayed to render the data textures.  Useful for debugging
  /// Note: Hitting a number key in the window displays the associated dataTexture
  ///     \param instance   Pointer to the Glsl2DGen instance
  ///     \param isVisible  True = this Glsl2DGen instance displays a debug window
  ///     \return           True = This call was successful
  __declspec(dllexport) bool g2dg_setDebugWindowVisible(Glsl2DGen* instance, bool isVisible);
  /// If the debug window is visible then this needs to be called each frame to refresh it
  /// Note: It is harmless to call this when the debug window is not visible
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          True = This call was successful
  __declspec(dllexport)  bool g2dg_tick(Glsl2DGen* instance);

  ///////////////////////
  // Rendering options //
  ///////////////////////
  /// How big the data textures are (both width and height)
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          The current data texture size, or 0 on error
  __declspec(dllexport) unsigned int g2dg_getSize(Glsl2DGen* instance);
  /// How big the data textures are (both width and height)
  /// Note: Resizing the data textures also sets their values to zero
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \param size      The new data texture size
  ///     \return          True = This call was successful
  __declspec(dllexport) bool g2dg_setSize(Glsl2DGen* instance, unsigned int size);
  /// True = Data texture values above 1.0 = data AT 1.0.  Data below 0.0 = data AT 0.0
  /// False = Data texture values repeat when moving above 1.0 and below 0.0.  (ex/ 1.1 = 0.1)
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          True = 'linear' filter.  False = 'nearest' filter (False on error)
  __declspec(dllexport) bool g2dg_getIsClamped(Glsl2DGen* instance);
  /// True = Data texture values above 1.0 = data AT 1.0.  Data below 0.0 = data AT 0.0
  /// False = Data texture values repeat when moving above 1.0 and below 0.0.  (ex/ 1.1 = 0.1)
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \param value     True = clamped.  False = repeated
  ///     \return          True = This call was successful
  __declspec(dllexport) bool g2dg_setIsClamped(Glsl2DGen* instance, bool value);
  /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
  /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          True = 'linear' filter.  False = 'nearest' filter (False on error)
  __declspec(dllexport) bool g2dg_getIsFiltered(Glsl2DGen* instance);
  /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
  /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \param value     True = 'linear' filter.  False = 'nearest' filter
  ///     \return          True = This call was successful
  __declspec(dllexport) bool g2dg_setIsFiltered(Glsl2DGen* instance, bool value);
  /// How many floats are bundled into each datum.  Valid values are: 1,3,4
  /// Note: 3-floats & 4-floats are mostly for bitmap data.  1-float is for non-bitmap & grayscale
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \return          Datum float size used by the given instance.  Can be: 1,3,4 (0 on error)
  __declspec(dllexport) unsigned int g2dg_getDataType(Glsl2DGen* instance);
  /// How many floats are bundled into each datum.  Valid values are: 1,3,4
  /// Note: 3-floats & 4-floats are mostly for bitmap data.  1-float is for non-bitmap & grayscale
  /// Note: Changing the data type also resets the data texture's values to zero
  ///     \param instance  Pointer to the Glsl2DGen instance
  ///     \param dataType  Datum float size used by the given instance.  Can be: 1,3,4
  ///     \return          True = This call was successful
  __declspec(dllexport) bool g2dg_setDataType(Glsl2DGen* instance, unsigned int dataType);
}

#endif // GLSL2DGEN_FNCS_H