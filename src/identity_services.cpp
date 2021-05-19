#include <identity_services.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>

using namespace std;
using namespace grpc;
using namespace csi::v1;
using namespace csi::services;
using namespace csi::services::identity;

IdentityImpl::IdentityImpl(Config const &config) : m_config(config)
{
}

IdentityImpl::~IdentityImpl()
{
}

////////////////////////////////////////////////////////////////////////
// GetPluginInfo
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::GetPluginInfo(ServerContext *context,
                                   GetPluginInfoRequest const *req,
                                   GetPluginInfoResponse *rsp)
{
    if (m_config.driverName.empty())
        return Status::CANCELLED;

    if (m_config.vendorVersion.empty())
        return Status::CANCELLED;

    rsp->set_name(m_config.driverName);
    rsp->set_vendor_version(m_config.vendorVersion);

    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// GetPluginCapabilities
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::GetPluginCapabilities(ServerContext *context,
                                           GetPluginCapabilitiesRequest const *req,
                                           GetPluginCapabilitiesResponse *rsp)
{
    auto *cap = rsp->mutable_capabilities()->Add();
    cap->mutable_service()->set_type(PluginCapability::Service::Type::PluginCapability_Service_Type_CONTROLLER_SERVICE);
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// Probe
////////////////////////////////////////////////////////////////////////
Status IdentityImpl::Probe(ServerContext *context,
                           ProbeRequest const *req,
                           ProbeResponse *rsp)
{
    return Status::OK;
}
