#ifndef CSI_SERVICES_H
#define CSI_SERVICES_H

#include <string>
#include <loguru/loguru.hpp>
#include <hostpath.pb.h>

namespace csi::services
{
    class CSIServices
    {
    public:
        CSIServices(hostpath::Config config);
        CSIServices(CSIServices const &) = delete;
        CSIServices &operator=(CSIServices const &) = delete;
        ~CSIServices();

        bool Run();

    private:
        hostpath::Config m_config;
    };

}

#endif // CSI_SERVICES_H
