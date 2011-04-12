#include "stdafx.h"
#include "main.h"
#define printf
#include "../gpp_common/gritterpanel.h"
#include "../gpp_common/gritterpanelemulator.h"

class CGritterPanelEmulatorGUI : public CGritterPanelEmulator
{
public:
	CGritterPanelEmulatorGUI();
	~CGritterPanelEmulatorGUI();
	
	void draw( unsigned int mode, HDC hDC );
};

CGritterPanelEmulatorGUI::CGritterPanelEmulatorGUI()
{
}

CGritterPanelEmulatorGUI::~CGritterPanelEmulatorGUI()
{
}

void CGritterPanelEmulatorGUI::draw( unsigned int mode, HDC hDC )
{
	if( m_hModule == NULL )
	{
		return;
	}

	int iResult = GppDraw( m_bitImage.getImage(), mode, 0, 0, 0 );

	//SelectObject(hDC , GetStockObject(BLACK_BRUSH));
	//Rectangle(hDC , 0 , 0 , 256 , 256 );

	for(int y=0; y<16; y++)
	{
		for(int x=0; x<16; x++)
		{
			if( m_bitImage.getPixel( x, y ) )
			{
				SelectObject(hDC , GetStockObject(WHITE_BRUSH));
				Ellipse(hDC , x*16 , y*16 , x*16+16 , y*16+16);
			}
			else
			{
				SelectObject(hDC , GetStockObject(DKGRAY_BRUSH));
				Ellipse(hDC , x*16 , y*16 , x*16+16 , y*16+16);
			}
		}
	}
}

CGritterPanelEmulatorGUI gpegui;

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if( __argc == 1 )
	{
		return 0;
	}

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GPP_EMUGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GPP_EMUGUI));

	gpegui.initialize( hInstance, hWnd, __argv[1] );

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GPP_EMUGUI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GPP_EMUGUI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

	hWnd = CreateWindow(szWindowClass, szTitle,
		WS_OVERLAPPED     |
        WS_CAPTION        |
        WS_SYSMENU        |
        WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	{
		int iWidth = 256;
		int iHeight = 256;
		DWORD dwStyle   = (DWORD)GetWindowLong( hWnd, GWL_STYLE );
		DWORD dwExStyle = (DWORD)GetWindowLong( hWnd, GWL_EXSTYLE );
		BOOL  bMenu     = GetMenu( hWnd ) != NULL;

		RECT rectWindowClient = { 0 };
		rectWindowClient.right  = (LONG)iWidth;
		rectWindowClient.bottom = (LONG)iHeight;

		AdjustWindowRectEx( &rectWindowClient, dwStyle, bMenu, dwExStyle );
		SetWindowPos( hWnd, HWND_TOP, 0, 0, rectWindowClient.right - rectWindowClient.left, rectWindowClient.bottom - rectWindowClient.top, SWP_NOMOVE | SWP_NOZORDER );
		UpdateWindow( hWnd );
	}
	UpdateWindow(hWnd);
	ShowWindow(hWnd, nCmdShow);

	SetTimer( hWnd, 1, 100, NULL );

	return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		gpegui.workFrame();
		hdc = BeginPaint(hWnd, &ps);
		gpegui.draw( 0, hdc );
		EndPaint(hWnd, &ps);
		break;

	case WM_TIMER:
		/*
		hdc = BeginPaint(hWnd, &ps);
		SelectObject(hdc , GetStockObject(GRAY_BRUSH));
		Rectangle(hdc , 10 , 10 , 200 , 50);
		EndPaint(hWnd, &ps);*/
		{
			RECT rect = { 0, 0, 255, 255 };
			InvalidateRect(hWnd , &rect , FALSE );
			PostMessage( hWnd, WM_PAINT, 0, 0 );
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// バージョン情報ボックスのメッセージ ハンドラです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
