#include "pai_app_base.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{

  pai::PaiAppBase app{};

  try
  {
    app.run();
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}