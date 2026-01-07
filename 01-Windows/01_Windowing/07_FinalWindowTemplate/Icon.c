// Win32 headers
#include<windows.h> // Includes the core Win32 API declarations for functions, types, and constants like HWND, MessageBox, CreateWindow, etc.
#include"Icon.h" 
// User-defined header file.
// Likely contains a custom icon resource ID or image data (used in WNDCLASSEX structure to set window icons).
#include<stdio.h>
#include<stdlib.h>
// Standard C libraries.
// stdio.h is used for file I/O (like writing to a log).
// stdlib.h for general utilities like memory allocation or exit handling.

// Micros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
// Defines your default window size (used in CreateWindow()).
// You can change these later when switching to fullscreen or restoring windowed mode.

// Global Function decalrations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// Forward declaration of your main message callback function.
// Handles all Windows messages: keystrokes, paint requests, resizing, etc.

// Gloabl variable declartions
// Variables related with fullscreen
BOOL gbFullScreen = FALSE;	// Global flag to track whether you're currently in fullscreen.
HWND ghwnd = NULL; 			// Stores the window handle — required for any Win32 window operations (ShowWindow, SetWindowLong, etc.).
DWORD dwStyle; 				// Used to temporarily store the current window style (GWL_STYLE) when switching between modes.
WINDOWPLACEMENT wpPrev; 	// Structure to save the window's size, position, and state before going fullscreen. Lets you restore it exactly as it was after coming out of fullscreen.

// Variables related to File I/O
char gszLogFileName[] = "Log.txt";
FILE *gpFile = NULL;
// Used to log events (initialization success/failure, errors, debugging).
// gpFile will be opened in main() or WinMain() as a global file pointer.

// Active window related variable
BOOL gbActiveWindow = FALSE;
// Tells you whether your app is the foreground (focused) window.
// Important for pausing rendering/game updates when your window loses focus (e.g., ALT+TAB).

// Exit key pressed related
BOOL gbEscKeyIsPressed = FALSE; // A simple flag to track if the user pressed ESC (used to exit app or pause menu).

