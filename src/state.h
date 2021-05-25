#ifndef STATE_H
#define STATE_H

#include <hostpath.pb.h>

namespace hostpath::state
{
    class State
    {
    public:
        State(std::string const &stateFilePath);
        State(State const &) = delete;
        State &operator=(State const &) = delete;
        ~State();

        void Init();
        std::string GetLastError() const;
        bool GetVolumeByName(std::string const &volumeName, hostpath::HostPathVolume &volume) const;
        void UpdateVolume(hostpath::HostPathVolume const &volume);
        bool GetVolumeByID(std::string const &volumeID, hostpath::HostPathVolume &volume) const;
        bool DeleteVolumeByID(std::string const &volumeID);

    private:
        bool Dump();
        bool Restore();

    private:
        std::string m_stateFilePath;
        hostpath::HostPathState m_hostpathState;
        std::string m_lastError;
    };
}
#endif // STATE_H