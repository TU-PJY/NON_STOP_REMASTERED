#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "tinyxml/tinyxml.h"
#include <iostream>
#include <string>

// read only
class ScriptUtil {
private:
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	bool FileExist{};

public:
	void Import(const char* FileName);
	float LoadData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);
	void Release();

private:
	TiXmlElement* FindCategory(const char* CategoryName);
	const char* FindData(const char* CategoryName, const char* DataName);
	float GetDigitData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);
};