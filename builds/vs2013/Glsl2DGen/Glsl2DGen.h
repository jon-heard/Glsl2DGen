#ifndef GLSL2DGEN_H
#define GLSL2DGEN_H

#include <string>
#include <unordered_map>

struct GLFWwindow;


class Glsl2DGen
{
public:
 
  void testErrorMessage();

  /// Defines the ways data can be stored.  Each datum can be a single float, three floats or four
  /// Note: FLOAT3 & FLOAT4 are mostly for bitmap data.  FLOAT1 is for non-bitmap & grayscale
  enum DataType { INVALID, FLOAT1, FLOAT3, FLOAT4 };

  ///////////////////////
  // Setup / breakdown //
  ///////////////////////
  /// Initialize member variables
  Glsl2DGen();
  /// Auto-dispose on destruction
  virtual ~Glsl2DGen();
  /// Shuts down systems controlled by this Glsl2DGen object
  ///   \return    True: This call was successful
  bool dispose();

  ////////////
  // Errors //
  ////////////
  /// Error output from the previous method call. Empty string means no error
  ///   \return    A string with any error messages from the previous method call
  const std::string& getErrorMessage() const;
  /// Executes 'getErrorMessage()' for return value.  If return isn't "" 'dispose()' is also called
  ///   \return    A string with any error messages from the previous method call
  std::string getErrorMessageAndDispose();

  //////////
  // Data //
  //////////
  /// Fill the given float array with values from the given data texture
  ///   \param dataTextureIndex  An index representing which data texture to copy from
  ///   \param data              A 1d array to copy to.  Represents a 2d array of float values
  ///   \param dataSize          The size of the 'data' array parameter (both width and height)
  ///   \return                  True: This call was successful
  bool getData(unsigned int dataTextureIndex, float* data, unsigned int dataSize);
  /// Fill the given data texture with values from the given float array
  ///   \param dataTextureIndex  An index representing which data texture to copy to
  ///   \param data              A 1d array to copy from.  Represents a 2d array of float values
  ///   \param dataSize          The size of the 'data' array parameter (both width and height)
  ///   \return                  True: This call was successful
  bool setData(unsigned int dataTextureIndex, float* data, unsigned int dataSize);

  /////////////
  // Shaders //
  /////////////
  /// Setup a new shader based on the given fragment shader source and vertex shader source
  /// Note: If no vertex shader source is given then a generic vertex shader is used
  ///   \param name        The name used to identify this shader
  ///   \param fragSource  The source code to compile into the fragment shader
  ///   \param vertSource  The source code to compile into the vertex shader
  ///   \return            True: This call was successful
  bool addShader(std::string name, std::string fragSource, std::string vertSource = "");
  /// Called to determine if there is a shader registered with the given name
  ///   \param name  The name to check for
  ///   \return      True: Shader found with given name.  False: No shader was found with name
  bool getHasShader(std::string name);
  /// Run a shader, storing the results in the given data texture
  /// Note: Arguments are passed as key-value pairs.  Keys are in 'argNames', values are in 'args'
  ///   \param name                     The name of the shader to run
  ///   \param outputDataTextureIndex   The index of the data texture to put the results into
  ///   \param inputDataTextureIndices  The indices of data textures to use for shader input
  ///   \param arguments                The name-value pairs of arguments to pass to this shader
  ///   \return                         True: This call was successful
  bool runShader(
    std::string name,
    unsigned int outputDataTextureIndex, std::vector<unsigned int> inputDataTextureIndices,
    std::unordered_map<std::string, float> arguments);

  //////////////////
  // Debug window //
  //////////////////
  /// True = A window is displayed to render the data textures.  Useful for debugging
  /// Note: Hitting a number key in the window displays the associated dataTexture
  ///   \return    True = this Glsl2DGen instance displays a debug window
  bool getDebugWindowVisible();
  /// True = A window is displayed to render the data textures.  Useful for debugging
  /// Note: Hitting a number key in the window displays the associated dataTexture
  ///   \param isVisible  True: this Glsl2DGen instance displays a debug window
  ///   \return           True: This call was successful
  bool setDebugWindowVisible(bool isVisible);
  /// If the debug window is visible then this needs to be called each frame to refresh it
  /// Note: It is harmless to call this when the debug window is not visible
  ///   \return    True = This call was successful
  bool tick();

