#include "http_server/http_server.h"

#include <arpa/inet.h>

#include <iostream>
#include <thread>
#include <utility>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "http_server/client_session.h"

namespace http_server {

namespace {
constexpr int kPort = 4221;
constexpr int kConnectionBacklog = 5;
}

HttpServer::HttpServer(std::string directory) : handler_(std::move(directory)) {}

int HttpServer::Run() {
  const int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }

  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) <
      0) {
    std::cerr << "setsockopt failed\n";
    close(server_fd);
    return 1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(kPort);

  if (bind(server_fd, reinterpret_cast<sockaddr*>(&server_addr),
           sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, kConnectionBacklog) != 0) {
    std::cerr << "listen failed\n";
    close(server_fd);
    return 1;
  }

  std::cout << "Waiting for a client to connect...\n";

  while (true) {
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    const int client_fd =
        accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr),
               &client_addr_len);
    if (client_fd < 0) {
      std::cerr << "accept failed\n";
      continue;
    }

    std::cout << "Client connected\n";
    std::thread([this, client_fd]() {
      ClientSession session(client_fd, handler_);
      session.Run();
    }).detach();
  }
}

}  // namespace http_server
