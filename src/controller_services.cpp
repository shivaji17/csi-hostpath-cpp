#include <controller_services.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>

using namespace std;
using namespace grpc;
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
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// DeleteVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::DeleteVolume(ServerContext *context,
                                    DeleteVolumeRequest const *req,
                                    DeleteVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerPublishVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerPublishVolume(ServerContext *context,
                                               ControllerPublishVolumeRequest const *req,
                                               ControllerPublishVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerUnpublishVolume
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerUnpublishVolume(ServerContext *context,
                                                 ControllerUnpublishVolumeRequest const *req,
                                                 ControllerUnpublishVolumeResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ValidateVolumeCapabilities
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ValidateVolumeCapabilities(ServerContext *context,
                                                  ValidateVolumeCapabilitiesRequest const *req,
                                                  ValidateVolumeCapabilitiesResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ListVolumes
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ListVolumes(ServerContext *context,
                                   ListVolumesRequest const *req,
                                   ListVolumesResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// GetCapacity
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::GetCapacity(ServerContext *context,
                                   GetCapacityRequest const *req,
                                   GetCapacityResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ControllerGetCapabilities
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ControllerGetCapabilities(ServerContext *context,
                                                 ControllerGetCapabilitiesRequest const *req,
                                                 ControllerGetCapabilitiesResponse *rsp)
{
    using cap = ControllerServiceCapability::RPC::Type;
    auto *capabilities = rsp->mutable_capabilities();
    capabilities->Add()->mutable_rpc()->set_type(cap::ControllerServiceCapability_RPC_Type_CREATE_DELETE_VOLUME);
    capabilities->Add()->mutable_rpc()->set_type(cap::ControllerServiceCapability_RPC_Type_LIST_VOLUMES);
    capabilities->Add()->mutable_rpc()->set_type(cap::ControllerServiceCapability_RPC_Type_GET_CAPACITY);
    capabilities->Add()->mutable_rpc()->set_type(cap::ControllerServiceCapability_RPC_Type_GET_VOLUME);
    capabilities->Add()->mutable_rpc()->set_type(cap::ControllerServiceCapability_RPC_Type_CLONE_VOLUME);
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// CreateSnapshot
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::CreateSnapshot(ServerContext *context,
                                      CreateSnapshotRequest const *req,
                                      CreateSnapshotResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// DeleteSnapshot
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::DeleteSnapshot(ServerContext *context,
                                      DeleteSnapshotRequest const *req,
                                      DeleteSnapshotResponse *rsp)
{
    return Status::OK;
}

////////////////////////////////////////////////////////////////////////
// ListSnapshots
////////////////////////////////////////////////////////////////////////
Status ControllerImpl::ListSnapshots(ServerContext *context,
                                     ListSnapshotsRequest const *req,
                                     ListSnapshotsResponse *rsp)
{
    return Status::OK;
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
    return Status::OK;
}
