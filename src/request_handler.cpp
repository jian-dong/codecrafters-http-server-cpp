#include "http_server/request_handler.h"

#include <utility>

#include "http_server/gzip_compressor.h"
#include "http_server/string_utils.h"

namespace http_server {

RequestHandler::RequestHandler(std::string directory)
    : storage_(std::move(directory)) {}

HttpResponse RequestHandler::Handle(const HttpRequest& request) const {
  if (request.method == "GET" && request.path == "/") {
    return HttpResponse::Ok();
  }

  if (request.method == "GET" && StartsWith(request.path, "/echo/")) {
    return HandleEcho(request);
  }

  if (request.method == "GET" && request.path == "/user-agent") {
    return PlainTextResponse(HttpResponse::Ok(), request.HeaderValue("user-agent"));
  }

  if (request.method == "GET" && StartsWith(request.path, "/files/")) {
    std::string contents;
    if (!storage_.ReadFile(request.path.substr(std::string("/files/").size()),
                           &contents)) {
      return HttpResponse::NotFound();
    }

    HttpResponse response = HttpResponse::Ok();
    response.SetHeader("Content-Type", "application/octet-stream");
    response.SetHeader("Content-Length", std::to_string(contents.size()));
    response.SetBody(std::move(contents));
    return response;
  }

  if (request.method == "POST" && StartsWith(request.path, "/files/")) {
    if (!storage_.WriteFile(
            request.path.substr(std::string("/files/").size()), request.body)) {
      return HttpResponse::NotFound();
    }
    return HttpResponse::Created();
  }

  return HttpResponse::NotFound();
}

HttpResponse RequestHandler::PlainTextResponse(HttpResponse response,
                                               const std::string& body) {
  response.SetHeader("Content-Type", "text/plain");
  response.SetHeader("Content-Length", std::to_string(body.size()));
  response.SetBody(body);
  return response;
}

HttpResponse RequestHandler::HandleEcho(const HttpRequest& request) {
  const std::string echo_body = request.path.substr(std::string("/echo/").size());

  if (request.HeaderContainsToken("accept-encoding", "gzip")) {
    std::string compressed;
    if (GzipCompressor::Compress(echo_body, &compressed)) {
      HttpResponse response = HttpResponse::Ok();
      response.SetHeader("Content-Type", "text/plain");
      response.SetHeader("Content-Encoding", "gzip");
      response.SetHeader("Content-Length", std::to_string(compressed.size()));
      response.SetBody(std::move(compressed));
      return response;
    }
  }

  return PlainTextResponse(HttpResponse::Ok(), echo_body);
}

}  // namespace http_server
