#ifndef CSI_SERVICES_H
#define CSI_SERVICES_H

namespace csi::services {

class CSIServices
{
	public:
		CSIServices();
		CSIServices( CSIServices const& ) = delete;
		CSIServices& operator=( CSIServices const& ) = delete;
		~CSIServices();

		bool Run();
};

}

#endif // CSI_SERVICES_H

