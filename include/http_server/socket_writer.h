#ifndef HTTP_SERVER_SOCKET_WRITER_H_
#define HTTP_SERVER_SOCKET_WRITER_H_

#include <string>

namespace http_server {

class SocketWriter {
 public:
  static bool SendAll(int fd, const std::string& data);
};

}  // namespace http_server

#endif  // HTTP_SERVER_SOCKET_WRITER_H_
