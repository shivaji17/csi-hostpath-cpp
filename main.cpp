#include <iostream>
#include <getopt.h>
#include <loguru/loguru.hpp>
#include <csi_services.h>

using namespace std;
using namespace csi::services;

Config ParseCommandLine(int argc, char *argv[])
{
    struct option opts[] = {
        {"help", no_argument, 0, 0},
        {"version", no_argument, 0, 1},
        {"endpoint", required_argument, 0, 'e'},
        {"nodeid", required_argument, 0, 'n'},
        {"driver-name", required_argument, 0, 'd'},
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
            cout << "Help...!!!" << endl;
            exit(1);

        case 1:
            cout << "CSI HostPath version: 0.0.1 " << endl;
            exit(0);

        case 'e':
            config.endpoint = string(optarg);
            break;

        case 'n':
            config.nodeName = string(optarg);
            break;

        case 'd':
            config.driverName = string(optarg);
            break;

        case '?':
            cout << "Invalid option provided" << endl;
            break;
        }
    }

    return config;
}

bool ValidateConfig(Config &config)
{
    if (config.nodeName.empty())
    {
        cout << "Node name cannot be empty" << endl;
        return false;
    }

    if (config.endpoint.empty())
    {
        config.endpoint = "unix://tmp/csi.sock";
    }
    else
    {
        // TODO Add IP address validation
    }

    if (config.driverName.empty())
        config.driverName = "hostpath-cpp.csi.k8s.io";

    // FIXME: Set version at runtime
    config.vendorVersion = "0.0.1";

    return true;
}

int main(int argc, char *argv[])
{
    auto config = ParseCommandLine(argc, argv);

    if (!ValidateConfig(config))
    {
        exit(1);
    }

    CSIServices csiService(config);
    csiService.Run();
    return 0;
}
