#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Setting.h"
#include "tinyxml/tinyxml.h"
#include "cryptopp/aes.h" 
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"
#include "cryptopp/osrng.h"
#include "cryptopp/hex.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cctype>
using namespace CryptoPP;

enum D_Type { 
	DATA_TYPE_DIGIT,
	DATA_TYPE_STRING 
};

struct FileData {
	const char* CategoryName;
	const char* DataName;
	int DataType;
	float DigitValue;
	const char* StringValue;
};

using DataSet = std::vector<FileData>;


// write and read
class FileUtil {
private:
	std::string FilePathStr{};
	std::filesystem::path FilePath{};
	TiXmlDocument Doc{};
	TiXmlElement* Root{};
	
	DataSet DataListBuffer{};

	bool FileExist{};

public:
	void Init(const char* FolderName, const char* FileName, DataSet List);
	void UpdateData(const char* CategoryName, const char* DataName, float Value);
	void UpdateStringData(const char* CategoryName, const char* DataName, const char* Value);
	float LoadData(const char* CategoryName, const char* DataName);
	const char* LoadStringData(const char* CategoryName, const char* DataName);
	void ResetData();
	void Release();

private:
	void SetupData();
	void CheckDataVersion();
	void UpdateDataVersion(float VersionValue);
	void CreateDec(float VersionValue);
	void AddRoot(const char* RootName);
	void AddCategory(const char* CategoryName);
	void AddData(const char* CategoryName, const char* DataName, float Value);
	void AddStringData(const char* CategoryName, const char* DataName, const char* Value);
	void WriteDigitData(TiXmlElement* CategoryVar, const char* DataName, float Value);
	void WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value);
	int GetIntData(TiXmlElement* CategoryVar, const char* DataName);
	float GetDigitData(TiXmlElement* CategoryVar, const char* DataName);
	const char* GetStringData(TiXmlElement* CategoryVar, const char* DataName);
	TiXmlElement* FindRoot();
	TiXmlElement* FindCategory(const char* CategoryName);
	const char* FileUtil::FindData(const char* CategoryName, const char* DataName);
	bool LoadDataFile(const char* FileName);
	void UpdateDataFile();
	std::string Encrypt(const std::string& plainText, const byte key[], const byte iv[]);
	std::string Decrypt(const std::string& cipherText, const byte key[], const byte iv[]);
};