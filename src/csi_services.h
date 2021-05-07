#ifndef CSI_SERVICES_H
#define CSI_SERVICES_H

#include <string>

namespace csi::services {

struct Config
{
	std::string endpoint;
	std::string nodeName;
};

class CSIServices
{
	public:
		CSIServices( Config config );
		CSIServices( CSIServices const& ) = delete;
		CSIServices& operator=( CSIServices const& ) = delete;
		~CSIServices();

		bool Run();

	private:
		Config m_config;
};

}

#endif // CSI_SERVICES_H

