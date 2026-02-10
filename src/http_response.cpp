#include "http_server/http_response.h"

#include <utility>

#include "http_server/string_utils.h"

namespace http_server {

HttpResponse::HttpResponse(int status_code, std::string reason_phrase)
    : status_code_(status_code), reason_phrase_(std::move(reason_phrase)) {}

void HttpResponse::SetHeader(std::string name, std::string value) {
  for (auto& header : headers_) {
    if (ToLowerCopy(header.first) == ToLowerCopy(name)) {
      header.second = std::move(value);
      return;
    }
  }
  headers_.push_back({std::move(name), std::move(value)});
}

void HttpResponse::SetBody(std::string body) { body_ = std::move(body); }

std::string HttpResponse::Serialize(bool close_connection) const {
  std::string response =
      "HTTP/1.1 " + std::to_string(status_code_) + " " + reason_phrase_ + "\r\n";

  for (const auto& header : headers_) {
    response += header.first + ": " + header.second + "\r\n";
  }

  if (!HasHeader("Content-Length")) {
    response += "Content-Length: " + std::to_string(body_.size()) + "\r\n";
  }

  if (close_connection && !HasHeader("Connection")) {
    response += "Connection: close\r\n";
  }

  response += "\r\n";
  response += body_;
  return response;
}

HttpResponse HttpResponse::Ok() { return HttpResponse(200, "OK"); }

HttpResponse HttpResponse::Created() { return HttpResponse(201, "Created"); }

HttpResponse HttpResponse::NotFound() { return HttpResponse(404, "Not Found"); }

bool HttpResponse::HasHeader(const std::string& name) const {
  const std::string lower_name = ToLowerCopy(name);
  for (const auto& header : headers_) {
    if (ToLowerCopy(header.first) == lower_name) {
      return true;
    }
  }
  return false;
}

}  // namespace http_server
