#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <tchar.h>

#ifdef _UNICODE
using _tstring = std::wstring;
#else
using _tstring = std::string;
#endif

// FlameCyclone
class Console
{
public:

    static void Printf(LPCTSTR pFormat, ...);
    static void Clear();
};
