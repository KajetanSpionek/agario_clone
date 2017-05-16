#include "server.hpp"
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

int main(int argc, char* argv[])
{
  try
  {
    // Check command line arguments.
    if (argc != 3)
    {
      std::cerr << "Usage: server <address> <port>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    server 0.0.0.0 7777\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    server 0::0 7777\n";

      return 1;
    }

    std::cout << "Server address: " << argv[1] << std::endl;
    std::cout << "Server port: " << argv[2] << std::endl;
    std::cout << "Server is starting..." << std::endl;

    // Initialise the server.
    websocket::Server s(argv[1], argv[2]);
    
    std::cout << "Server has started." << std::endl;
    std::cout << "Press Ctrl+C (Ctrl+Break) to exit." << std::endl << std::endl;
    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}