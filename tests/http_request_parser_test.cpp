#include "http_server/http_request_parser.h"

#include <string>

#include <gtest/gtest.h>

namespace http_server {
namespace {

TEST(HttpRequestParserTest, ReturnsFalseWhenBodyIsIncomplete) {
  HttpRequestParser parser;
  std::string buffer =
      "POST /files/a.txt HTTP/1.1\r\n"
      "Content-Length: 5\r\n"
      "\r\n"
      "abc";

  HttpRequest request;
  EXPECT_FALSE(parser.TryParse(&buffer, &request));
}

TEST(HttpRequestParserTest, ParsesRequestAndLeavesRemainingBytes) {
  HttpRequestParser parser;
  std::string buffer =
      "POST /files/a.txt HTTP/1.1\r\n"
      "Host: localhost\r\n"
      "Content-Length: 5\r\n"
      "\r\n"
      "hello"
      "GET / HTTP/1.1\r\n"
      "\r\n";

  HttpRequest first_request;
  ASSERT_TRUE(parser.TryParse(&buffer, &first_request));
  EXPECT_EQ(first_request.method, "POST");
  EXPECT_EQ(first_request.path, "/files/a.txt");
  EXPECT_EQ(first_request.HeaderValue("host"), "localhost");
  EXPECT_EQ(first_request.body, "hello");

  HttpRequest second_request;
  ASSERT_TRUE(parser.TryParse(&buffer, &second_request));
  EXPECT_EQ(second_request.method, "GET");
  EXPECT_EQ(second_request.path, "/");
  EXPECT_TRUE(buffer.empty());
}

}  // namespace
}  // namespace http_server
