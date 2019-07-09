
using System;
using System.Collections.Generic;

namespace g2dg
{
  public interface IGlsl2DGen
  {
    /////////////////////////
    //// Setup / breakdown //
    /////////////////////////
    ///// Shuts down this Glsl2DGen object
    /////     \return    True = This call was successful
    bool Dispose();

    ////////////
    // Errors //
    ////////////
    /// Error output from the previous method call. Empty string means no error
    ///     \return    A string with any error messages from the previous method call
    string ErrorMessage { get; }
    /// Executes 'GetErrorMessage', followed by 'Dispose'
    ///     \return    A string with any error messages from the previous method call
    string GetErrorMessageAndDispose();

    //////////
    // Data //
    //////////
    /// Fill the given float array with values from the given data texture
    ///     \param dataTextureIndex  An index representing which data texture to copy from
    ///     \param data              A 1d array to copy to.  Represents a 2d array of float values
    ///     \param dataSize          The size of the 'data' array parameter (both width and height)
    ///     \return                  True = This call was successful
    bool GetData(uint dataIndex, float[] data);
    /// Fill the given data texture with values from the given float array
    ///     \param dataTextureIndex  An index representing which data texture to copy to
    ///     \param data              A 1d array to copy from.  Represents a 2d array of float values
    ///     \param dataSize          The size of the 'data' array parameter (both width and height)
    ///     \return                  True = This call was successful
    bool SetData(uint dataIndex, float[] data);

    /////////////
    // Shaders //
    /////////////
    /// Setup a new shader based on the given fragment shader source and vertex shader source
    /// Note: If no vertex shader source is given then a generic vertex shader is used
    ///     \param name        The name used to identify this shader
    ///     \param fragSource  The source code to compile into the fragment shader
    ///     \param vertSource  The source code to compile into the vertex shader
    ///     \return            True = This call was successful
    bool AddShader(string name, string fragSource, string vertSource = "");
		/// Called to determine if there is a shader registered with the given name
		///   \param name  The name to check for
		///   \return      True: Shader found with given name.  False: No shader was found with name
		bool GetHasShader(string name);
    /// Run a shader, storing the results in the given data texture
    /// Note: Arguments are passed as key-value pairs.  Keys are in 'argNames', values are in 'args'
    ///     \param name             The name of the shader to run
    ///     \param outputDataIndex  The index of the data texture to put the results into
    ///     \param argCount         The number of arguments to pass to this shader
    ///     \param argNames         The names of the arguments to pass to this shader
    ///     \param args             The arguments to pass to this shader
    ///     \return                 True = This call was successful
    bool RunShader(
      string name, uint outputDataTextureIndex, uint[] inputDataTextureIndices = null,
      Dictionary<String, float> args = null);

    //////////////////
    // Debug window //
    //////////////////
    /// True = A window is displayed to render the data textures.  Useful for debugging
    /// Note: Hitting a number key in the window displays the associated dataTexture
    ///     \return    True = this Glsl2DGen instance displays a debug window
    bool DebugWindowVisible { get; set; }
    /// If the debug window is visible then this needs to be called each frame to refresh it
    /// Note: It is harmless to call this when the debug window is not visible
    ///     \return    True = This call was successful
    bool Tick();

    ///////////////////////
    // Rendering options //
    ///////////////////////
    /// How big the data textures are (both width and height)
    /// Note: Resizing the data textures also resets their values to zero
    ///     \return    The current data texture size, or 0 on error
    uint Size { get; set; }
    /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
    /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
    ///     \return    True = 'linear' filter.  False = 'nearest' filter (False on error)
    bool IsClamped { get; set; }
    /// True = Data textures are read to and written from using a 'linear' filter (blurs when zoomed)
    /// False = Data textures are read to and written from using a 'nearest' filter (ie. no filter)
    ///     \return    True = 'linear' filter.  False = 'nearest' filter (False on error)
    bool IsFiltered { get; set; }
    /// The dataType represents how many floats are bundled into each datum (one, three or four)
    /// Note: FLOAT3 & FLOAT4 are mostly for bitmap data.  FLOAT1 is for non-bitmap & grayscale
    /// Note: Changing the data type also resets the data texture's values to zero
    ///     \return    The current dataType used by this Glsl2DGen instance (INVALID on error)
    DataType DataType { get; set; }
  }
}
