
#ifndef HELPERS_H
#define HELPERS_H

#include <string>

struct Test
{
  std::string title;
  std::string description;
  std::string(*function)();

  Test(std::string title, std::string description, std::string(*function)())
  {
    this->title = title;
    this->description = description;
    this->function = function;
  }
};

void print2DFloatArray(
  float* toPrint, unsigned int sizeX, unsigned int sizeY, unsigned int clusterSize = 1);

#endif // HELPERS_H
