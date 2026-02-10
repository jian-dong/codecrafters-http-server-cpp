#include "http_server/http_request.h"

#include "http_server/string_utils.h"

namespace http_server {

std::string HttpRequest::HeaderValue(const std::string& name) const {
  const auto it = headers.find(ToLowerCopy(name));
  if (it == headers.end()) {
    return "";
  }
  return it->second;
}

bool HttpRequest::HeaderContainsToken(const std::string& name,
                                      const std::string& token_lowercase) const {
  const auto it = headers.find(ToLowerCopy(name));
  if (it == headers.end()) {
    return false;
  }
  return HeaderHasToken(it->second, token_lowercase);
}

}  // namespace http_server
