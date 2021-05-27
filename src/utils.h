#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <tuple>
namespace utils
{
    using Available = uint64_t;
    using Capacity = uint64_t;
    bool CreateImageFile(std::string const &file, uint64_t fileSize);
    bool IsUnixSocket(std::string const &address);
    bool IsValidIPaddress(std::string const &address);
    bool IsNameValid(std::string const &name);
    bool ReadFile(std::string const &file, std::string &output);
    bool WriteToFile(std::string const &file, std::string const &input);
    bool DirectoryExists(std::string const &directoryPath);
    bool CreateDirectory(std::string const &directoryPath);
    void DeleteDirectory(std::string const &directoryPath);
    std::tuple<Capacity, Available> GetDirectorySpace(std::string const &directoryPath);
    std::string CreateUUID();
} // namespace utils

#endif // UTILS_H