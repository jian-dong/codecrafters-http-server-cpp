#include "http_server/http_response.h"

#include <string>

#include <gtest/gtest.h>

namespace http_server {
namespace {

TEST(HttpResponseTest, AddsDefaultContentLengthWhenMissing) {
  HttpResponse response = HttpResponse::Ok();
  response.SetBody("hello");

  const std::string serialized = response.Serialize(false);
  EXPECT_NE(serialized.find("Content-Length: 5\r\n"), std::string::npos);
}

TEST(HttpResponseTest, AddsConnectionCloseHeaderWhenRequested) {
  HttpResponse response = HttpResponse::Ok();
  response.SetBody("ok");

  const std::string serialized = response.Serialize(true);
  EXPECT_NE(serialized.find("Connection: close\r\n"), std::string::npos);
}

TEST(HttpResponseTest, PreservesExplicitContentLengthHeader) {
  HttpResponse response = HttpResponse::Ok();
  response.SetHeader("Content-Length", "42");
  response.SetBody("hello");

  const std::string serialized = response.Serialize(false);
  EXPECT_NE(serialized.find("Content-Length: 42\r\n"), std::string::npos);
  EXPECT_EQ(serialized.find("Content-Length: 5\r\n"), std::string::npos);
}

}  // namespace
}  // namespace http_server
