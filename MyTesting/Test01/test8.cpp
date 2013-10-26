#include <Windows.h>
#include "resource.h"
//using namespace std;
#include <string>
#include <sstream>
#include <iostream>

static const wchar_t g_szClassName[] = L"MyWindowsClass";
static HWND g_hToolbar = NULL;
static HBRUSH g_hbrBackground = CreateSolidBrush(RGB(0,0,0));

static BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			HINSTANCE hInst = GetModuleHandle(NULL);
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)));
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)));
		}
		break;
	case WM_CTLCOLORDLG:
		return (LONG)g_hbrBackground;
	case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255,255,255));
			//if you leave this line off the background will be filled in with the brush you specify, 
			//but when the control draws the text it will get written over with the default background 
			//color! Setting the text drawing mode to transparent fixes this problem. The other option 
			//would be to SetBkColor() to the same color as our background brush, but I like this solution better.
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LONG)g_hbrBackground;
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_BUTTON1: //fill random text
				//IDC_EDIT1
				SetDlgItemText(hwnd, IDC_EDIT1, L"Hello how are you?");
				break;
			case IDC_BUTTON2: // get text
				{
					int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT1));
					if (len > 0)
					{
						wchar_t* buf;
						//NOTE: len+1 because GetWindowTextLength returns number of characters (not including NULL). 
						buf = (wchar_t*)GlobalAlloc(GPTR, (len+1)*sizeof(wchar_t));
						//returns number of characters (not including NULL). 
						GetDlgItemText(hwnd, IDC_EDIT1, buf, len+1);

						MessageBox(hwnd, buf, L"Notice", MB_OK);

						GlobalFree((HANDLE)buf);
					}
				}
				break;
			case IDC_BUTTON4: // get number
				{
					//last param: signed
					BOOL translated;
					int num = GetDlgItemInt(hwnd, IDC_EDIT1, &translated, TRUE);
					if (translated)
					{
						std::wostringstream wostr;
						wostr << L"the number is " << num;
						MessageBox(hwnd, wostr.str().data(), L"Notice", MB_OK);
					}
					else
					{
						MessageBox(hwnd, L"not a number", L"Notice", MB_OK);
					}
				}
				break;
			case IDC_BUTTON5:
				{
					int index = SendDlgItemMessage(hwnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)L"Hi there!");
				}
				break;
			case IDC_LIST1:
				{
					switch(HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						//Selection changed, do stuff here
						MessageBox(hwnd, L"sel changed", L"Notice", MB_OK);
						break;
					}
				}
				break;
			case IDC_BUTTON6:
				{
					HWND hList = GetDlgItem(hwnd, IDC_LIST1);
					int index = SendMessage(hList, LB_GETCURSEL, 0,0);
					std::cout << index;
					//Note that static controls are all given a default ID of IDC_STATIC (-1) by the resource editor, 
					//so if you want to be able to tell them apart you'll need to assign them new IDs.
					SetDlgItemInt(hwnd, IDC_STATIC1, index, TRUE);
				}
				break;
			case IDOK:
				EndDialog(hwnd, IDOK);
				break;
			case IDCANCEL:
				EndDialog(hwnd, IDCANCEL);
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
			case ID_ABOUT40006:
				{
					int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hwnd, AboutDlgProc);
					if (ret == IDOK)
					{
						MessageBox(hwnd, L"OK was clicked", L"Notice", MB_OK | MB_ICONINFORMATION);
					}
					else if (ret == IDCANCEL)
					{
						MessageBox(hwnd, L"Cancel was clicked", L"Notice", MB_OK | MB_ICONINFORMATION);
					}
					else if (ret == -1)
					{
						MessageBox(hwnd, L"Error loading window", L"Error", MB_OK | MB_ICONEXCLAMATION);
					}
				}
				break;

			case ID_FILE_EXIT:
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
	wex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU2);
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
