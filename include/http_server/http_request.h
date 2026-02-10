#ifndef HTTP_SERVER_HTTP_REQUEST_H_
#define HTTP_SERVER_HTTP_REQUEST_H_

#include <string>
#include <unordered_map>

namespace http_server {

struct HttpRequest {
  std::string method = "GET";
  std::string path = "/";
  std::string version = "HTTP/1.1";
  std::unordered_map<std::string, std::string> headers;
  std::string body;

  std::string HeaderValue(const std::string& name) const;
  bool HeaderContainsToken(const std::string& name,
                           const std::string& token_lowercase) const;
};

}  // namespace http_server

#endif  // HTTP_SERVER_HTTP_REQUEST_H_
