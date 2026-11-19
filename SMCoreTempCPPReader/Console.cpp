#include "stdafx.h"
#include "Console.h"

void Console::Printf(LPCTSTR pFormat, ...)
{
	size_t nCchCount = MAX_PATH;
	_tstring strResult(nCchCount, 0);
	va_list args;

	va_start(args, pFormat);

	do
	{
		// Formatted String
		int nSize = _vsntprintf_s(&strResult[0], nCchCount, _TRUNCATE, pFormat, args);
		if (-1 != nSize)
		{
			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFOEX ScreenBuffer = { 0 };
			ScreenBuffer.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

			::GetConsoleScreenBufferInfoEx(console, &ScreenBuffer);

			// If text is entered at the beginning of a line, the current line content is cleared first.
			if (0 == ScreenBuffer.dwCursorPosition.X)
			{
				DWORD dwNumberOfCharsWritten = 0;
				::FillConsoleOutputCharacter(
					console, 
					_T(' '), 
					ScreenBuffer.dwMaximumWindowSize.X, 
					{ 0, ScreenBuffer.dwCursorPosition.Y },
					&dwNumberOfCharsWritten
				);
			}

			::WriteConsole(console, strResult.c_str(), nSize, NULL, NULL);
			break;
		}

		if (nCchCount >= INT32_MAX)
		{
			break;
		}

		nCchCount *= 2;
		strResult.resize(nCchCount);

	} while (true);

	va_end(args);
}

void Console::Clear()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFOEX ScreenBuffer = { 0 };
	ScreenBuffer.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	::GetConsoleScreenBufferInfoEx(console, &ScreenBuffer);

	for (SHORT i = ScreenBuffer.dwCursorPosition.Y; i < ScreenBuffer.dwMaximumWindowSize.Y; i++)
	{
		DWORD dwNumberOfCharsWritten = 0;
		::FillConsoleOutputCharacter(
			console, 
			_T(' '), 
			ScreenBuffer.dwMaximumWindowSize.X, 
			{ 0, ScreenBuffer.dwCursorPosition.Y },
			&dwNumberOfCharsWritten
		);
	}

	::SetConsoleCursorPosition(::GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
}
