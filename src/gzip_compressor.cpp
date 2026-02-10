#include "http_server/gzip_compressor.h"

#include <utility>

#include <zlib.h>

namespace http_server {

namespace {
constexpr size_t kCompressionBufferSize = 4096;
}

bool GzipCompressor::Compress(const std::string& input, std::string* output) {
  z_stream stream{};
  if (deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8,
                   Z_DEFAULT_STRATEGY) != Z_OK) {
    return false;
  }

  stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
  stream.avail_in = static_cast<uInt>(input.size());

  std::string compressed;
  char outbuffer[kCompressionBufferSize];
  int ret = Z_OK;

  do {
    stream.next_out = reinterpret_cast<Bytef*>(outbuffer);
    stream.avail_out = sizeof(outbuffer);
    ret = deflate(&stream, Z_FINISH);
    if (compressed.size() < stream.total_out) {
      compressed.append(outbuffer, stream.total_out - compressed.size());
    }
  } while (ret == Z_OK);

  deflateEnd(&stream);
  if (ret != Z_STREAM_END) {
    return false;
  }

  *output = std::move(compressed);
  return true;
}

}  // namespace http_server
