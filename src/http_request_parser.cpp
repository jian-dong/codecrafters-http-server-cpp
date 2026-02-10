#include "http_server/http_request_parser.h"

#include "http_server/string_utils.h"

namespace http_server {

bool HttpRequestParser::TryParse(std::string* buffer, HttpRequest* request) const {
  const size_t header_end = buffer->find("\r\n\r\n");
  if (header_end == std::string::npos) {
    return false;
  }

  HttpRequest parsed_request;
  ParseRequestLine(*buffer, header_end, &parsed_request);
  ParseHeaders(*buffer, header_end, &parsed_request);

  size_t content_length = 0;
  const std::string content_length_value =
      parsed_request.HeaderValue("content-length");
  if (!content_length_value.empty()) {
    try {
      content_length = std::stoul(Trim(content_length_value));
    } catch (...) {
      content_length = 0;
    }
  }

  const size_t request_end = header_end + 4 + content_length;
  if (buffer->size() < request_end) {
    return false;
  }

  parsed_request.body.assign(buffer->data() + header_end + 4, content_length);

  buffer->erase(0, request_end);
  *request = std::move(parsed_request);
  return true;
}

void HttpRequestParser::ParseRequestLine(const std::string& buffer,
                                         size_t header_end,
                                         HttpRequest* request) {
  const size_t line_end = buffer.find("\r\n");
  if (line_end == std::string::npos || line_end > header_end) {
    return;
  }

  const std::string request_line = buffer.substr(0, line_end);
  const size_t method_end = request_line.find(' ');
  if (method_end == std::string::npos) {
    return;
  }

  request->method = request_line.substr(0, method_end);
  const size_t path_end = request_line.find(' ', method_end + 1);
  if (path_end == std::string::npos) {
    request->path = request_line.substr(method_end + 1);
    return;
  }

  request->path = request_line.substr(method_end + 1, path_end - method_end - 1);
  request->version = request_line.substr(path_end + 1);
}

void HttpRequestParser::ParseHeaders(const std::string& buffer,
                                     size_t header_end,
                                     HttpRequest* request) {
  size_t line_start = buffer.find("\r\n");
  if (line_start == std::string::npos || line_start >= header_end) {
    return;
  }
  line_start += 2;

  while (line_start < header_end) {
    const size_t line_end = buffer.find("\r\n", line_start);
    if (line_end == std::string::npos || line_end > header_end) {
      break;
    }

    const std::string line = buffer.substr(line_start, line_end - line_start);
    const size_t colon = line.find(':');
    if (colon != std::string::npos) {
      const std::string name =
          ToLowerCopy(Trim(std::string_view(line).substr(0, colon)));
      const std::string value = Trim(std::string_view(line).substr(colon + 1));
      request->headers[name] = value;
    }

    line_start = line_end + 2;
  }
}

}  // namespace http_server
