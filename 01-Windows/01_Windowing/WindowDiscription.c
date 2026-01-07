// Win32 headers
#include<windows.h>

// Librarys :- User32.dll, GDI32.dll(Graphics device interface), Kernel32.dll
// derived datatypes
// #define strict reason for complosry used micros 

// Global Function decalrations // prototype // signature
LRESULT // long // RETURN VALUE INTERNALLY LONG 
CALLBACK // __far _pascal (calling convention)  // CALLING CONVENTION HOW TO TAKE PRAMETERS FROM RIGHT TO LEFT OR LEFT TO WRITE LIKE SEQUENCE (HOW TO ARNGE IN CPU OR ACCESS IT IS DECLARE HERE (BASE POINTER)) AFTER COMPLETING FUNCTION WHO WILL CLEARE THIS FUNCTIONS STACK(INTERNALLY CALLBACK :- _far(far segment because this function is going to call by OS) _pascal(standered calling convetion))  
WndProc // function name // windo procedure // HANGARIAN NOTATION DECLARAED BY DEVELOPER
// parameters
(HWND, // handle // unsigned int // handle to window // window cha handle aahe
UINT, // usigne int // message cha number aahe 
WPARAM,// unsigned int // pass word type parameter
LPARAM); // long // pass long type parameter


// CODING CONVEMTION WHO MADE THIS ONE :- CHARLES SIMONYI (HANGARIAN NOTATION)
// #PRAGMA argused // for supperasing warning for unused variables tailing complier to precompiled direction like here we did't used hPrevInstance, lpszCmdLine
// only consider those variable whihch is used dom;t consider thos are not used that's why #pragma argused is used 


// RULES:- 
// 1. MININGFULL NAME 
// e.g :- 
// float roi; (wrong one)
// float RateOfInteraste; (right one)
// 2. IF THE MANE OF VARIABLE IS MADE UP OF MULTIPLE WORD THEN FIRST LETTER WILL BE CAPITAL FOR e.g :- RateOfInteres (not like rateofinteraste)
// 3.(SUBRULE) :- VARIABLE NAME WHIHCH DATATYPE IT HAS TO START WITH SAMLL CASE LETTER OF NAME 1ST LETTER OF VARIABLE 
// e.g:- 
// int iNum; (i IS HERE IS FOR INTGER)
// float fRateOfInteraste (f IS FOR FLOAT)
// hInstance :- HANDLE TYPE
// hPrevInstance :- PREVIEOUS HANDLE TYPE 
// lpszCmdLine:- COMMAND LINE ARGUEMNT CUI :- [*argv] 
// lpsz :- string end // long pointer to string which is zero terminated zero  
// WPARAM wParam :- UINT :- UNSIGNED INT // PARAM :- VARIABLE NAME (WORD MEANS 16 BYTES THATS WHY W )
// LPARAM :- LONG PARAMETER // lParam :- long parameter 



