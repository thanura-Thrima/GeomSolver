#include <iostream>

#include "Application.h"
#include "Def.h"

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		LOGE("Error in command line argument list.\n");
		LOGI("Usage: napa.exe <dataPath> <command> (x,y)\n");
		LOGI("    <dataPath>  : data folder path with csv files.\n");
		LOGI("    <command>  : OBJECTS_AT_POINT or OVERLAPPING_OBJECTS\n");
		return 0;
	}
	GeomSolver::Application app(argv[1]);

	app.run(argc, argv);

	return 0;
}