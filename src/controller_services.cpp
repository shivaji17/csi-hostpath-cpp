#include <controller_services.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>
#include <hostpath.pb.h>
#include <utils.h>

using namespace std;
using namespace grpc;
using namespace utils;
using namespace hostpath;
using namespace hostpath::state;
using namespace csi::v1;
using namespace csi::services;
using namespace csi::services::controller;

ControllerImpl::ControllerImpl(Config const &config, State &state) : m_config(config), m_state(state)
{
}

ControllerImpl::~ControllerImpl()
{
}

////////////////////////////////////////////////////////////////////////
// CreateVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::CreateVolume(ServerContext *context,
                                    CreateVolumeRequest const *req,
                                    CreateVolumeResponse *rsp)
{
    if (!IsControllerServiceRequestValid(ControllerServiceCapability_RPC_Type::ControllerServiceCapability_RPC_Type_CREATE_DELETE_VOLUME))
    {
        LOG_F(ERROR, "Volume creation is not supported");
        return Status::CANCELLED;
    }

    if (req->name().empty())
    {
        LOG_F(ERROR, "Volume name missing in the request");
        return Status::CANCELLED;
    }

    auto &caps = req->volume_capabilities();
    if (caps.empty())
    {
        LOG_F(ERROR, "Volume capabilities missing in the request");
        return Status::CANCELLED;
    }

    for (auto &cap : caps)
    {
        if (cap.has_block())
        {
            LOG_F(ERROR, "Block type volume is not supported");
            return Status::CANCELLED;
        }
    }

    auto volSize = req->capacity_range().required_bytes();
    lock_guard<mutex> lock(m_state.GetMutex());
    HostPathVolume volume;
    if (m_state.GetVolumeByName(req->name(), volume))
    {
        LOG_F(ERROR, "Volume '%s' already exists", req->name().c_str());
        return Status::CANCELLED;
    }

    volume.set_volume_name(req->name());
    volume.set_volume_id(CreateUUID());
    volume.set_vol_size(volSize);
    string dirPath = m_config.state_directory() + "/" + volume.volume_id();
    if (CreateDirectory(dirPath))
    {
        LOG_F(ERROR, "Failed to create volume %s. Error: Failed to create directory", req->name().c_str());
        return Status::CANCELLED;
    }
    volume.set_directory_path(dirPath);

    LOG_F(INFO, "Successfully creaeted volume '%s'", req->name().c_str());

    m_state.UpdateVolume(volume);
    auto *vol = rsp->mutable_volume();
    vol->set_capacity_bytes(volSize);
    vol->set_volume_id(volume.volume_id());
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// DeleteVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::DeleteVolume(ServerContext *context,
                                    DeleteVolumeRequest const *req,
                                    DeleteVolumeResponse *rsp)
{
    if (!IsControllerServiceRequestValid(ControllerServiceCapability_RPC_Type::ControllerServiceCapability_RPC_Type_CREATE_DELETE_VOLUME))
    {
        LOG_F(ERROR, "Volume deletion is not supported");
        return Status::CANCELLED;
    }

    if (req->volume_id().empty())
    {
        LOG_F(ERROR, "Volume ID missing in request");
        return Status::CANCELLED;
    }

    lock_guard<mutex> lock(m_state.GetMutex());
    HostPathVolume volume;
    if (!m_state.GetVolumeByID(req->volume_id(), volume))
    {
        LOG_F(WARNING, "Volume with volume-id '%s' does not exists", req->volume_id().c_str());
        return Status::OK;
    }

    // FIXME Add checks to see if volume is used or not

    if (!m_state.DeleteVolumeByID(req->volume_id()))
    {
        LOG_F(ERROR, "Failed to delete volume with id '%s'", req->volume_id().c_str());
        return Status::CANCELLED;
    }

    LOG_F(INFO, "Successfully deleted volume with id '%s'", req->volume_id().c_str());
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerPublishVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerPublishVolume(ServerContext *context,
                                               ControllerPublishVolumeRequest const *req,
                                               ControllerPublishVolumeResponse *rsp)
{
    LOG_F(ERROR, "ControllerPublishVolume is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// ControllerUnpublishVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerUnpublishVolume(ServerContext *context,
                                                 ControllerUnpublishVolumeRequest const *req,
                                                 ControllerUnpublishVolumeResponse *rsp)
{
    LOG_F(ERROR, "ControllerUnpublishVolume is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// ValidateVolumeCapabilities
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ValidateVolumeCapabilities(ServerContext *context,
                                                  ValidateVolumeCapabilitiesRequest const *req,
                                                  ValidateVolumeCapabilitiesResponse *rsp)
{
    if (req->volume_id().empty())
    {
        LOG_F(ERROR, "Volume ID missing in request");
        return Status::CANCELLED;
    }

    if (req->volume_capabilities().size() == 0)
    {
        LOG_F(ERROR, "Invalid capabilities requested");
        return Status::CANCELLED;
    }

    lock_guard<mutex> lock(m_state.GetMutex());
    HostPathVolume volume;

    if (!m_state.GetVolumeByID(req->volume_id(), volume))
    {
        LOG_F(ERROR, "Volume with id '%s' does not exists", req->volume_id().c_str());
        return Status::CANCELLED;
    }

    bool capabilitiesProvided = false;
    for (auto const &cap : req->volume_capabilities())
    {
        if (cap.has_block())
        {
            LOG_F(ERROR, "Block volume type is not supported");
            return Status::CANCELLED;
        }
        else if (cap.has_mount())
        {
            capabilitiesProvided = true;
        }
    }

    if (!capabilitiesProvided)
    {
        LOG_F(ERROR, "No capabilities provided in request");
        return Status::CANCELLED;
    }

    rsp->mutable_confirmed()->mutable_volume_capabilities()->CopyFrom(req->volume_capabilities());
    *(rsp->mutable_confirmed()->mutable_volume_context()) = req->volume_context();
    *(rsp->mutable_confirmed()->mutable_parameters()) = req->parameters();
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ListVolumes
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ListVolumes(ServerContext *context,
                                   ListVolumesRequest const *req,
                                   ListVolumesResponse *rsp)
{
    lock_guard<mutex> lock(m_state.GetMutex());

    auto CopyVolumes = [&](vector<HostPathVolume> const &volumeList) -> void
    {
        auto *entries = rsp->mutable_entries();
        for (auto const &vol : volumeList)
        {
            auto rspVol = entries->Add();
            rspVol->mutable_volume()->set_volume_id(vol.volume_id());
            rspVol->mutable_volume()->set_capacity_bytes(vol.vol_size());
            rspVol->mutable_status()->add_published_node_ids(m_config.node_name());

            // FIXME Add volume condition
        }
    };

    if (req->starting_token().empty())
    {
        auto [volumeList, token] = m_state.GetVolumeList(req->max_entries());

        if (!token.empty())
            rsp->set_next_token(token);

        CopyVolumes(volumeList);
    }
    else
    {
        vector<HostPathVolume> volumeList;
        if (!m_state.GetVolumeListForGivenToken(req->starting_token(), volumeList, req->max_entries()))
        {
            LOG_F(ERROR, "Failed to fetch volumes: Error: %s", m_state.GetLastError().c_str());
            return Status::CANCELLED;
        }

        CopyVolumes(volumeList);
    }
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// GetCapacity
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::GetCapacity(ServerContext *context,
                                   GetCapacityRequest const *req,
                                   GetCapacityResponse *rsp)
{
    lock_guard<mutex> lock(m_state.GetMutex());

    auto [capacity, available] = GetDirectorySpace(m_config.state_directory());
    rsp->set_available_capacity(available);
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerGetCapabilities
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerGetCapabilities(ServerContext *context,
                                                 ControllerGetCapabilitiesRequest const *req,
                                                 ControllerGetCapabilitiesResponse *rsp)
{
    auto *capabilities = rsp->mutable_capabilities();
    for (auto const cap : GetControllerServiceCapabilities())
        capabilities->Add()->mutable_rpc()->set_type(cap);
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// CreateSnapshot
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::CreateSnapshot(ServerContext *context,
                                      CreateSnapshotRequest const *req,
                                      CreateSnapshotResponse *rsp)
{
    LOG_F(ERROR, "Create Snapshot is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// DeleteSnapshot
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::DeleteSnapshot(ServerContext *context,
                                      DeleteSnapshotRequest const *req,
                                      DeleteSnapshotResponse *rsp)
{
    LOG_F(ERROR, "Delete Snapshot is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// ListSnapshots
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ListSnapshots(ServerContext *context,
                                     ListSnapshotsRequest const *req,
                                     ListSnapshotsResponse *rsp)
{
    LOG_F(ERROR, "List Snapshot is not supported");
    return Status::CANCELLED;
}

////////////////////////////////////////////////////////////////////////
// ControllerExpandVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerExpandVolume(ServerContext *context,
                                              ControllerExpandVolumeRequest const *req,
                                              ControllerExpandVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerGetVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerGetVolume(ServerContext *context,
                                           ControllerGetVolumeRequest const *req,
                                           ControllerGetVolumeResponse *rsp)
{
    if (req->volume_id().empty())
    {
        LOG_F(ERROR, "Volume ID is missing in request");
        return Status::CANCELLED;
    }

    HostPathVolume volume;
    if (!m_state.GetVolumeByID(req->volume_id(), volume))
    {
        LOG_F(ERROR, "Volume with id '%s' does not exists", req->volume_id().c_str());
        return Status::CANCELLED;
    }

    rsp->mutable_volume()->set_volume_id(volume.volume_id());
    rsp->mutable_volume()->set_capacity_bytes(volume.vol_size());
    rsp->mutable_status()->add_published_node_ids(m_config.node_name());
    return Status::OK;
}

bool ControllerImpl::IsControllerServiceRequestValid(ControllerServiceCapability_RPC_Type serviceType) const
{
    for (auto const cap : GetControllerServiceCapabilities())
    {
        if (cap == serviceType)
            return true;
    }
    return false;
}

vector<ControllerServiceCapability_RPC_Type> ControllerImpl::GetControllerServiceCapabilities() const
{
    using cap = ControllerServiceCapability::RPC::Type;
    return std::vector<ControllerServiceCapability_RPC_Type>{
        cap::ControllerServiceCapability_RPC_Type_CREATE_DELETE_VOLUME,
        cap::ControllerServiceCapability_RPC_Type_LIST_VOLUMES,
        cap::ControllerServiceCapability_RPC_Type_GET_CAPACITY,
        cap::ControllerServiceCapability_RPC_Type_GET_VOLUME,
        cap::ControllerServiceCapability_RPC_Type_CLONE_VOLUME};
}
