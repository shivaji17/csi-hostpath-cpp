#include <csi_services.h>
#include <functional>
#include <sstream>
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

using namespace std;
using namespace grpc;
using namespace csi::v1;
using namespace csi::services;
using namespace csi::services::node;
using namespace csi::services::identity;
using namespace csi::services::controller;

CSIServices::CSIServices(Config config) : m_config(config)
{
}

CSIServices::~CSIServices()
{
}

bool CSIServices::Run()
{
    IdentityImpl identitySvc(m_config);
    NodeImpl nodeSvc;
    ControllerImpl controllerSvc;

    ServerBuilder builder;

    builder.AddListeningPort(m_config.endpoint, InsecureServerCredentials());
    builder.RegisterService(&identitySvc);
    builder.RegisterService(&nodeSvc);
    builder.RegisterService(&controllerSvc);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout << "Server listening on " << m_config.endpoint << std::endl;
    server->Wait();
    return true;
}
