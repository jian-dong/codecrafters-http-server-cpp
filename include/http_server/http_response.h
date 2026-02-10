#ifndef HTTP_SERVER_HTTP_RESPONSE_H_
#define HTTP_SERVER_HTTP_RESPONSE_H_

#include <string>
#include <utility>
#include <vector>

namespace http_server {

class HttpResponse {
 public:
  HttpResponse(int status_code, std::string reason_phrase);

  void SetHeader(std::string name, std::string value);
  void SetBody(std::string body);
  std::string Serialize(bool close_connection) const;

  static HttpResponse Ok();
  static HttpResponse Created();
  static HttpResponse NotFound();

 private:
  bool HasHeader(const std::string& name) const;

  int status_code_;
  std::string reason_phrase_;
  std::vector<std::pair<std::string, std::string>> headers_;
  std::string body_;
};

}  // namespace http_server

#endif  // HTTP_SERVER_HTTP_RESPONSE_H_
