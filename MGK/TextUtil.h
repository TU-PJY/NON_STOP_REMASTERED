#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "EngineHeader.h"
#include <windows.h>
#include <map>

enum Align { 
	ALIGN_DEFAULT, 
	ALIGN_MIDDLE, 
	ALIGN_LEFT 
};


class TextUtil {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };

	GLfloat Rotation{};
	GLfloat Transparency{ 1.0f };
	GLfloat CurrentHeight{};
	GLfloat NewLineSpace{};

	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	std::map <wchar_t, bool> GlyphCache;

	HDC hDC{};
	HFONT Font{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[65536]{};

	bool AutoNewLine{};
	int WordNumForNewLine{};
	int CurrentWordNum{};

	int TextAlign{ ALIGN_DEFAULT };
	int RenderType{ RENDER_TYPE_STATIC };


public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	void SetAlign(int AlignOpt);
	void Rotate(GLfloat Radians);
	void SetLineSpace(GLfloat Value);
	void LineNumber(int LineNum);
	void ResetLineSpace();
	void ResetLine();
	void SetRenderType(int Type);
	void EnableAutoLine();
	void DisableAutoLine();
	void SetAutoLineWordSpace(int Value);
	void Render(GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...);

private:
	void GetLength(GLfloat& Length, unsigned Index, const wchar_t* Text, GLfloat Size);
	void InitMatrix();
	void PrepareRender();
	bool CheckGlyphCache(wchar_t Char);
	void LoadGlyph(wchar_t Char);
};
extern TextUtil textUtil;