  ///////////////////////
  // Rendering options //
  ///////////////////////
  /// How big the data textures are (both width and height)
  ///   \return    The current data texture size, or 0 on error
  unsigned int getSize();
  /// How big the data textures are (both width and height)
  /// Note: Resizing the data textures also resets their values to zero
  ///   \param size  The new data texture size
  ///   \return      True: This call was successful
  bool setSize(unsigned int size);
  /// True = Data texture values above 1.0 = data AT 1.0.  Data below 0.0 = data AT 0.0
  /// False = Data texture values repeat when moving above 1.0 and below 0.0.  (ex/ 1.1 = 0.1)
  bool getIsClamped();
  /// True = Data texture values above 1.0 = data AT 1.0.  Data below 0.0 = data AT 0.0
  /// False = Data texture values repeat when moving above 1.0 and below 0.0.  (ex/ 1.1 = 0.1)
  ///   \param value  True: data is clamped.  False: data repeats
  ///   \return       True: This call was successful
  bool setIsClamped(bool value);
  /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
  /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
  ///   \return    True: 'linear' filter.  False: 'nearest' filter (False on error)
  bool getIsFiltered();
  /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
  /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
  ///   \param value  True: 'linear' filter.  False: 'nearest' filter
  ///   \return       True: This call was successful
  bool setIsFiltered(bool value);
  /// The dataType represents how many floats are bundled into each datum (one, three or four)
  /// Note: FLOAT3 & FLOAT4 are mostly for bitmap data.  FLOAT1 is for non-bitmap & grayscale
  ///   \return    The current dataType used by this Glsl2DGen instance (INVALID on error)
  Glsl2DGen::DataType getDataType();
  /// The dataType represents how many floats are bundled into each datum (one, three or four)
  /// Note: FLOAT3 & FLOAT4 are mostly for bitmap data.  FLOAT1 is for non-bitmap & grayscale
  /// Note: Changing the data type also resets the data texture's values to zero
  ///   \param dataType  The new dataType to use in this Glsl2DGen instance (error if INVALID)
  ///   \return          True: This call was successful
  bool setDataType(Glsl2DGen::DataType dataType);


private:
  ///////////////////////
  // Rendering options //
  ///////////////////////
  // The size of the data textures (in 'DataType' units)
  unsigned int _size;
  // True = Data textures are clamped between 0.0 & 1.0.  Data above 1.0 = data AT 1.0.  Data below
  // 0.0 = data AT 0.0.  False = Data textures repeat
  bool _isClamped;
  // True = Data textures accessed with 'linear' filter.  False = ...accessed with 'nearest' filter
  bool _isFiltered;
  // Determines How many floats are attached to each datum
  DataType _dataType;
  // Called for certain render option changes to change texture parameters to match
  bool updateTextureSettings();
  // Get the internal format to be used for the given DataType
  int getInternalFormatForDataType(DataType dataType);
  // Get the format to be used for the given DataType
  unsigned int getFormatForDataType(DataType dataType);
  ////////////
  // Errors //
  ////////////
  // True = This instance is successfully initialized
  bool _isReady;
  // The error output from the previous method call.  Empty string means no error.
  std::string _errorMessage;
  //////////////
  // Textures //
  //////////////
  // Used to allow mis-sizing between data float and data texture during data get/set
  unsigned int _bufferTexture;
  // Opengl textures to store input and output data for shaders
  unsigned int* _dataTextures;
  /////////////
  // Shaders //
  /////////////
  // Shader used to render a texture with no effects
  unsigned int _genericShader;
  // Location of attribute 'texture' in '_genericShader'
  int _genericShaderTextureLoc;
  // All user-defined shaders
  std::unordered_map<std::string, unsigned int> _shaders;
  ////////////////////////////
  // Static setup/breakdown //
  ////////////////////////////
  // Static initialization (called by each instance's init to make sure global systems are running)
  static std::string staticInit();
  // Static shut down (called by each instance's dispose to make sure global systems are shut down)
  static std::string staticDispose();
  // How many instances are currently initialized
  static unsigned int _instanceCount;
  ////////////
  // System //
  ////////////
  // GLFW window that opengl is connected to (when visible, this is the debug window)
  GLFWwindow* _window;
  // Opengl VAO for rendering a screen-aligned-quad
  unsigned int _vaoId;
  // Opengl VBO storing data for a screen-aligned-quad
  unsigned int _vboId;
  // Opengl FBOs used to render to textures
  unsigned int _fboIds[2];
  ///////////////
  // Debugging //
  ///////////////
  // Which data texture is currently displayed in the debug window
  unsigned int _debugWindow_curTexture;
};

#endif // GLSL2DGEN_H
