
#include <iostream>

void print2DFloatArray(
  float* toPrint, unsigned int sizeX, unsigned int sizeY, unsigned int clusterSize)
{
  unsigned int cluster = 0;
  for (unsigned int i = 0; i < sizeY; ++i)
  {
    for (unsigned int j = 0; j < sizeX; ++j)
    {
      std::cout << toPrint[j + i * sizeX] << " ";
      if (clusterSize > 1) { ++cluster; }
      if (cluster >= clusterSize)
      {
        cluster = 0;
        std::cout << "  ";
      }
    }
    cluster = 0;
    std::cout << std::endl;
  }
}
