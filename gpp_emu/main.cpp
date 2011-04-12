#include "stdafx.h"

/* -------------------------------------------------------------------------------------
 *
 * -------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
	if( argc <= 1 )
	{
		printf("gpp_hack <plugin filename>\n");
		return 0;
	}

	CGritterPanelEmulator gpe;

	gpe.initialize( NULL, NULL, argv[1] );

	while( _kbhit() == 0 )
	{
		gpe.workFrame();
		gpe.draw( 0 );
		Sleep(500);
	}

	gpe.terminate();

	Sleep(2000);

	return 0;
}

