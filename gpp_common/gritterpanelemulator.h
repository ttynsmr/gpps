#pragma once

/* -------------------------------------------------------------------------------------
 *
 * -------------------------------------------------------------------------------------*/
class CGritterPanelEmulator
{
public:
	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	CGritterPanelEmulator()
	{
		m_hModule = NULL;
		GppDraw = NULL;
		GppInfo = NULL;
		GppCreate = NULL;
		GppRelease = NULL;
		GppStart = NULL;
		GppStop = NULL;
		GppWorkFrame = NULL;
	}
	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	virtual ~CGritterPanelEmulator()
	{
	}

	/* -------------------------------------------------------------------------------------
	 * szPluginFilename: plugin filename(*.gpp)
	 * -------------------------------------------------------------------------------------*/
	bool initialize( HINSTANCE hInstance, HWND hWnd, const char* szPluginFilename )
	{
		m_hModule = LoadLibrary( szPluginFilename );
		if( m_hModule == NULL )
		{
			return false;
		}

		GppDraw		= (tf_GppDraw)		GetProcAddress( m_hModule, "GppDraw" );
		if( GppDraw == NULL )
		{
			return false;
		}
		GppInfo		= (tf_GppInfo)		GetProcAddress( m_hModule, "GppInfo" );
		if( GppInfo == NULL )
		{
			return false;
		}
		GppCreate	= (tf_GppCreate)	GetProcAddress( m_hModule, "GppCreate" );
		if( GppCreate == NULL )
		{
			return false;
		}
		GppRelease	= (tf_GppRelease)	GetProcAddress( m_hModule, "GppRelease" );
		if( GppRelease == NULL )
		{
			return false;
		}
		GppStart	= (tf_GppStart)		GetProcAddress( m_hModule, "GppStart" );
		if( GppStart == NULL )
		{
			return false;
		}
		GppStop		= (tf_GppStop)		GetProcAddress( m_hModule, "GppStop" );
		if( GppStop == NULL )
		{
			return false;
		}
		GppWorkFrame= (tf_GppWorkFrame)	GetProcAddress( m_hModule, "GppWorkFrame" );
		if( GppWorkFrame == NULL )
		{
			return false;
		}
		
		int iResult = 0;

		iResult = GppInfo( m_szGppInfo, sizeof(m_szGppInfo) );
		printf("[GppInfo] [%s], result=%d\n", m_szGppInfo, iResult);
		if( iResult != 0 )
		{
			return false;
		}

		iResult = GppCreate(hInstance, hWnd);
		printf("[GppCreate] result=%d\n", iResult);
		if( iResult != 0 )
		{
			return false;
		}

		iResult = GppStart();
		printf("[GppStart] result=%d\n", iResult);
		if( iResult != 0 )
		{
			return false;
		}

		return true;
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	void terminate()
	{
		int iResult = 0;

		iResult = GppStop();
		printf("[GppStop] result=%d\n", iResult);

		iResult = GppRelease();
		printf("[GppRelease] result=%d\n", iResult);

		if( m_hModule == NULL )
		{
			return;
		}
		FreeLibrary( m_hModule );
		m_hModule = NULL;
		GppDraw = NULL;
		GppInfo = NULL;
		GppCreate = NULL;
		GppRelease = NULL;
		GppStart = NULL;
		GppStop = NULL;
		GppWorkFrame = NULL;

		printf("CGritterPanel terminate successful\n");
	}

	/* -------------------------------------------------------------------------------------
	 *
	 * -------------------------------------------------------------------------------------*/
	bool workFrame()
	{
		if( m_hModule == NULL )
		{
			return false;
		}

		int iResult = GppWorkFrame();
		printf("[GppWorkFrame] result=%d\n", iResult);

		if( iResult == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/* -------------------------------------------------------------------------------------
	 *	mode:	0 = ALL
	 *			1 = Upper regeon
	 *			2 = Lower region
	 * -------------------------------------------------------------------------------------*/
	virtual void draw( unsigned int mode )
	{
		if( m_hModule == NULL )
		{
			return;
		}

		int iResult = GppDraw( m_bitImage.getImage(), mode, 0, 0, 0 );
		printf("[GppDraw] result=%d\n", iResult);
		printf("-0-1-2-3-4-5-6-7-8-9-A-B-C-D-E-F\n");
		for(int y=0; y<16; y++)
		{
			for(int x=0; x<16; x++)
			{
				if( m_bitImage.getPixel( x, y ) )
				{
					printf("œ");
				}
				else
				{
					printf("@");
				}
			}
			printf("\n");
		}
	}

protected:
	HMODULE			m_hModule;
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;

	// args: bitmap pointer, mode, unknown, unknown, unknown
	typedef	int (__stdcall *tf_GppDraw) (unsigned char*,int,int,int,int);

	// args: string buffer pointer, string buffer pointer size
	typedef	int (__stdcall *tf_GppInfo) (char*,int);

	// args: unknown, unknown
	typedef	int (__stdcall *tf_GppCreate) (HINSTANCE,HWND);

	typedef	int (__stdcall *tf_GppRelease) (void);
	typedef	int (__stdcall *tf_GppStart) (void);
	typedef	int (__stdcall *tf_GppStop) (void);
	typedef	int (__stdcall *tf_GppWorkFrame) (void);

	tf_GppDraw		GppDraw;
	tf_GppInfo		GppInfo;
	tf_GppCreate	GppCreate;
	tf_GppRelease	GppRelease;
	tf_GppStart		GppStart;
	tf_GppStop		GppStop;
	tf_GppWorkFrame	GppWorkFrame;

	char			m_szGppInfo[256+1];
	CBitImage		m_bitImage;
};
