#include "http_server/file_storage.h"

#include <fstream>
#include <iterator>
#include <utility>

namespace http_server {

FileStorage::FileStorage(std::string directory)
    : base_directory_(directory.empty() ? "/tmp" : std::move(directory)) {}

bool FileStorage::ReadFile(const std::string& filename,
                           std::string* contents) const {
  std::ifstream file(FilePath(filename), std::ios::binary);
  if (!file.is_open()) {
    return false;
  }

  *contents = std::string((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
  return true;
}

bool FileStorage::WriteFile(const std::string& filename,
                            const std::string& contents) const {
  std::ofstream file(FilePath(filename), std::ios::binary);
  if (!file.is_open()) {
    return false;
  }

  file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
  return file.good();
}

std::string FileStorage::FilePath(const std::string& filename) const {
  return base_directory_ + "/" + filename;
}

}  // namespace http_server
