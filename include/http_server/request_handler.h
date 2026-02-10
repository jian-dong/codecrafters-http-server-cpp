#ifndef HTTP_SERVER_REQUEST_HANDLER_H_
#define HTTP_SERVER_REQUEST_HANDLER_H_

#include <string>

#include "http_server/file_storage.h"
#include "http_server/http_request.h"
#include "http_server/http_response.h"

namespace http_server {

class RequestHandler {
 public:
  explicit RequestHandler(std::string directory);

  HttpResponse Handle(const HttpRequest& request) const;

 private:
  static HttpResponse PlainTextResponse(HttpResponse response,
                                        const std::string& body);
  static HttpResponse HandleEcho(const HttpRequest& request);

  FileStorage storage_;
};

}  // namespace http_server

#endif  // HTTP_SERVER_REQUEST_HANDLER_H_
