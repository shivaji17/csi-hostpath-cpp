#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace utils
{
    bool CreateImageFile(std::string const &file, uint64_t fileSize);
    bool IsUnixSocket(std::string const &address);
    bool IsValidIPaddress(std::string const &address);
    bool IsNameValid(std::string const &name);
    bool IsNameValid(std::string const &name);
} // namespace utils

#endif // UTILS_H