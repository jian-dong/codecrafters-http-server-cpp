#ifndef HTTP_SERVER_STRING_UTILS_H_
#define HTTP_SERVER_STRING_UTILS_H_

#include <string>
#include <string_view>

namespace http_server {

std::string Trim(std::string_view value);
std::string ToLowerCopy(std::string_view value);
bool HeaderHasToken(std::string_view header_value,
                    const std::string& target_token_lowercase);
bool StartsWith(const std::string& value, const std::string& prefix);

}  // namespace http_server

#endif  // HTTP_SERVER_STRING_UTILS_H_
