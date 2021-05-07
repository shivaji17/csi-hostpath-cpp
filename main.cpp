#include <iostream>
#include <getopt.h>
#include <csi_services.h>

using namespace std;
using namespace csi::services;

Config ParseCommandLine( int argc, char* argv[] )
{
	struct option opts[] = {
		{ "help",          no_argument,       0,  0   },
		{ "version",       no_argument,       0,  1   },
		{ "endpoint",      required_argument, 0,  'e' },
		{ "nodeid",        required_argument, 0,  'n' },
		{ 0, 0, 0, 0 }
	};

	Config config;
	while( true )
	{
		int optionIndex = 0;
		int c = getopt_long( argc, argv, "e:n:", opts, &optionIndex );
		if( c == -1 )
			break;

		switch( c )
		{
			case 0:
				cout << "Help...!!!" << endl;
				exit(1);

			case 1:
				cout << "CSI HostPath version: 0.0.1 " << endl;
				exit(0);

			case 'e':
				config.endpoint = string( optarg );
				break;

			case 'n':
				config.nodeName = string( optarg );

			case '?':
				cout << "Invalid option provided" << endl;
				break;
		}
	}

	return config;
}

bool ValidateConfig( Config const& config )
{
	return true;
}

int main( int argc, char* argv[] )
{
	auto config = ParseCommandLine( argc, argv );

	if( !ValidateConfig( config ) )
	{
		exit(1);
	}

	CSIServices csiService( config );
	csiService.Run();
	return 0;
}
