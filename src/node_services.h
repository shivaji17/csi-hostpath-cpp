#ifndef NODE_SERVICES_H
#define NODE_SERVICES_H

#include <loguru/loguru.hpp>
#include <csi.pb.h>
#include <csi.grpc.pb.h>
#include <hostpath.pb.h>
#include <state.h>

namespace csi::services::node
{

    class NodeImpl final : public csi::v1::Node::Service
    {
    public:
        NodeImpl(hostpath::Config const &config);
        NodeImpl(NodeImpl const &) = delete;
        NodeImpl &operator=(NodeImpl const &) = delete;
        ~NodeImpl();

        grpc::Status NodeStageVolume(grpc::ServerContext *context,
                                     csi::v1::NodeStageVolumeRequest const *req,
                                     csi::v1::NodeStageVolumeResponse *rsp);

        grpc::Status NodeUnstageVolume(grpc::ServerContext *context,
                                       csi::v1::NodeUnstageVolumeRequest const *req,
                                       csi::v1::NodeUnstageVolumeResponse *rsp);

        grpc::Status NodePublishVolume(grpc::ServerContext *context,
                                       csi::v1::NodePublishVolumeRequest const *req,
                                       csi::v1::NodePublishVolumeResponse *rsp);

        grpc::Status NodeUnpublishVolume(grpc::ServerContext *context,
                                         csi::v1::NodeUnpublishVolumeRequest const *req,
                                         csi::v1::NodeUnpublishVolumeResponse *rsp);

        grpc::Status NodeGetVolumeStats(grpc::ServerContext *context,
                                        csi::v1::NodeGetVolumeStatsRequest const *req,
                                        csi::v1::NodeGetVolumeStatsResponse *rsp);

        grpc::Status NodeExpandVolume(grpc::ServerContext *context,
                                      csi::v1::NodeExpandVolumeRequest const *req,
                                      csi::v1::NodeExpandVolumeResponse *rsp);

        grpc::Status NodeGetCapabilities(grpc::ServerContext *context,
                                         csi::v1::NodeGetCapabilitiesRequest const *req,
                                         csi::v1::NodeGetCapabilitiesResponse *rsp);

        grpc::Status NodeGetInfo(grpc::ServerContext *context,
                                 csi::v1::NodeGetInfoRequest const *req,
                                 csi::v1::NodeGetInfoResponse *rsp);

    private:
        hostpath::Config const &m_config;
    };

}

#endif
