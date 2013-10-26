#include <Windows.h>

static const wchar_t g_szClassName[] = L"MyWindowsClass";

//LRESULT = LONG_PTR
//CALLBACK = __stdcall
//used for passing and returning values
//wParam = UINT_PTR
//lParam = LONG_PTR
static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain2(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdToShow)
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
	wex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wex.lpszMenuName = NULL;
	wex.lpszClassName = g_szClassName;
	wex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// set by PostQuitMessage.
	return msg.wParam;
}
