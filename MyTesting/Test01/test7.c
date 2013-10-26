#include <Windows.h>
#include "resource.h"

static const wchar_t g_szClassName[] = L"MyWindowsClass";
static HWND g_hToolbar = NULL;

static BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDOK:
				MessageBox(hwnd, L"OK was clicked", L"Notice", MB_OK | MB_ICONINFORMATION);
				break;
			case IDCANCEL:
				MessageBox(hwnd, L"Cancel was clicked", L"Notice", MB_OK | MB_ICONINFORMATION);
				break;
			}
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//LRESULT = LONG_PTR
//CALLBACK = __stdcall
//used for passing and returning values
//wParam = UINT_PTR
//lParam = LONG_PTR
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
		{
			g_hToolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hwnd, AboutDlgProc);
			if (g_hToolbar != NULL)
			{
				ShowWindow(g_hToolbar, SW_SHOW);
			}
			else
			{
				MessageBox(hwnd, L"Dialog create failure (returned NULL)!", L"Error", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			wchar_t wszFileName[MAX_PATH];
			HINSTANCE hInst = GetModuleHandle(NULL);
			GetModuleFileName(hInst, wszFileName, MAX_PATH);
			MessageBox(hwnd, wszFileName, L"This program is:", MB_OK | MB_ICONINFORMATION);
		}
		break;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case ID_ABOUT_SHOW:
				{
					if (g_hToolbar != NULL) ShowWindow(g_hToolbar, SW_SHOW);
				}
				break;
			case ID_ABOUT_HIDE:
				{
					if (g_hToolbar != NULL) ShowWindow(g_hToolbar, SW_HIDE);
				}
				break;
			case ID_FILE_EX:
				DestroyWindow(hwnd);
				break;
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (g_hToolbar != NULL) DestroyWindow(g_hToolbar);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdToShow)
{
	WNDCLASSEX wex = {0};
	HWND hwnd;
	MSG msg;
	// Register the window class
	wex.cbSize = sizeof(WNDCLASSEX);
	wex.style = 0;
	wex.lpfnWndProc = WndProc;
	wex.cbClsExtra = 0; //extra data per class
	wex.cbWndExtra = 0; //extra data per window of this class 
	wex.hInstance = hInst; //handle to app instance
	wex.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); //using resources
	wex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wex.lpszClassName = g_szClassName;
	wex.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)); // using resources

	if (!RegisterClassEx(&wex))
	{
		//first argument null: no owner window
		MessageBox(NULL, L"Window registration failed", L"Error", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	// Create the window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE, // client area has sunken border
		g_szClassName,
		L"Sample window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		240,
		120,
		NULL, //hWndParent: a handle to parent or owner window
		NULL, //hMenu: can be NULL if class menu is used
		hInst,
		NULL); //pointer to window creation data (rarely used)

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Create window failed", L"", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Handle message loop

	//if hWnd (second parameter) is NULL, it retrieves messages for any window
	//belonging to current thread or any message on the current thread with hwnd value of NULL
	//if hwnd is -1, then only thread messages
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!IsDialogMessage(g_hToolbar, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//while (GetMessage(&msg, NULL, 0, 0) > 0)
	//{
	//	WNDPROC fWndProc = (WNDPROC)GetWindowLong(msg.hwnd, GWL_WNDPROC);
	//	if (fWndProc != NULL) fWndProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	//}

	// set by PostQuitMessage.
	return msg.wParam;
}
