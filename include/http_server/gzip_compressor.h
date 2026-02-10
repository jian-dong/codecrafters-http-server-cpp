#ifndef HTTP_SERVER_GZIP_COMPRESSOR_H_
#define HTTP_SERVER_GZIP_COMPRESSOR_H_

#include <string>

namespace http_server {

class GzipCompressor {
 public:
  static bool Compress(const std::string& input, std::string* output);
};

}  // namespace http_server

#endif  // HTTP_SERVER_GZIP_COMPRESSOR_H_
