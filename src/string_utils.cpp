#include "http_server/string_utils.h"

#include <cctype>

namespace http_server {

std::string Trim(std::string_view value) {
  size_t start = 0;
  while (start < value.size() &&
         std::isspace(static_cast<unsigned char>(value[start]))) {
    ++start;
  }

  size_t end = value.size();
  while (end > start &&
         std::isspace(static_cast<unsigned char>(value[end - 1]))) {
    --end;
  }

  return std::string(value.substr(start, end - start));
}

std::string ToLowerCopy(std::string_view value) {
  std::string result;
  result.reserve(value.size());
  for (char ch : value) {
    result.push_back(static_cast<char>(
        std::tolower(static_cast<unsigned char>(ch))));
  }
  return result;
}

bool HeaderHasToken(std::string_view header_value,
                    const std::string& target_token_lowercase) {
  size_t pos = 0;
  while (pos < header_value.size()) {
    const size_t comma = header_value.find(',', pos);
    std::string token =
        comma == std::string_view::npos
            ? Trim(header_value.substr(pos))
            : Trim(header_value.substr(pos, comma - pos));

    const size_t semicolon = token.find(';');
    if (semicolon != std::string::npos) {
      token = Trim(std::string_view(token).substr(0, semicolon));
    }

    if (ToLowerCopy(token) == target_token_lowercase) {
      return true;
    }

    if (comma == std::string_view::npos) {
      break;
    }
    pos = comma + 1;
  }

  return false;
}

bool StartsWith(const std::string& value, const std::string& prefix) {
  return value.rfind(prefix, 0) == 0;
}

}  // namespace http_server
