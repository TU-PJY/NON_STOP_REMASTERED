#pragma once
#include "EngineHeader.h"
#include <string>
#include <map>

enum ImageType { 
	IMAGE_TYPE_LINEAR, 
	IMAGE_TYPE_NEAREST 
};


class ImageUtil {
private:
	GLuint VAO{};

public:
	void Init();
	void Import(Image& ImageStruct, const char* FileName, int Type=IMAGE_TYPE_LINEAR);
	void Render(Image& ImageStruct);
	void Release(Image& ImageStruct);
};
extern ImageUtil imageUtil;