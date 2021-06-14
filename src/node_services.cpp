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
    LOG_F(ERROR, "Node stage volume is not supported");
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeUnstageVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeUnstageVolume(ServerContext *context,
                                   NodeUnstageVolumeRequest const *req,
                                   NodeUnstageVolumeResponse *rsp)
{
    LOG_F(ERROR, "Node unstage volume is not supported");
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodePublishVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodePublishVolume(ServerContext *context,
                                   NodePublishVolumeRequest const *req,
                                   NodePublishVolumeResponse *rsp)
{
    LOG_F(ERROR, "Node publish volume is not supported");
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeUnpublishVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeUnpublishVolume(ServerContext *context,
                                     NodeUnpublishVolumeRequest const *req,
                                     NodeUnpublishVolumeResponse *rsp)
{
    LOG_F(ERROR, "Node unpublish volume is not supported");
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeGetVolumeStats
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeGetVolumeStats(ServerContext *context,
                                    NodeGetVolumeStatsRequest const *req,
                                    NodeGetVolumeStatsResponse *rsp)
{
    if (req->volume_id().empty())
    {
        LOG_F(ERROR, "Volume ID missing in request");
        return Status::CANCELLED;
    }

    if (req->volume_path().empty())
    {
        LOG_F(ERROR, "Volume path missing in request");
        return Status::CANCELLED;
    }

    lock_guard<mutex> lock(m_state.GetMutex());
    HostPathVolume volume;

    if (!m_state.GetVolumeByID(req->volume_id(), volume))
    {
        LOG_F(ERROR, "Volume with id '%s' does not exists", req->volume_id().c_str());
        return Status::CANCELLED;
    }

    // FIXME
    // We need a concrete to find the amount of data stored in given directory

    auto *usage = rsp->mutable_usage()->Add();
    usage->set_unit(VolumeUsage::Unit::VolumeUsage_Unit_BYTES);
    usage->set_total(volume.vol_size());
    usage->set_used(0);
    usage->set_available(volume.vol_size());
    
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// NodeExpandVolume
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeExpandVolume(ServerContext *context,
                                  NodeExpandVolumeRequest const *req,
                                  NodeExpandVolumeResponse *rsp)
{
    LOG_F(ERROR, "Node expand volume is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// NodeGetCapabilities
////////////////////////////////////////////////////////////////////////
Status NodeImpl::NodeGetCapabilities(ServerContext *context,
                                     NodeGetCapabilitiesRequest const *req,
                                     NodeGetCapabilitiesResponse *rsp)
{
    rsp->add_capabilities()->mutable_rpc()->set_type(NodeServiceCapability::RPC::Type::NodeServiceCapability_RPC_Type_GET_VOLUME_STATS);
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
