#include <identity_services.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>

using namespace std;
using namespace grpc;
using namespace csi::v1;
using namespace csi::services::identity;

////////////////////////////////////////////////////////////////////////
// GetPluginInfo
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::GetPluginInfo( ServerContext* context,
		GetPluginInfoRequest const* req,
		GetPluginInfoResponse* rsp )
{
	return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// GetPluginCapabilities
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::GetPluginCapabilities( ServerContext* context,
			GetPluginCapabilitiesRequest const* req,
			GetPluginCapabilitiesResponse* rsp )
{
	return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// Probe
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::Probe( ServerContext* context,
	ProbeRequest const* req,
	ProbeResponse* rsp )
{
	return Status::OK;
}
