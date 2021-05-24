#include <state.h>
#include <filesystem>
#include <loguru/loguru.hpp>
#include <utils.h>

using namespace std;
using namespace std::filesystem;
using namespace utils;
using namespace hostpath;
using namespace hostpath::state;

State::State(string const &stateFilePath) : m_stateFilePath(stateFilePath)
{
}

State::~State()
{
}

void State::Init()
{
    if (!exists(m_stateFilePath))
    {
        Dump();
        return;
    }

    if (Restore())
    {
        LOG_F(WARNING, "Failed to restore the previous state");
    }

    Dump();
}

string State::GetLastError() const
{
    return m_lastError;
}

bool State::Restore()
{
    string fileContents;
    if (!ReadFile(m_stateFilePath, fileContents))
    {
        LOG_F(ERROR, "Failed to read state file '%s'", m_stateFilePath.c_str());
        return false;
    }

    if (!m_hostpathState.ParseFromString(fileContents))
    {
        LOG_F(ERROR, "Failed to parse HostPathState message proto bytes");
        return false;
    }

    return true;
}
bool State::Dump()
{
    string protoBytes;
    if (!m_hostpathState.SerializeToString(&protoBytes))
    {
        LOG_F(ERROR, "Failed to serialize the HostPathState message to string");
        return false;
    }

    if (!WriteToFile(m_stateFilePath, protoBytes))
    {
        LOG_F(ERROR, "Failed to dump proto message to file %s", m_stateFilePath.c_str());
        return false;
    }

    return true;
}

bool State::GetVolumeByName(string const &volumeName, HostPathVolume &volume) const
{
    for (auto const &vol : m_hostpathState.volume_list())
    {
        if (vol.volume_name() == volumeName)
        {
            volume.CopyFrom(vol);
            return true;
        }
    }
    return false;
}

void State::UpdateVolume(HostPathVolume const &volume)
{
    m_hostpathState.mutable_volume_list()->Add()->CopyFrom(volume);
    Dump();
}
