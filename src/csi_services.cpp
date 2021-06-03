#include <csi_services.h>
#include <functional>
#include <sstream>
#include <loguru/loguru.hpp>
#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <csi.pb.h>
#include <csi.grpc.pb.h>
#include <controller_services.h>
#include <identity_services.h>
#include <node_services.h>
#include <state.h>

using namespace std;
using namespace grpc;
using namespace hostpath;
using namespace hostpath::state;
using namespace csi::v1;
using namespace csi::services;
using namespace csi::services::node;
using namespace csi::services::identity;
using namespace csi::services::controller;

static const string STATE_FILE = "state.bin";

CSIServices::CSIServices(Config config) : m_config(config)
{
}

CSIServices::~CSIServices()
{
}

bool CSIServices::Run()
{
    State state(m_config.state_directory() + "/" + STATE_FILE);
    state.Init();

    IdentityImpl identitySvc(m_config);
    NodeImpl nodeSvc(m_config, state);
    ControllerImpl controllerSvc(m_config, state);

    ServerBuilder builder;

    builder.AddListeningPort(m_config.endpoint(), InsecureServerCredentials());
    builder.RegisterService(&identitySvc);
    builder.RegisterService(&nodeSvc);
    builder.RegisterService(&controllerSvc);

    unique_ptr<Server> server(builder.BuildAndStart());
    LOG_F(INFO, "Server listening at '%s'", m_config.endpoint().c_str());
    server->Wait();
    return true;
}
