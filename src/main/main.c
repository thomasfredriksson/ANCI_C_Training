#include "unity_fixture.h"
#include "winsockmulticast_IP_TTL.h"
#include "winsockheaders.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	return main_WSAEnumNameSpaceProviders(argc, argv);
	//return main_so_type(argc, argv);
}

