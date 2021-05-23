#include <utils.h>
#include <functional>
#include <memory>
#include <array>
#include <cmath>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <regex>
#include <loguru/loguru.hpp>
using namespace std;

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
}