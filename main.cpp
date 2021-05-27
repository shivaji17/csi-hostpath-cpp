#include <iostream>
#include <getopt.h>
#include <loguru/loguru.hpp>
#include <HostPathCPPConfig.h>
#include <utils.h>
#include <csi_services.h>

using namespace std;
using namespace utils;
using namespace hostpath;
using namespace csi::services;

void Usage(int retVal)
{
    cout
        << "Usage: ./hostpath [OPTIONS]\n"
        << "\n"
        << "\t--help                 Print this help information and exit\n"
        << "\t--version              Print CSI HostPath version and exit\n"
        << "\t--endpoint=ENDPOINT    CSI endpoint (default='unix://tmp/csi.sock')\n"
        << "\t--nodeid=NODENAME      node id\n"
        << "\t--state-dir=DIRECTORY  directory for storing state information across driver restarts, volumes and snapshots (default=/csi-data-dir)"
        << endl;

    exit(retVal);
}

Config ParseCommandLine(int argc, char *argv[])
{
    struct option opts[] = {
        {"help", no_argument, 0, 0},
        {"version", no_argument, 0, 1},
        {"endpoint", required_argument, 0, 'e'},
        {"nodeid", required_argument, 0, 'n'},
        {"driver-name", required_argument, 0, 'd'},
        {"state-dir", required_argument, 0, 's'},
        {0, 0, 0, 0}};

    Config config;
    while (true)
    {
        int optionIndex = 0;
        int c = getopt_long(argc, argv, "e:n:", opts, &optionIndex);
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            Usage(0);

        case 1:
            cout
                << "CSI HostPath version: "
                << HostPathCPP_VERSION_MAJOR << "."
                << HostPathCPP_VERSION_MINOR << "."
                << HostPathCPP_VERSION_PATCH
                << endl;
            exit(0);

        case 'e':
            config.set_endpoint(string(optarg));
            break;

        case 'n':
            config.set_node_name(string(optarg));
            break;

        case 'd':
            config.set_driver_name(string(optarg));
            break;

        case 's':
            config.set_state_directory(string(optarg));
            break;

        case '?':
            cout << "Invalid option provided" << endl;
            Usage(1);
        }
    }

    return config;
}

bool ValidateConfig(Config &config)
{
    if (config.node_name().empty())
    {
        LOG_F(ERROR, "Node name cannot be empty. Please provide node name with '--nodeid' input");
        return false;
    }
    else if (!IsNameValid(config.node_name()))
    {
        LOG_F(ERROR, "Invalid node name '%s'", config.node_name().c_str());
        return false;
    }

    if (config.endpoint().empty())
    {
        LOG_F(INFO, "Endpoint not provided. Using endpoint 'unix://tmp/csi.sock'");
        config.set_endpoint("unix://tmp/csi.sock");
    }
    else
    {
        if (!IsUnixSocket(config.endpoint()) && !IsValidIPaddress(config.endpoint()))
        {
            LOG_F(ERROR, "Invalid endpoint address'%s'", config.endpoint().c_str());
            return false;
        }
    }

    if (config.driver_name().empty())
    {
        LOG_F(INFO, "CSI driver name not provided. Using default drivername 'hostpath-cpp.csi.k8s.io'");
        config.set_driver_name("hostpath-cpp.csi.k8s.io");
    }

    if (config.state_directory().empty())
    {
        config.set_state_directory("/csi-data-dir");
    }
    else
    {
        if (!DirectoryExists(config.state_directory()) && !CreateDirectory(config.state_directory()))
        {
            LOG_F(ERROR, "Failed to create state directory '%s'", config.state_directory().c_str());
            return false;
        }
    }

    auto [capacity, available] = GetDirectorySpace(config.state_directory());
    config.set_max_capacity(capacity);

    config.set_vendor_version(to_string(HostPathCPP_VERSION_MAJOR) + "." + to_string(HostPathCPP_VERSION_MINOR) + "." + to_string(HostPathCPP_VERSION_PATCH));
    return true;
}

void InitializeLogger(int argc, char *argv[])
{
    loguru::init(argc, argv);
}

int main(int argc, char *argv[])
{
    auto config = ParseCommandLine(argc, argv);
    InitializeLogger(argc, argv);

    if (!ValidateConfig(config))
    {
        exit(1);
    }

    CSIServices csiService(config);
    csiService.Run();
    return 0;
}
