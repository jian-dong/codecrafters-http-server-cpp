#ifndef HTTP_SERVER_CLIENT_SESSION_H_
#define HTTP_SERVER_CLIENT_SESSION_H_

#include <string>

#include "http_server/http_request_parser.h"
#include "http_server/request_handler.h"

namespace http_server {

class ClientSession {
 public:
  ClientSession(int client_fd, const RequestHandler& handler);

  void Run();

 private:
  bool ReadNextRequest(HttpRequest* request);

  int client_fd_;
  const RequestHandler& handler_;
  HttpRequestParser parser_;
  std::string pending_buffer_;
};

}  // namespace http_server

#endif  // HTTP_SERVER_CLIENT_SESSION_H_