// Entry point function
int WINAPI // calling conventions // INTERNALLY _far _pascal // return type is int because of OS calling convetions 
WinMain // internally __winmainCRTstratup // __ mainCRTstartup // main // CRT:- "C" RUN TIME // GUI ENTRY POINT FUNCTION // for the feel it is write like WinMain in GUI programing 
// WinMain is called by os or runtime or loader
(HINSTANCE hInstance, // while click on mouse or keybord input will be send through this 1st parameter through OS // Instances handle 
HINSTANCE hPrevInstance, // currently in not used internally is NULL is 0 // backwrord comtabillity that's why it's is here // beacause today used preamptive multitasking // before it is used for time sharing
LPSTR lpszCmdLine, // for taking cammond line argument // LPSTR internally long pointer to string (int _far *) like double pointer char **argv (*argv[]) // NULL terminated string '\0'
int iCmdShow) // your window where and how to see it tell by OS // i is for integer 
{
	// Variable declarations
	WNDCLASSEX wndclass; // WNDCLASS is a struct // window structre // EX is for extended class of WNDCLASS
	HWND hwnd;// HWND internally is HANDLE is interally UINT is internally unsigned int
	MSG msg;// MSG is a struct 6 members nested 6 members
	TCHAR szAppName[] = TEXT("RTR6_WIN"); // custom string // TCHAR here it is used for UNICODE // TEXT here it is micro

	// Code	
	
	// Window Class Initilization 12 members
	wndclass.cbSize = sizeof(WNDCLASSEX); // Newlly added // cbSize:- count of bytes(byte count of structure) // sizeof give the size of WNDCLASSEX to it
	wndclass.style = CS_HREDRAW | CS_VREDRAW; // style here it is 's' is small = CS_HREDRAW (class style horizontally redraw) | CS_VREDRAW(class style vertically redraw)
	wndclass.cbClsExtra = 0; // cbClsExtra :- extra information byte count which will you want to give to class 
	wndclass.cbWndExtra = 0; // cbWndExtra :- extra information byte count which will you want to give to window which are you going to create 
	wndclass.lpfnWndProc = WndProc; // long pointer function // callback function address are given here which we declared above(name of the function is it's address) // os will call this function
	wndclass.hInstance = hInstance; // 1st parameter of WinMain // current instance handle //
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // handle brush background // OS stock color here it is white// brush(hbrush)(WHITE_BRUSH(micro int) here BRUSH will tell devloper want brush) fount(hfount) pen(hpen) // return value HGDIOBJ (handle to graphich device interface object) // typecast by HBRUSH for brush // GetStockObject This API is in the GDI32.dll and all other are in the User32.dll // WIN32 API
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // left top icon // which icon show that's HANDLE // 1st parametere NULL means i don't have icon so give me one of your to OS so we give (IDI_APPLICATION which is micro string type) icon of identifier of application //  
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);// which type of cursors handle // give arrow type cursors from your stock IDC 'c' here is cursor above IDI 'I' is for id above and here null is telling idont have cursor and icon that's why we are giving NULL 
	wndclass.lpszClassName = szAppName; // this one my class name which one will be different here i give RTR6_WIN
	wndclass.lpszMenuName = NULL; // Name of menu (not a system menu) here we don't have menu that's why we give NULL
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // Newlly added // sm :- small icon // taskbar icon(hIcon) // explorer icon(this one is here) // stock ICON here used 

	// Registration Of Window Class
	RegisterClassEx(&wndclass); // EX is for extended // here we are giving the wndclass's adress // return value name ATOM unique string imutable(we cannot change) string like ATOM
	
	// Create Window
	hwnd =	CreateWindow(szAppName, // string(LPCSTR) // custom window name
		 	TEXT("Omkar Ankush Kashid"), // LPCSTR // Window tital UNICODE string that why we are using TEXT micro // caption bar name or tital
	  		WS_OVERLAPPEDWINDOW, // DWORD double word 32bit internally unsigned long //WS:- Windows style // internally 6 styles 
	   		CW_USEDEFAULT, // x quardinate
	    	CW_USEDEFAULT, // y quardinate // CW:- create window
		 	CW_USEDEFAULT, // width 
		  	CW_USEDEFAULT, // height
		   	NULL, 
		    NULL,// in some book used :- HWND_DESKTOP // winodw dosen't have handle 
			hInstance, // 1st parameter of WinMain  
            // HANDLE :- how to used tool is HANDLE 
            // HWND means is windows handle 
            // HBRUSH :- brush's handle 
            // HANDLE is internalyy 32 bit unsigned int whihch behave like pointer but you can't used it as a pointer 
            // it's is like you can see but can't used like opec (transperent) pointer 
            // What is Instance :- I created 3 process but i run only one program (one program has 3 instances)
            // hPrevInstance chek that where that program's Instance is olready avilabe or not
			NULL); // window creation window 

	// Show Window
	ShowWindow // return BOOL:-      1.TRUE:- 1      2.FALSE:- 0 
	(hwnd, 
	iCmdShow); // SW_MAXIMIZE // SW_MINIMIZE // SE_HIDE // SW_SHOWNORMAL // SW_NORMAL // SW:- SHOW WINDOW 
	// iCmdShow 4th parameter of WinMain how to show window default parameter which is told by Operating system in 4th parameter of WinMain and it is used here

	// Paint Background Of The Window
	UpdateWindow(hwnd); // IN CLIENT AREA BACKGROUND WNDCLASS BEUSH GIVE COLOR // RETURN BOOL 

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
	case WM_DESTROY : // WM:- WINDOW MESSAGE   
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


