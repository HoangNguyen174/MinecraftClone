#include "WindowFunc.hpp"

BOOL WindowFunc::GetCursorPosition(LPPOINT pos)
{
	return GetCursorPos( pos );
}

BOOL WindowFunc::SetCursorPosition(int x, int y)
{
	return SetCursorPos(x,y);
}

void WindowFunc::WinDispatchMessage(const MSG* msg)
{
	DispatchMessage(msg);
}

void WindowFunc::WinTranslateMessage(const MSG* msg)
{
	TranslateMessage(msg);
}

HCURSOR WindowFunc::WinLoadCusor(HINSTANCE ins, LPCWSTR ptr)
{
	return LoadCursor(ins, ptr);
}

HCURSOR WindowFunc::WinSetCuror(HCURSOR ins)
{
	return SetCursor( ins );
}