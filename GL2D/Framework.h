#pragma once
#include "GameObject.h"
#include <array>
#include <deque>
#include <ranges>
#include <algorithm>
#include <map>

typedef std::string(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Layers = static_cast<int>(Layer::END);

enum class DeleteRange
{ One, All };

enum class SearchRange
{ One, All };

enum class ModeType
{ Default, Floating };

class Framework {
private:
	std::array<std::deque<GameObject*>, Layers> ObjectDeque;
	std::map<std::string, GameObject*> ObjectList;

	std::string						  CurrentRunningMode{};
	std::string                       PrevRunningMode{};

	bool							  RoutineRunningActivated{};
	bool							  FloatingRunningActivated{};
	bool                              FloatingFocusActivated{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};

public:
	Framework();
	std::string Mode();
	void Init(Function ModeFunction);
	void SetController(ControllerFunction Controller, ModeType Type);
	void InputFrameTime(float ElapsedTime);
	void Routine();
	void SwitchMode(Function ModeFunction);
	void StartFloatingMode(Function ModeFunction, bool FloatingFocus=false);
	void EndFloatingMode();
	void ResetControlState(GameObject* Object);
	void ResetControlState(std::string Tag);
	void InputKey(std::string Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey);
	void InputMouse(std::string Tag, int button, int state, int x, int y);
	void InputScroll(std::string Tag, int button, int Wheel, int x, int y);
	void AddObject(GameObject* Object, std::string Tag, Layer AddLayer, bool SetStaticObject=false, bool SetFloatingObject=false);
	void SwapLayer(GameObject* Object, Layer TargetLayer);
	void DeleteSelf(GameObject* Object);
	void DeleteObject(std::string Tag, DeleteRange deleteRange);
	GameObject* Find(std::string Tag);
	GameObject* Find(std::string Tag, Layer LayerToSearch, int Index);
	size_t Size(Layer TargetLayer);
	void Exit();

private:
	void UpdateContainer(int i);
	void ClearFloatingObject();
	void ClearAll();
};
extern Framework framework;