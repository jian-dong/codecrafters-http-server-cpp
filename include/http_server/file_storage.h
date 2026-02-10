#ifndef HTTP_SERVER_FILE_STORAGE_H_
#define HTTP_SERVER_FILE_STORAGE_H_

#include <string>

namespace http_server {

class FileStorage {
 public:
  explicit FileStorage(std::string directory);

  bool ReadFile(const std::string& filename, std::string* contents) const;
  bool WriteFile(const std::string& filename, const std::string& contents) const;

 private:
  std::string FilePath(const std::string& filename) const;

  std::string base_directory_;
};

}  // namespace http_server

#endif  // HTTP_SERVER_FILE_STORAGE_H_
