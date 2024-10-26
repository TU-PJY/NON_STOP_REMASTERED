#include "Scene.h"

void Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

const char* Scene::Mode() {
	return CurrentRunningMode;
}

void Scene::Stop() {
	RoutineActivateCommand = false;
}

void Scene::Resume() {
	RoutineActivateCommand = true;
}

void Scene::Routine() {
	for (int i = 0; i < Layers; ++i) {
		for (auto const& O : ObjectList[i]) {
			if (!O->DeleteCommand) {
				if (RoutineActivateCommand) {
					if(FloatingFocusCommand && O->FloatingCommand)
						O->UpdateFunc(FrameTime);
					else
						O->UpdateFunc(FrameTime);
				}

				O->RenderFunc();
			}
		}

		ProcessListCommand(i);
	}

	ProcessIndexCommand();
}

void Scene::Init(Function ModeFunction) {
	for(int i = 0; i < Layers; ++i)
		ObjectList[i].reserve(OBJECT_LIST_RESERVE);
	ModeFunction();
}

void Scene::SwitchMode(Function ModeFunction) {
	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	ModeFunction();

	if (FloatingActivateCommand) {
		FloatingActivateCommand = false;
		FloatingFocusCommand = false;
	}
}

void Scene::RegisterDestructor(Function DestructorFunction) {
	DestructorBuffer = DestructorFunction;
}

void Scene::RegisterModeName(const char* ModeName) {
	CurrentRunningMode = ModeName;
}

void Scene::RegisterController(ControllerFunction Controller, int Type) {
	Controller();
	if (Type == MODE_TYPE_DEFAULT)
		ControllerBuffer = Controller;
}

void Scene::ReleaseDestructor() {
	DestructorBuffer = nullptr;
}

void Scene::StartFloatingMode(Function ModeFunction, bool FloatingFocus) {
	if (FloatingActivateCommand)
		return;

	PrevRunningMode = CurrentRunningMode;
	ModeFunction();
	FloatingFocusCommand = FloatingFocus;

	FloatingActivateCommand = true;
}

void Scene::EndFloatingMode() {
	if (!FloatingActivateCommand)  
		return;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingActivateCommand = false;
	FloatingFocusCommand = false;
}

void Scene::ResetControlState(GameObject* Object) {
	Object->ResetControlState();
}

void Scene::ResetControlState(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteCommand)
		It->second->ResetControlState();
}

void Scene::InputKey(const char* Tag, int State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteCommand)
		It->second->InputKey(State, NormalKey, SpecialKey);
}

void Scene::InputMouse(const char* Tag, int State) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteCommand)
		It->second->InputMouse(State);
}

void Scene::InputScroll(const char* Tag, int State) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteCommand)
		It->second->InputScroll(State);
}

void Scene::AddObject(GameObject* Object, const char* Tag, int AddLayer, int Type1, int Type2) {
	ObjectList[AddLayer].emplace_back(Object);
	ObjectIndex.insert(std::make_pair(Tag, Object));

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_STATIC) {
		Object->StaticCommand = true;
		return;
	}

	if (Type1 == Type2 && Type1 == OBJECT_TYPE_FLOATING) {
		Object->FloatingCommand = true;
		return;
	}

	if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC)
		Object->StaticCommand = true;

	if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
		Object->FloatingCommand = true;
}

void Scene::SwapLayer(GameObject* Object, int TargetLayer) {
	if (Object->ObjectLayer == TargetLayer)
		return;

	Object->SwapCommand = true;
	Object->ObjectLayer = TargetLayer;
}

void Scene::DeleteObject(GameObject* Object) {
	Object->DeleteCommand = true;
}

void Scene::DeleteObject(const char* Tag, int DeleteRange) {
	if (DeleteRange == DELETE_RANGE_SINGLE) {
		auto It = ObjectIndex.find(Tag);
		if (It != end(ObjectIndex) && !It->second->DeleteCommand)
			It->second->DeleteCommand = true;
	}

	else if (DeleteRange == DELETE_RANGE_EQUAL) {
		auto Range = ObjectIndex.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteCommand) 
					It->second->DeleteCommand = true;
			}
		}
	}
}

GameObject* Scene::Find(const char* Tag) {
	auto It = ObjectIndex.find(Tag);
	if (It != end(ObjectIndex) && !It->second->DeleteCommand)
		return It->second;

	return nullptr;
}

GameObject* Scene::FindMulti(const char* Tag, int SearchLayer, int Index) {
	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag == Tag)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

ObjectRange Scene::EqualRange(const char* Tag) {
	ObjectRange Range;
	auto It = ObjectIndex.equal_range(Tag);

	Range.First = It.first;
	Range.End = It.second;

	return Range;
}

size_t Scene::LayerSize(int TargetLayer) {
	return ObjectList[TargetLayer].size();
}

void Scene::Exit() {
	glutDestroyWindow(1);
}


//////// private ///////////////
void Scene::ProcessListCommand(int Index) {	
	for (auto It = begin(ObjectList[Index]); It != end(ObjectList[Index]);) {
		if ((*It)->DeleteCommand) {
			It = ObjectList[Index].erase(It);
			continue;
		}

		if ((*It)->SwapCommand) {
			auto Object = (*It);
			int DestLayer = (*It)->ObjectLayer;
			It = ObjectList[Index].erase(It);
			ObjectList[DestLayer].emplace_back(Object);
			Object->SwapCommand = false;
			continue;
		}
		++It;
	}
}

void Scene::ProcessIndexCommand() {
	for (auto It = begin(ObjectIndex); It != end(ObjectIndex);) {
		if (It->second->DeleteCommand) {
			delete It->second;
			It->second = nullptr;
			It = ObjectIndex.erase(It);
			continue;
		}
		++It;
	}
}

void Scene::ClearFloatingObject() {
	for (auto const& O : ObjectIndex) {
		if (O.second->FloatingCommand && !O.second->StaticCommand)
			O.second->DeleteCommand = true;
	}
}

void Scene::ClearAll() {
	for (auto const& O : ObjectIndex) {
		if (!O.second->StaticCommand)
			O.second->DeleteCommand = true;
	}
}
