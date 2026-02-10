#ifndef HTTP_SERVER_HTTP_SERVER_H_
#define HTTP_SERVER_HTTP_SERVER_H_

#include <string>

#include "http_server/request_handler.h"

namespace http_server {

class HttpServer {
 public:
  explicit HttpServer(std::string directory);

  int Run();

 private:
  RequestHandler handler_;
};

}  // namespace http_server

#endif  // HTTP_SERVER_HTTP_SERVER_H_
