#include "http_server/request_handler.h"

#include <filesystem>
#include <string>

#include <zlib.h>

#include <gtest/gtest.h>

namespace http_server {
namespace {

struct ParsedResponse {
  std::string status_line;
  std::string headers;
  std::string body;
};

ParsedResponse ParseSerializedResponse(const std::string& serialized) {
  const size_t line_end = serialized.find("\r\n");
  const size_t header_end = serialized.find("\r\n\r\n");
  EXPECT_NE(line_end, std::string::npos);
  EXPECT_NE(header_end, std::string::npos);

  ParsedResponse parsed;
  parsed.status_line = serialized.substr(0, line_end);
  parsed.headers = serialized.substr(line_end + 2, header_end - (line_end + 2));
  parsed.body = serialized.substr(header_end + 4);
  return parsed;
}

std::string Gunzip(const std::string& data) {
  z_stream stream{};
  stream.next_in =
      reinterpret_cast<Bytef*>(const_cast<char*>(data.data()));
  stream.avail_in = static_cast<uInt>(data.size());

  if (inflateInit2(&stream, 15 + 16) != Z_OK) {
    return "";
  }

  std::string output;
  char outbuffer[4096];
  int ret = Z_OK;

  do {
    stream.next_out = reinterpret_cast<Bytef*>(outbuffer);
    stream.avail_out = sizeof(outbuffer);
    ret = inflate(&stream, 0);
    if (output.size() < stream.total_out) {
      output.append(outbuffer, stream.total_out - output.size());
    }
  } while (ret == Z_OK);

  inflateEnd(&stream);
  if (ret != Z_STREAM_END) {
    return "";
  }

  return output;
}

class RequestHandlerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    temp_dir_ = std::filesystem::temp_directory_path() /
                "http_server_request_handler_test";
    std::filesystem::remove_all(temp_dir_);
    std::filesystem::create_directories(temp_dir_);
  }

  void TearDown() override { std::filesystem::remove_all(temp_dir_); }

  std::filesystem::path temp_dir_;
};

TEST_F(RequestHandlerTest, ReturnsUserAgentAsPlainText) {
  RequestHandler handler(temp_dir_.string());
  HttpRequest request;
  request.method = "GET";
  request.path = "/user-agent";
  request.headers["user-agent"] = "my-test-agent";

  const ParsedResponse response = ParseSerializedResponse(
      handler.Handle(request).Serialize(false));

  EXPECT_EQ(response.status_line, "HTTP/1.1 200 OK");
  EXPECT_NE(response.headers.find("Content-Type: text/plain"),
            std::string::npos);
  EXPECT_EQ(response.body, "my-test-agent");
}

TEST_F(RequestHandlerTest, WritesThenReadsFileContent) {
  RequestHandler handler(temp_dir_.string());

  HttpRequest write_request;
  write_request.method = "POST";
  write_request.path = "/files/test.txt";
  write_request.body = "abc123";
  const ParsedResponse write_response = ParseSerializedResponse(
      handler.Handle(write_request).Serialize(false));

  EXPECT_EQ(write_response.status_line, "HTTP/1.1 201 Created");

  HttpRequest read_request;
  read_request.method = "GET";
  read_request.path = "/files/test.txt";
  const ParsedResponse read_response = ParseSerializedResponse(
      handler.Handle(read_request).Serialize(false));

  EXPECT_EQ(read_response.status_line, "HTTP/1.1 200 OK");
  EXPECT_NE(read_response.headers.find("Content-Type: application/octet-stream"),
            std::string::npos);
  EXPECT_EQ(read_response.body, "abc123");
}

TEST_F(RequestHandlerTest, ReturnsGzipForEchoWhenRequested) {
  RequestHandler handler(temp_dir_.string());
  HttpRequest request;
  request.method = "GET";
  request.path = "/echo/zip-me";
  request.headers["accept-encoding"] = "br, gzip";

  const ParsedResponse response = ParseSerializedResponse(
      handler.Handle(request).Serialize(false));

  EXPECT_EQ(response.status_line, "HTTP/1.1 200 OK");
  EXPECT_NE(response.headers.find("Content-Encoding: gzip"),
            std::string::npos);
  EXPECT_EQ(Gunzip(response.body), "zip-me");
}

}  // namespace
}  // namespace http_server
