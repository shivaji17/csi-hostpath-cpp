#ifndef CONTROLLER_SERVICES_H
#define CONTROLLER_SERVICES_H

#include <mutex>
#include <vector>
#include <csi.pb.h>
#include <csi.grpc.pb.h>
#include <csi_services.h>
#include <state.h>

namespace csi::services::controller
{

    class ControllerImpl final : public csi::v1::Controller::Service
    {
    public:
        ControllerImpl(hostpath::Config const &config, hostpath::state::State &state);
        ControllerImpl(ControllerImpl const &) = delete;
        ControllerImpl &operator=(ControllerImpl const &) = delete;
        ~ControllerImpl();

        grpc::Status CreateVolume(grpc::ServerContext *context,
                                  csi::v1::CreateVolumeRequest const *req,
                                  csi::v1::CreateVolumeResponse *rsp);

        grpc::Status DeleteVolume(grpc::ServerContext *context,
                                  csi::v1::DeleteVolumeRequest const *req,
                                  csi::v1::DeleteVolumeResponse *rsp);

        grpc::Status ControllerPublishVolume(grpc::ServerContext *context,
                                             csi::v1::ControllerPublishVolumeRequest const *req,
                                             csi::v1::ControllerPublishVolumeResponse *rsp);

        grpc::Status ControllerUnpublishVolume(grpc::ServerContext *context,
                                               csi::v1::ControllerUnpublishVolumeRequest const *req,
                                               csi::v1::ControllerUnpublishVolumeResponse *rsp);

        grpc::Status ValidateVolumeCapabilities(grpc::ServerContext *context,
                                                csi::v1::ValidateVolumeCapabilitiesRequest const *req,
                                                csi::v1::ValidateVolumeCapabilitiesResponse *rsp);

        grpc::Status ListVolumes(grpc::ServerContext *context,
                                 csi::v1::ListVolumesRequest const *req,
                                 csi::v1::ListVolumesResponse *rsp);

        grpc::Status GetCapacity(grpc::ServerContext *context,
                                 csi::v1::GetCapacityRequest const *req,
                                 csi::v1::GetCapacityResponse *rsp);

        grpc::Status ControllerGetCapabilities(grpc::ServerContext *context,
                                               csi::v1::ControllerGetCapabilitiesRequest const *req,
                                               csi::v1::ControllerGetCapabilitiesResponse *rsp);

        grpc::Status CreateSnapshot(grpc::ServerContext *context,
                                    csi::v1::CreateSnapshotRequest const *req,
                                    csi::v1::CreateSnapshotResponse *rsp);

        grpc::Status DeleteSnapshot(grpc::ServerContext *context,
                                    csi::v1::DeleteSnapshotRequest const *req,
                                    csi::v1::DeleteSnapshotResponse *rsp);

        grpc::Status ListSnapshots(grpc::ServerContext *context,
                                   csi::v1::ListSnapshotsRequest const *req,
                                   csi::v1::ListSnapshotsResponse *rsp);

        grpc::Status ControllerExpandVolume(grpc::ServerContext *context,
                                            csi::v1::ControllerExpandVolumeRequest const *req,
                                            csi::v1::ControllerExpandVolumeResponse *rsp);

        grpc::Status ControllerGetVolume(grpc::ServerContext *context,
                                         csi::v1::ControllerGetVolumeRequest const *req,
                                         csi::v1::ControllerGetVolumeResponse *rsp);

    private:
        bool IsControllerServiceRequestValid(csi::v1::ControllerServiceCapability_RPC_Type serviceType) const;
        std::vector<csi::v1::ControllerServiceCapability_RPC_Type> GetControllerServiceCapabilities() const;

    private:
        hostpath::Config const &m_config;
        hostpath::state::State &m_state;
        std::mutex m_mutex;
    };

}

#endif
