#include <iostream>
#include <string>

#include "http_server/http_server.h"

namespace {

std::string ParseDirectoryArgument(int argc, char** argv) {
  std::string directory;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--directory" && i + 1 < argc) {
      directory = argv[i + 1];
      ++i;
    }
  }
  return directory;
}

}  // namespace

int main(int argc, char** argv) {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Logs from your program will appear here!\n";

  http_server::HttpServer server(ParseDirectoryArgument(argc, argv));
  return server.Run();
}
