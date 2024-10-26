#pragma once
#include "GameObject.h"
#include <algorithm>
#include <array>
#include <map>
#include <vector>

typedef void(*Function)(void);
typedef void(*ControllerFunction)(void);
constexpr int Layers = END;

using LayerIter = std::map<const char*, GameObject*>::iterator;

typedef struct {
	LayerIter First, End;
}ObjectRange;

enum DeleteRange { 
	DELETE_RANGE_SINGLE, 
	DELETE_RANGE_EQUAL 
};

enum ModeType { 
	MODE_TYPE_NONE, 
	MODE_TYPE_DEFAULT,
	MODE_TYPE_FLOATING 
};

enum ObjectType { 
	OBJECT_TYPE_NONE, 
	OBJECT_TYPE_STATIC, 
	OBJECT_TYPE_FLOATING 
};


class Scene {
private:
	std::array<std::vector<GameObject*>, Layers> ObjectList;
	std::map<const char*, GameObject*> ObjectIndex;

	const char*						  CurrentRunningMode{};
	const char*						  PrevRunningMode{};

	bool							  FloatingActivateCommand{};
	bool                              FloatingFocusCommand{};

	float							  FrameTime{};

	ControllerFunction				  ControllerBuffer{};
	Function						  DestructorBuffer{};

	bool RoutineActivateCommand{ true };

public:
	const char* Mode();
	void Stop();
	void Resume();
	void RegisterModeName(const char* ModeName);
	void Init(Function ModeFunction);
	void RegisterController(ControllerFunction Controller, int Type);
	void InputFrameTime(float ElapsedTime);
	void Routine();
	void SwitchMode(Function ModeFunction);
	void RegisterDestructor(Function DestructorFunction);
	void ReleaseDestructor();
	void StartFloatingMode(Function ModeFunction, bool FloatingFocus=false);
	void EndFloatingMode();
	void ResetControlState(GameObject* Object);
	void ResetControlState(const char* Tag);
	void InputKey(const char* Tag, int State, unsigned char NormalKey, int SpecialKey);
	void InputMouse(const char* Tag, int State);
	void InputScroll(const char* Tag, int State);
	void AddObject(GameObject* Object, const char* Tag, int AddLayer, int Type1= OBJECT_TYPE_NONE, int Type2=OBJECT_TYPE_NONE);
	void SwapLayer(GameObject* Object, int TargetLayer);
	void DeleteObject(GameObject* Object);
	void DeleteObject(const char* Tag, int deleteRange);
	GameObject* Find(const char* Tag);
	GameObject* FindMulti(const char* Tag, int SearchLayer, int Index);
	ObjectRange EqualRange(const char* Tag);
	size_t LayerSize(int TargetLayer);
	void Exit();

private:
	void ProcessListCommand(int Index);
	void ProcessIndexCommand();
	void ClearFloatingObject();
	void ClearAll();
};
extern Scene scene;