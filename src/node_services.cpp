#include <node_services.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>

using namespace std;
using namespace grpc;
using namespace hostpath;
using namespace hostpath::state;
using namespace csi::v1;
using namespace csi::services::node;

NodeImpl::NodeImpl(Config const &config, State &state) : m_config(config), m_state(state)
{
}

NodeImpl::~NodeImpl()
{
}

////////////////////////////////////////////////////////////////////////
// NodeStageVolume
////////////////////////////////////////////////////////////////////////
Status
NodeImpl::NodeStageVolume(ServerContext *context,
                          NodeStageVolumeRequest const *req,
                          NodeStageVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeUnstageVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeUnstageVolume(ServerContext *context,
                                   NodeUnstageVolumeRequest const *req,
                                   NodeUnstageVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodePublishVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodePublishVolume(ServerContext *context,
                                   NodePublishVolumeRequest const *req,
                                   NodePublishVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeUnpublishVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeUnpublishVolume(ServerContext *context,
                                     NodeUnpublishVolumeRequest const *req,
                                     NodeUnpublishVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeGetVolumeStats
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeGetVolumeStats(ServerContext *context,
                                    NodeGetVolumeStatsRequest const *req,
                                    NodeGetVolumeStatsResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeExpandVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeExpandVolume(ServerContext *context,
                                  NodeExpandVolumeRequest const *req,
                                  NodeExpandVolumeResponse *rsp)
{
    LOG_F(ERROR, "Node expan volume is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// NodeGetCapabilities
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeGetCapabilities(ServerContext *context,
                                     NodeGetCapabilitiesRequest const *req,
                                     NodeGetCapabilitiesResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeGetInfo
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeGetInfo(ServerContext *context,
                             NodeGetInfoRequest const *req,
                             NodeGetInfoResponse *rsp)
{
    lock_guard<mutex> lock(m_state.GetMutex());
    rsp->set_node_id(m_config.node_name());
    return Status::OK;
}
