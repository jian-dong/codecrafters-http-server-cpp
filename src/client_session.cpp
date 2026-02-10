#include "http_server/client_session.h"

#include <iostream>

#include <sys/socket.h>
#include <unistd.h>

#include "http_server/socket_writer.h"

namespace http_server {

namespace {
constexpr size_t kReadBufferSize = 4096;
}

ClientSession::ClientSession(int client_fd, const RequestHandler& handler)
    : client_fd_(client_fd), handler_(handler) {}

void ClientSession::Run() {
  HttpRequest request;

  while (ReadNextRequest(&request)) {
    const bool close_connection =
        request.HeaderContainsToken("connection", "close");
    HttpResponse response = handler_.Handle(request);

    if (!SocketWriter::SendAll(client_fd_, response.Serialize(close_connection))) {
      std::cerr << "send failed\n";
      break;
    }

    if (close_connection) {
      break;
    }
  }

  close(client_fd_);
}

bool ClientSession::ReadNextRequest(HttpRequest* request) {
  while (true) {
    if (parser_.TryParse(&pending_buffer_, request)) {
      return true;
    }

    char buffer[kReadBufferSize] = {0};
    const ssize_t bytes_received = recv(client_fd_, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
      std::cerr << "recv failed\n";
      return false;
    }
    if (bytes_received == 0) {
      return false;
    }

    pending_buffer_.append(buffer, static_cast<size_t>(bytes_received));
  }
}

}  // namespace http_server
