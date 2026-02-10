#ifndef HTTP_SERVER_HTTP_REQUEST_PARSER_H_
#define HTTP_SERVER_HTTP_REQUEST_PARSER_H_

#include <string>

#include "http_server/http_request.h"

namespace http_server {

class HttpRequestParser {
 public:
  bool TryParse(std::string* buffer, HttpRequest* request) const;

 private:
  static void ParseRequestLine(const std::string& buffer,
                               size_t header_end,
                               HttpRequest* request);
  static void ParseHeaders(const std::string& buffer,
                           size_t header_end,
                           HttpRequest* request);
};

}  // namespace http_server

#endif  // HTTP_SERVER_HTTP_REQUEST_PARSER_H_
