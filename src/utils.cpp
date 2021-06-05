#include <utils.h>
#include <functional>
#include <memory>
#include <array>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <regex>
#include <filesystem>
#include <uuid/uuid.h>
#include <loguru/loguru.hpp>

using namespace std;
using namespace std::filesystem;

namespace utils
{
    static string const DD_IF = "/dev/zero";
    static uint64_t const IMAGE_BLOCK_SIZE = 4096;

    static bool ExecuteCommand(string const &cmd, string &cmdOutput)
    {
        std::array<char, 128> buffer;
        string result;
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe)
        {
            return false;
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            cmdOutput += buffer.data();
        }

        return true;
    }

    bool CreateImageFile(string const &file, uint64_t fileSize)
    {
        stringstream cmd("");
        cmd << "dd -if=" << DD_IF << " -of" << file << " bs" << IMAGE_BLOCK_SIZE
            << "count=0"
            << "seek=" << ceil(double(fileSize) / double(IMAGE_BLOCK_SIZE));

        string cmdOutput;
        if (!ExecuteCommand(cmd.str(), cmdOutput))
        {
            LOG_F(ERROR, "Failed to create image file. Error: %s", cmdOutput.c_str());
            return false;
        }
        return true;
    }

    bool IsUnixSocket(string const &address)
    {
        return regex_match(address, regex("^unix://([a-zA-Z_])([/a-zA-Z0-9._-])*([a-zA-Z0-9._-])$"));
    }

    bool IsValidIPaddress(string const &address)
    {
        return regex_match(address, regex("^(tcp://)?((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$"));
    }

    bool IsNameValid(string const &name)
    {
        return regex_match(name, regex("^[a-z0-9][a-z0-9.-]{0,251}[a-z0-9]$"));
    }

    bool ReadFile(string const &file, string &output)
    {
        if (!exists(file))
        {
            LOG_F(ERROR, "File '%s' does not exists", file.c_str());
            return false;
        }

        ifstream fp(file, ofstream::binary);
        if (fp.good())
        {
            string data((istreambuf_iterator<char>(fp)), istreambuf_iterator<char>());
            output = data;
            fp.close();
            return true;
        }

        fp.close();
        return false;
    }

    bool WriteToFile(string const &file, string const &input)
    {
        ofstream fileStream;
        string tempFile =  file + "-temp";
        try
        {
            fileStream.open(tempFile.c_str(), ofstream::binary | ofstream::trunc);

            if (!fileStream.is_open())
            {
                LOG_F(ERROR, "Failed to open file %s", tempFile.c_str());
                return false;
            }

            fileStream << input;
            if (fileStream.bad())
            {
                fileStream.close();
                remove(tempFile.c_str());
                LOG_F(ERROR, "Failed to write data to file %s", tempFile.c_str());
                return false;
            }
        }
        catch (ios_base::failure const &e)
        {
            remove(tempFile.c_str());
            LOG_F(ERROR, "Execption occured while writing to file %s. ERROR: %s", tempFile.c_str(), e.what());
            return false;
        }

        if (rename(tempFile.c_str(), file.c_str()) != 0)
        {
            LOG_F(ERROR, "Failed to rename file. Error: %s", strerror(errno));
            return false;
        }

        return true;
    }

    bool DirectoryExists(std::string const &directoryPath)
    {
        return is_directory(directoryPath);
    }

    bool CreateDirectory(std::string const &directoryPath)
    {
        // If directory exists, return true
        if (is_directory(directoryPath))
            return true;

        return create_directories(directoryPath);
    }

    std::string CreateUUID()
    {
        uuid_t uuid;
        uuid_generate(uuid);
        string uuidStr;
        char uuidArr[37] = "";
        uuid_unparse(uuid, uuidArr);
        for (int i = 0; uuidArr[i] != '\0'; ++i)
            uuidStr += uuidArr[i];
        return uuidStr;
    }

    void DeleteDirectory(string const &directoryPath)
    {
        remove_all(directoryPath);
    }

    tuple<Capacity, Available> GetDirectorySpace(string const &directoryPath)
    {
        auto info = space(directoryPath);
        return make_tuple(static_cast<uint64_t>(info.capacity), static_cast<uint64_t>(info.available));
    }
}