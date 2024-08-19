#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "EngineHeader.h"
#include <windows.h>
#include <map>

enum class Align
{ Default, Middle, Left };

enum class TextRenderMode
{ Static, Default };


class TextUtil {
private:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	GLfloat Rotation{};
	GLfloat Transparency{ 1.0f };
	GLfloat CurrentHeight{};
	GLfloat NewLineSpace{};

	glm::vec3 TextColor{ glm::vec3(1.0, 1.0, 1.0) };
	Align TextAlign{ Align::Default };

	std::map <wchar_t, bool> GlyphCache;

	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{};

	HDC hDC{};
	HFONT Font{};
	GLuint FontBase{};
	GLYPHMETRICSFLOAT TextGlyph[65536]{};

public:
	~TextUtil();
	void Init(const wchar_t* FontName, int Type, int Italic=FALSE);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetAlign(Align AlignOpt);
	void Rotate(GLfloat Radians);
	void SetNewLineSpace(GLfloat Value);
	void NewLine();
	void ResetNewLineSpace();
	void ResetLine();
	void Render(TextRenderMode Mode, GLfloat X, GLfloat Y, GLfloat Size, GLfloat TransparencyValue, const wchar_t* Format, ...);

private:
	void GetLength(GLfloat& Length, unsigned Index, const wchar_t* Text, GLfloat Size);
	void InitTransform();
	void ProcessTransform();
	bool CheckGlyphCache(wchar_t Char);
	void LoadGlyph(wchar_t Char);
};
extern TextUtil textUtil;