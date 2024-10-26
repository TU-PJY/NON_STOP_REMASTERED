#pragma once
#include <windows.h>
#include <string>

class FontUtil{
public:
	bool Import(const std::string& fontFilePath, DWORD& numFonts);

private:
	std::wstring ToWstr(const std::string& str);
};
extern FontUtil fontloaderUtil;
