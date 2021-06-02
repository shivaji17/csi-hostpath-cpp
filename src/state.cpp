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

bool State::GetVolumeByID(string const &volumeID, HostPathVolume &volume) const
{
    for (auto const &vol : m_hostpathState.volume_list())
    {
        if (vol.volume_id() == volumeID)
        {
            volume.CopyFrom(vol);
            return true;
        }
    }
    return false;
}

bool State::DeleteVolumeByID(string const &volumeID)
{
    bool volumeFound = false;
    for (auto it = m_hostpathState.mutable_volume_list()->begin(); it != m_hostpathState.mutable_volume_list()->end();)
    {
        auto &vol = *it;
        if (vol.volume_id() == volumeID)
        {
            DeleteDirectory(vol.directory_path());
            it = m_hostpathState.mutable_volume_list()->erase(it);
            volumeFound = true;
            break;
        }
        else
        {
            ++it;
        }
    }

    if (!volumeFound)
    {
        LOG_F(ERROR, "Volume with id '%s' does not exists", volumeID.c_str());
        return false;
    }

    Dump();
    return true;
}

VolumeListWithToken State::GetVolumeList(int maxLength) const
{
    if (maxLength <= 0)
    {
        vector<HostPathVolume> volumeList(m_hostpathState.volume_list().begin(), m_hostpathState.volume_list().end());
        return make_pair(volumeList, "");
    }

    if (m_hostpathState.volume_list().size() < maxLength)
    {
        vector<HostPathVolume> volumeList(m_hostpathState.volume_list().begin(), m_hostpathState.volume_list().end());
        return make_pair(volumeList, "");
    }

    vector<HostPathVolume> volumeList(m_hostpathState.volume_list().begin(), m_hostpathState.volume_list().begin() + maxLength);
    auto token = CreateUUID();
    m_tokenList.insert({token, maxLength});

    return make_pair(volumeList, token);
}

bool State::GetVolumeListForGivenToken(string const &token, vector<HostPathVolume> &volumeList, int maxLength) const
{
    auto itr = m_tokenList.find(token);

    if (itr == m_tokenList.end())
    {
        m_lastError = "Token '" + token + "' does not exist";
        return false;
    }

    if (maxLength <= 0 || m_hostpathState.volume_list().size() - itr->second < maxLength)
    {
        auto index = itr->second;
        m_tokenList.erase(itr);
        if (itr->second >= m_hostpathState.volume_list().size())
        {
            return true;
        }

        copy(m_hostpathState.volume_list().begin() + index, m_hostpathState.volume_list().end(), volumeList.begin());
        return true;
    }

    copy(m_hostpathState.volume_list().begin() + itr->second, m_hostpathState.volume_list().begin() + itr->second + maxLength, volumeList.begin());
    itr->second = itr->second + maxLength;
    return true;
}
