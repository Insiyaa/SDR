// SDR.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "framework.h"
#include "SDR.h"
#include "Windows.h"
#include "commdlg.h"
#include <stdio.h>

#define MAX_LOADSTRING 100
#define OPEN_FILE 1

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hEdit;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void AddControls(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// macro to prevent the compiler from generating warnings, due to unused variables.
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
	// To retrieve a string resource from a specified instance. 
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SDR, szWindowClass, MAX_LOADSTRING);
	// Registers the class of the application
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	// Simple declarations
	// Retrieve a keyboard accelerator from a resource file
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SDR));

    MSG msg;

    // Main message loop:
	// For dispatching messages from the message queue to the appropriate window
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
			// Translate virtual-key messages into character messages.
            TranslateMessage(&msg);
			// Dispatch messages to a window procedur
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	// Style of window
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // A pointer to a WNDPROC callback to use as a message listener
	wcex.lpfnWndProc    = WndProc;
	// A set of additional flag values
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
	// A handle to the current application instance
    wcex.hInstance      = hInstance;
	// A handle to an icon
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SDR));
	// A handle to an cursor
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	// A handle to a brush colour
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	// The menu name
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SDR);
	// This window's class name
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	// Takes a pointer to a WNDCLASSEX structure and returns a unique identifier for the window
    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	// A handle to our main window
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void displayFile(wchar_t* path)
{
	FILE* file;
	errno_t err = _wfopen_s(&file, path, L"rb");
	if (err != 0)
	{
		MessageBoxW(NULL, NULL, L"Error opening file", MB_OK);
	}
	// going at the end of the file to know the file size
	fseek(file, 0, SEEK_END);
	int _size = ftell(file);
	rewind(file); // back to start
	//wchar_t data[1024];
	wchar_t* data = new wchar_t(_size + 1);
	fread(data, _size, 1, file);
	data[_size] = '\0';

	SetWindowTextW(hEdit, data);
}
void open_file(HWND hWNd)
{
	OPENFILENAME ofn;
	wchar_t fileBuffer[MAX_PATH] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWNd;
	ofn.lpstrFile = fileBuffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"All Files\0*.*\0CSV File\0*.CSV\0Text Files\0*.TXT\0";
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);
	// Print filename
	//MessageBoxW(NULL, ofn.lpstrFile, L"", MB_OK);

	displayFile(ofn.lpstrFile);

}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// hwnd - A handle to the window itself
    switch (message)
    {
	// Whenever a user interaction occurs
    case WM_COMMAND:
        {
			// LOWORD: Performs logical conjunction (AND) operation with value 0xffff which clears the upper 16 bits, leaving only the lower part unaffected.
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
			case OPEN_FILE:
				open_file(hWnd);
				break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CREATE:
		AddControls(hWnd);
		break;
	//  This event occurs everytime a window is redrawn
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
		// This leads us back to the message loop
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

void AddControls(HWND hWnd)
{
	// visible and type child of parent window
	CreateWindowW(L"static", L"SDR Project ", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"button", L"Open File", WS_VISIBLE | WS_CHILD, 10, 50, 100, 50, hWnd, (HMENU)OPEN_FILE, NULL, NULL);
	hEdit = CreateWindowW(L"Edit", L"Content...", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL, 10, 110, 400, 300, hWnd, NULL, NULL, NULL);
}