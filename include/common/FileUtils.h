#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void showAllFiles(const std::string& directory);

#endif // FILEUTILS_H
