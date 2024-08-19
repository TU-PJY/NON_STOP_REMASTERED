#pragma once
#include "EngineHeader.h"
#include <string>
#include <map>

class ImageUtil {
private:
	GLuint VAO{};
	std::map<std::string, unsigned int> LoadedImageList;

public:
	void Init();
	void LoadImageFromFile(unsigned int& Image, const char* FileName);
	void SetImage(unsigned int& Image, std::string ImageName);
	void Render(unsigned int ImageVar);

private:
	void LoadSystemImage();
	void LoadImageFromList();
};
extern ImageUtil imageUtil;