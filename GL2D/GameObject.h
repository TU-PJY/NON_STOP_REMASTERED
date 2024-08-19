#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "SoundUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "AnimationUtil.h"

using Sound = FMOD::Sound*;
using Channel = FMOD::Channel*;
using Image = unsigned int;

enum class Flip
{Horizontal, Vertical};

enum class ImageRenderMode
{Static, Default};

class GameObject {
private:
	unsigned int ModelLocation{}, TransparencyLocation{}, ObjectColorLocation{}, TextAlphaLocation{};

public:
	std::string ObjectTag{};
	int PrevLayer{};
	int DestLayer{};
	bool SwapLayerMarked{};
	bool DeleteObjectMarked{};
	bool FloatingObjectMarked{};
	bool StaticObjectMarked{};

	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f };
	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };
	GLfloat TransparencyValue{ 1.0f };

	void SetPosition(GLfloat MoveX, GLfloat MoveY);
	void Rotate(GLfloat RotationValue);
	void RotateHorziontal(GLfloat RotationValue);
	void RotateVertical(GLfloat RotationValue);
	void Scale(GLfloat ScaleX, GLfloat ScaleY);
	void MoveStraight(GLfloat& Position, int MoveDirection, GLfloat Speed, float FT);
	void MoveStraight(GLfloat& Position, GLfloat Speed, float FT);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, int MoveDirection, GLfloat RotationValue, float FT, bool Plus90Option=false);
	void MoveForward(GLfloat& X, GLfloat& Y, GLfloat Speed, GLfloat RotationValue, float FT, bool Plus90Option=false);
	GLfloat ASP(GLfloat Value);
	void BeginProcess(ImageRenderMode Mode);
	void RotateAxis(GLfloat RotationValue, GLfloat AxisX, GLfloat AxisY);
	void LookAt(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void LookAt(GLfloat Rotation, GLfloat& RotationVar, GLfloat RotationSpeed, float FT);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColorRGB(int R, int G, int B);
	glm::vec4 ViewportPosition();

	void SetImage(Image& Image, std::string ImageName);
	void FlipImage(Flip FlipOption);
	void RenderImage(Image Image, GLfloat Transparency=1.0, GLfloat ImageWidth=0.0, GLfloat ImageHeight=0.0);

	void FirstColorClipping();
	void SecondColorClipping();
	void EndColorClipping();

	void FirstAlphaClipping();
	void SecondAlphaClipping();
	void EndAlphaClipping();

	void SetSound(Sound& Sound, std::string SoundName);
	void PlaySound(Sound Sound, Channel& Channel, unsigned int MS = 0);
	void PauseSound(Channel& Channel, bool Flag);
	void StopSound(Channel& Channel);
	void SetPlaySpeed(Channel& Channel, float PlaySpeed);
	void ResetPlaySpeed(Channel& Channel);
	void SetFreqCutOff(Channel& Channel, float Frequency);
	void SetBeatDetect(Channel& Channel);
	void DetectBeat(float Threshold, int SamplingRate);
	void UnsetFreqCutOff(Channel& Channel);
	void UnsetBeatDetect(Channel& Channel);
	void SetChannelDistance(Channel& Channel, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetSoundPosition(Channel& Channel, float X, float Y, float Diff);

	// object defined functions
	virtual ~GameObject() {}
	virtual void Update(float FT) {}
	virtual void Render() {}
	virtual void InputKey(KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {}
	virtual void InputMouse(int Button, int State, int X, int Y) {}
	virtual void InputScroll(int Button, int State, int X, int Y) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OBB GetOBB() { return{}; }
	virtual Range GetRange() { return {}; }

private:
	void ProcessTransform(); 
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat CalculateShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
};

// dummy object for avoiding iterator error
class __DUMMY__ : public GameObject {};