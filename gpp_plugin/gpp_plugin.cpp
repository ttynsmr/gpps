#include <windows.h>
#include "../gpp_common/gritterpanel.h"

static CBitImage image;

extern "C" {

// args: bitmap pointer, mode, unknown, unknown, unknown
int __stdcall GppDraw( unsigned char* pImg, int mode, int unknown1, int unknown2, int unknown3 )
{
	switch( mode )
	{
	case 0:
		image.setPixel( 1, 1, !image.getPixel( 1, 1 ) );
		image.copyTo( pImg, 0 );
		break;
	case 1:
		image.setPixel( 2, 2, !image.getPixel( 2, 2 ) );
		image.copyTo( pImg, 1 );
		break;
	case 2:
		image.setPixel( 3, 3, !image.getPixel( 3, 3 ) );
		image.copyTo( pImg, 2 );
		break;
	}

	return 0;
}

// args: string buffer pointer, string buffer pointer size
int __stdcall GppInfo( char* pszDest, int iDestSize )
{
	strncpy_s( pszDest, iDestSize, "test plugin", iDestSize );
	return 0;
}

// args: unknown, unknown
int __stdcall GppCreate( int unknown1, int unknown2 )
{
	return 0;
}

int __stdcall GppRelease(void)
{
	return 0;
}

int __stdcall GppStart(void)
{
	return 0;
}

int __stdcall GppStop(void)
{
	return 0;
}

int __stdcall GppWorkFrame(void)
{
	return 0;
}

}
