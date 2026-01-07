// Win32 headers
#include<windows.h>
#include"Icon.h"

// Global Function decalrations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Variable declarations
	WNDCLASSEX wndclass; // WNDCLASS is a struct
	HWND hwnd;
	MSG msg;// MSG is a struct
	TCHAR szAppName[] = TEXT("RTR6_WIN");

	// Code
	
	// Window Class Initilization 
	wndclass.cbSize = sizeof(WNDCLASSEX); 
	wndclass.style = CS_HREDRAW | CS_VREDRAW; 
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// Registration Of Window Class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd =	CreateWindow(szAppName,
		 	TEXT("Omkar Ankush Kashid"),
	  		WS_OVERLAPPEDWINDOW,
	   		CW_USEDEFAULT,
	    	CW_USEDEFAULT,
		 	CW_USEDEFAULT,
		  	CW_USEDEFAULT,
		   	NULL,
		    NULL,
			hInstance,
			NULL);

	// Show Window
	ShowWindow(hwnd, iCmdShow);

	// Paint Background Of The Window
	UpdateWindow(hwnd);

	// Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam,
LPARAM lParam)
{
	// Code
	switch (iMsg)
	{
	case WM_DESTROY :
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

//glutinit
//wndclass registerclass
//
//glutinit position 
//create window 4th 5th 
//
//glut window size 6th 7th 
//
//glut mainloop
//whileloop
//glut mainloop
//wn_distroy

