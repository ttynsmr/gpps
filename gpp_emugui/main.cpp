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

// �O���[�o���ϐ�:
HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
HWND hWnd;
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂�:
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

 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GPP_EMUGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GPP_EMUGUI));

	gpegui.initialize( hInstance, hWnd, __argv[1] );

	// ���C�� ���b�Z�[�W ���[�v:
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
//  �֐�: MyRegisterClass()
//
//  �ړI: �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g:
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//    Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
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
//   �֐�: InitInstance(HINSTANCE, int)
//
//   �ړI: �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g:
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

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
//  �֐�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �ړI:  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
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
		// �I�����ꂽ���j���[�̉��:
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

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
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
