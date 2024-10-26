#pragma once
#include "CollisionUtil.h"
#include "PhysicsUtil.h"
#include "RandomUtil.h"
#include "SoundUtil.h"
#include "imageUtil.h"
#include "TextUtil.h"
#include "TimerUtil.h"
#include "TransformUtil.h"
#include "AnimationUtil.h"
#include "ClippingUtil.h"
#include "MathUtil.h"
#include "UiUtil.h"
#include "ExUtil.h"
#include "LineBrush.h"
#include "RectBrush.h"
#include "SystemResources.h"
#include "GameResources.h"


enum FlipDir { 
	FLIP_H, 
	FLIP_V, 
	FLIP_HV 
};


class GameObject {
public:
	glm::mat4 TranslateMatrix{ 1.0f }, RotateMatrix{ 1.0f }, ScaleMatrix{ 1.0f }, ResultMatrix{ 1.0f };
	glm::mat4 ImageAspectMatrix{ 1.0f }, FlipMatrix{ 1.0f };

	glm::vec3 ObjectColor{ glm::vec3(0.0, 0.0, 0.0) };

	GLfloat TransparencyValue{ 1.0f };
	GLfloat BlurValue{};

	const char* ObjectTag{};
	int ObjectLayer{};

	bool SwapCommand{};
	bool DeleteCommand{};
	bool FloatingCommand{};
	bool StaticCommand{};

	// init functions
	void InitMatrix(int RenderType=RENDER_TYPE_DEFAULT);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3 Color);
	void SetColorRGB(int R, int G, int B);

	// viewport functions
	void UpdateViewportPosition(GLfloat& ValueX, GLfloat& ValueY, bool ApplyAspect = true);
	void UpdateLocalPosition(GLfloat& ValueX, GLfloat& ValueY);
	void UpdateViewportPosition(glm::vec2& Position, bool ApplyAspect);
	void UpdateLocalPosition(glm::vec2& Position);
	GLfloat ASP(GLfloat Value);

	// image functions
	void Flip(int FlipOpt);
	void Blur(int Strength);
	void Render(Image& Image, GLfloat Transparency = 1.0, bool DisableAdjustAspect=false);

	// sound functions
	void PlaySound(Sound Sound, SoundChannel& ChannelVar, unsigned int StartTime);
	void PauseSound(SoundChannel& ChannelVar, bool Flag);
	void StopSound(SoundChannel& ChannelVar);
	void SetPlaySpeed(SoundChannel& ChannelVar, float PlaySpeed);
	void ResetPlaySpeed(SoundChannel& ChannelVar);
	void EnableFreqCutoff(SoundChannel& ChannelVar, float Frequency);
	void EnableBeatDetect(SoundChannel& ChannelVar);
	void DetectBeat(GLfloat& Value, float ThresHold, float SamplingRate);
	bool IsBeat(float ThresHold, float SamplingRate);
	void DisableFreqCutoff(SoundChannel& ChannelVar);
	void DisableBeatDetect(SoundChannel& ChannelVar);
	void SetSoundDistance(SoundChannel& ChannelVar, float MinDist, float MaxDist);
	void SetListnerPosition(float X, float Y);
	void SetListnerPosition(glm::vec2 Position);
	void SetSoundPosition(SoundChannel& ChannelVar, float X, float Y, float Diff);

	void SetSoundPosition(SoundChannel& ChannelVar, glm::vec2 Position, float Diff);

	// object defined functions
	virtual ~GameObject() {}
	virtual void UpdateFunc(float FT) {}
	virtual void RenderFunc() {}
	virtual void InputKey(int State, unsigned char NormalKey, int SpecialKey) {}
	virtual void InputMouse(int State) {}
	virtual void InputScroll(int State) {}
	virtual void ResetControlState() {}
	virtual AABB GetAABB() { return {}; }
	virtual OOBB GetOBB() { return{}; }
	virtual BoundingSphere GetRange() { return {}; }

private:
	void PrepareRender(); 
	glm::vec4 ViewportPosition();
	glm::vec4 LocalPosition();
};