// Entry point function
// hInstance: Handle to this application instance.
// hPrevInstance: Always NULL in Win32 (legacy, ignore it).
// lpszCmdLine: Command line args (as single string).
// iCmdShow: How to show the window (minimized, normal, maximized).

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) // WinMain is the starting point for GUI apps.
{

	// Local Function Decalartions
	int initialize(int);
	void display(void);
	void update(void);
	void uninitialized(void);

	// Variable declarations
	WNDCLASSEX wndclass; 	// WNDCLASS is a struct
	HWND hwnd;
	MSG msg;				// MSG is a struct
	TCHAR szAppName[] = TEXT("RTR6_WIN");
	BOOL bDone = FALSE;

	// Code
	// Create Log File // fopen_s // fprint_s s for secured
	gpFile = fopen(gszLogFileName, "w");  // r :- read, w :- write, a :- append r+ a+ 
	if(gpFile == NULL)
	{
		MessageBox(NULL, TEXT("LogFile creation Failed"), TEXT("File I/O ERROR"), MB_OK);// NULL
		exit(0);
	}
	else
	{
		fprintf(gpFile, "program started sucessfully\n");
	}
	// Attempts to create Log.txt in write mode.
	// If it fails:
	// A MessageBox appears with error.
	// exit(0) cleanly ends the program.
	// If it succeeds:
	// Logs that the program started.

	// Window Class Initilization 
	wndclass.cbSize = sizeof(WNDCLASSEX); 
																	// Specifies the size of the WNDCLASSEX structure. 
																	// Always use sizeof(WNDCLASSEX) to avoid mismatch issues with different compilers or platform targets.
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC ; 			// Defines the window's class styles
																	// CS_HREDRAW: Redraw the window if width changes.
																	// CS_VREDRAW: Redraw the window if height changes.
																	// CS_OWNDC: Allocates a unique Device Context (DC) for the window.
	wndclass.cbClsExtra = 0; 										// cbClsExtra: Class-level data
	wndclass.cbWndExtra = 0;			 							// cbWndExtra: Window instance-level data.
																	// Set to 0 since don’t need extra memory storage.
	wndclass.lpfnWndProc = WndProc; 								// Sets the address of your Window Procedure function.
																	// This is the heart of any Windows app: it handles all events (keyboard, mouse, resize, etc).
	wndclass.hInstance = hInstance; 								//Specifies the handle to your app instance (passed in WinMain).
																	// Used when loading resources like icons, cursors, or menu templates.
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 	// Background brush used to paint the window before WM_PAINT
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); 	// MYICON is a custom icon resource ID defined in Icon.h
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); 				// Sets the default mouse cursor to an arrow (IDC_ARROW). Can be replaced later with SetCursor() if you want custom cursors.
	wndclass.lpszClassName = szAppName; 							// Names this window class. Used later in RegisterClassEx and CreateWindow.
	wndclass.lpszMenuName = NULL; 									// No menu is attached to this window.
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));// Loads the small icon shown in the window’s title bar (top-left).

	// Registration Of Window Class
	RegisterClassEx(&wndclass);
	// What It Does:
	// Registers your custom WNDCLASSEX with the OS so you can later create a window with that class.
	// If registration fails, the system won't let you create a window using CreateWindowEx().
	// It connects your class name (like "RTR6_WIN") with all the style and behavior you defined earlier.

	// Create Window
	hwnd =	CreateWindowEx(WS_EX_APPWINDOW, 					// Extended style
			szAppName,											// Class name
		 	TEXT("Omkar Ankush Kashid"),						// Window title (shown in title bar)
	  		WS_OVERLAPPEDWINDOW 								// Basic window with title, border, resize
			| WS_CLIPCHILDREN 									// Optimize for OpenGL: Prevent drawing child windows
			| WS_CLIPSIBLINGS 									// Prevent overlapping sibling windows
			| WS_VISIBLE,										// Show the window immediately
        	(GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2, 	// Center X
        	(GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2, 	// Center Y
		 	WIN_WIDTH,											// Window width
		  	WIN_HEIGHT,											// Window height
		   	NULL,												// No parent window (top-level)
		    NULL,												// No menu
			hInstance,											// Current app instance
			NULL												// No extra creation data
		);
		
	ghwnd = hwnd;
	// What It Does:
	// Saves the handle of the created window into your global ghwnd variable.
	// This is later used in:
	// ShowWindow(ghwnd, ...)
	// SetWindowLong(ghwnd, ...)
	// SetWindowPos(ghwnd, ...)
	// Any function that needs to refer to your window.

	// Show Window
	ShowWindow(hwnd, iCmdShow);
	// What It Does:
	// This makes the window visible on the screen.
	// iCmdShow comes from WinMain() — it tells Windows how to show the window (like SW_SHOWNORMAL, SW_MAXIMIZE, etc.).

	// Paint Background Of The Window
	UpdateWindow(hwnd);
	// What It Does:
	// Sends a WM_PAINT message immediately, instead of waiting for the system to do it.
	// Usually used to trigger your WndProc() to paint right away — helpful in OpenGL apps for ensuring first frame shows up properly.

	// Initilized
	int result = initialized();
	// What It Does:
	// Calls your own function initialized() where you'll likely initialize:
	// OpenGL rendering context
	// Viewport settings
	// Any game or 3D engine setup
	// Load resources (textures, shaders, etc.)
	if(result != 0)
	{
		fprintf(gpFile, "initilized() failed\n");
		DestroyWindow(hwnd);
		hwnd = NULL;
	}
	else
	{
		fprintf(gpFile, "initilized() Complited Sucessfully");
	}
	// Checks if initialized() failed (non-zero = failure).
	// If so, logs the error, destroys the window, and sets hwnd to NULL to avoid further use.
	// Else, logs a success message.

	// Set this window as foreground and active window
	SetForegroundWindow(hwnd);
	// Brings the window to the front of all others.
	// Ensures it's the main focus for user input and visibility.	

	SetFocus(hwnd);
	// Explicitly sets keyboard input focus to this window.
	// Important in gaming or OpenGL apps where you want immediate control.


	// GameLoop
	// Why PeekMessage() Instead of GetMessage()?
	// PeekMessage() is non-blocking (it checks for messages but does not wait).
	// GetMessage() is blocking (it waits until a message arrives before proceeding).
	// Using PeekMessage() allows continuous rendering and updating of the game while still handling system messages.
	// This is the main game loop that runs continuously until bDone is set to TRUE.
	// bDone is typically a flag that indicates whether the application should exit.
	while(bDone == FALSE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// PeekMessage() checks if there is any message in the Windows message queue.
			// If there is a message, it retrieves it and removes it from the queue.
			// The function parameters:
			// &msg → Pointer to a MSG structure that stores the message.
			// NULL → Monitors messages for all windows created by this thread.
			// 0, 0 → Retrieves messages in the full range.
			// PM_REMOVE → Removes the message from the queue after retrieving it.

			if(msg.message == WM_QUIT)
			{
				//If the message is WM_QUIT, the program should exit, so bDone is set to TRUE.
				// WM_QUIT is usually posted when the user closes the application.
				bDone = TRUE;
			}
			else
			{
				//TranslateMessage(&msg) → Converts virtual key messages into character messages (for keyboard input).
				// DispatchMessage(&msg) → Sends the message to the appropriate window procedure (WndProc), where it is processed.
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// If there are no messages to process, the program executes the game logic:
			if(gbActiveWindow == TRUE)
			{
				//gbActiveWindow is likely a global flag that indicates if the application window is currently active (i.e., in focus).
				// If the window is minimized or loses focus, the game pauses rendering to reduce CPU/GPU usage.

				if(gbEscKeyIsPressed == TRUE)
				{
					//gbEscKeyIsPressed is likely a global flag that gets set when the Escape key (VK_ESCAPE) is pressed.
					// This allows the user to exit the game by pressing the Escape key.
					bDone = TRUE;
				}

				// Render -> Render graphics.
				display();

				// Update -> Update game logic.
				update();	
			}
		}
	}
	// Uninitilized
	uninitialized();
	// Clean up everything your app initialized, such as:
	// Release the OpenGL rendering context
	// Restore fullscreen settings (if needed)
	// Close the log file
	// Free any allocated memory or handles

	return((int)msg.wParam);
	// Ends the WinMain() function.
	// Returns the exit code of the application to the OS.
	// msg.wParam is set by PostQuitMessage() — you usually set it to 0 (success) or any error code if needed.
}

// Call Back Function
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void togglefullscreen(void);
	void resize(int, int);
	void uninitialized(void);

	// Code
	switch (iMsg)
	{
		
		case WM_CREATE:
			ZeroMemory((void *)&wpPrev, sizeof(WINDOWPLACEMENT));
			wpPrev.length = sizeof(WINDOWPLACEMENT);
			break;

		case WM_SETFOCUS:
			gbActiveWindow = TRUE;
			break;

		case WM_KILLFOCUS:
			gbActiveWindow = FALSE;
			break;

		case WM_SIZE:
			resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_KEYDOWN:
			switch(wParam)
			{
				case VK_ESCAPE:
					gbEscKeyIsPressed = TRUE;
					break;
				default:
					break;
			}
			break;
		

		case WM_CHAR:
			switch(wParam)
			{
				case 'F':
				case 'f':
					if(gbFullScreen == FALSE)
					{
						togglefullscreen();
						gbFullScreen = TRUE;
					}
					else
					{
						togglefullscreen();
						gbFullScreen = FALSE;
					}
					break;
				default:
					break;
			
			}
			break;

		case WM_DESTROY :
			PostQuitMessage(0);
			break;

		
		case WM_CLOSE:
			uninitialized();
			break;
			
		default:
			break;
			
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void togglefullscreen(void)
{
	// variable declarations
	MONITORINFO mi;
	

	// Code
	if(gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if(dwStyle & WS_OVERLAPPEDWINDOW)
		{
			ZeroMemory((void *)&mi, sizeof(MONITORINFO));
			mi.cbSize = sizeof(MONITORINFO);
			if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right-mi.rcMonitor.left, mi.rcMonitor.bottom-mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		
		BOOL ShowCursor = FALSE;
	}
	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		BOOL ShowCursor = TRUE;
	}

}

int initialized(void)
{
	// code

	return(0);
}

void resize(int width, int height)
{
	// code
}

void display(void)
{
	// code
}

void update(void)
{
	// code
}

void uninitialized(void)
{
	// code

	// Close the file
	if(gpFile)
	{
		fprintf(gpFile, "Program terminated sucessfully");
		fclose(gpFile);
		gpFile = NULL;
	}
}



