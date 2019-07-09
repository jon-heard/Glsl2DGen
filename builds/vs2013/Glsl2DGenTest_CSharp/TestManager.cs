
using System;

class TestManager
{
  static void Main(string[] args)
  {
    var app = new TestManager();
    app.Run();
  }

  Test[] tests = new Test[] {
        new Test_DataIO()
      , new Test_Perlin()
    };

  void Run() {
    bool running = true;
    while (running)
    {
      Console.WriteLine("Choose a test to run, or 'enter' key to quit:");
      for (int i = 0; i < tests.Length; ++i)
      {
        Console.WriteLine("\t" + (char)(i + 'a') + " - " + tests[i].Title);
      }
      Console.Write(">> ");
      int choice = Console.ReadKey().KeyChar;
      Console.WriteLine();
      Console.WriteLine();
      if (choice == 13)
      {
        running = false;
      }
      else
      {
        choice = choice - 'a';
        if (choice >= 0 && choice < tests.Length)
        {
          Console.WriteLine("--------------------------------");
          Console.WriteLine(tests[choice].Title);
          Console.WriteLine();
          Console.WriteLine(tests[choice].Description);
          string result = tests[choice].execute();
          if (result == "")
          {
            Console.WriteLine("PASSED");
          }
          else
          {
            Console.WriteLine("FAILED: " + result);
          }
          Console.WriteLine("--------------------------------");
          Console.WriteLine();
          Console.WriteLine();
        }
      }
    }
  }
}
