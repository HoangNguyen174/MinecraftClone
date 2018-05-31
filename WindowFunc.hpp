#ifndef WINDOW_FUNCTION
#define WINDOW_FUNCTION
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace std;
class WindowFunc
{
	public:
		static void WinTranslateMessage( const MSG* msg );
		static void WinDispatchMessage( const MSG* msg );
		static BOOL GetCursorPosition(POINT* pos);
		static BOOL SetCursorPosition(int x, int y);
		static void Initialzize(HINSTANCE applicationHandle);
		static HCURSOR WinLoadCusor(HINSTANCE ins, LPCWSTR ptr);
		static HCURSOR WinSetCuror(HCURSOR ins);
};
#endif