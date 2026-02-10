#include "http_server/socket_writer.h"

#include <sys/socket.h>

namespace http_server {

bool SocketWriter::SendAll(int fd, const std::string& data) {
  size_t total_sent = 0;
  while (total_sent < data.size()) {
    const ssize_t sent =
        send(fd, data.data() + total_sent, data.size() - total_sent, 0);
    if (sent <= 0) {
      return false;
    }
    total_sent += static_cast<size_t>(sent);
  }
  return true;
}

}  // namespace http_server
