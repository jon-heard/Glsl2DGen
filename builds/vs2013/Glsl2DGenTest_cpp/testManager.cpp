
#include <string>
#include <iostream>
#include "helpers.h"

// If not -1, only render the single test given by this index
static const int SINGLE_RUN_INDEX = -1;

extern Test test_dataIO;       // 0
extern Test test_colored;      // 1
extern Test test_coloredAlpha; // 2
extern Test test_debugDisplay; // 3
extern Test test_shaders;      // 4
extern Test test_perlin;       // 5

const Test* TESTS[] = {
    &test_dataIO
  , &test_colored
  , &test_coloredAlpha
  , &test_debugDisplay
  , &test_shaders
  , &test_perlin
};

const int TEST_COUNT = (sizeof(TESTS) / sizeof(*TESTS));

int main()
{
  if (SINGLE_RUN_INDEX != -1)
  {
    std::cout << TESTS[SINGLE_RUN_INDEX]->function() << std::endl;
    system("pause");
    return 0;
  }

  bool running = true;
  while (running)
  {
    std::cout << "Choose a test to run, or 'enter' key to quit:" << std::endl;
    for (int i = 0; i < TEST_COUNT; ++i)
    {
      std::cout << '\t' << (char)(i + 'a') << " - " << TESTS[i]->title << std::endl;
    }
    std::cout << ">> ";
    std::cout.flush();
    int choice = getchar();
    if (choice == '\n')
    {
      running = false;
    }
    else
    {
      getchar(); // eat extra newline char
      choice = choice - 'a';
      if (choice >= 0 && choice < TEST_COUNT)
      {
        std::cout << "------------------------------------" << std::endl;
        std::cout << TESTS[choice]->title << std::endl << std::endl;
        std::cout << TESTS[choice]->description << std::endl << std::endl;
        std::string result = TESTS[choice]->function();
        if (result == "")
        {
          std::cout << "PASSED" << std::endl;
        }
        else
        {
          std::cout << "FAILED: " << result << std::endl;
        }
        std::cout << "------------------------------------" << std::endl;
        std::cout << std::endl << std::endl;
      }
    }
  }
}
