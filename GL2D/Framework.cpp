#include "Framework.h"

Framework::Framework() {
}

void Framework::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string Framework::Mode() {
	return CurrentRunningMode;
}

void Framework::Routine() {
	if (RoutineRunningActivated) {
		for (int i = 0; i < Layers; ++i) {
			for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
				if (!(*It)->DeleteObjectMarked) {
					if (FloatingRunningActivated && FloatingFocusActivated && (*It)->FloatingObjectMarked)
						(*It)->Update(FrameTime);
					else
						(*It)->Update(FrameTime);
					(*It)->Render();
				}
			}

			UpdateContainer(i);
		}
	}
}

void Framework::Init(Function ModeFunction) {
	if (RoutineRunningActivated)
		return;

	for (int i = 0; i < Layers; ++i) {
		ObjectDeque[i].push_back(new __DUMMY__);
		ObjectDeque[i].back()->StaticObjectMarked = true;
	}

	CurrentRunningMode = ModeFunction();
	RoutineRunningActivated = true;
}

void Framework::SwitchMode(Function ModeFunction) {
	if (!RoutineRunningActivated)
		return;

	ClearAll();

	CurrentRunningMode = ModeFunction();

	if (FloatingRunningActivated) {
		FloatingRunningActivated = false;
		FloatingFocusActivated = false;
	}
}

void Framework::StartFloatingMode(Function ModeFunction, bool FloatingFocus) {
	if (!RoutineRunningActivated || FloatingRunningActivated)
		return;

	PrevRunningMode = CurrentRunningMode;
	CurrentRunningMode = ModeFunction();
	FloatingFocusActivated = FloatingFocus;

	FloatingRunningActivated = true;
}

void Framework::EndFloatingMode() {
	if (!RoutineRunningActivated || !FloatingRunningActivated)  
		return;

	ClearFloatingObject();
	CurrentRunningMode = PrevRunningMode;

	if (ControllerBuffer)  
		ControllerBuffer();

	FloatingRunningActivated = false;
	FloatingFocusActivated = false;
}

void Framework::ResetControlState(GameObject* Object) {
	Object->ResetControlState();
}

void Framework::ResetControlState(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->ResetControlState();
}

void Framework::InputKey(std::string Tag, KeyType Key, KeyState State, unsigned char NormalKey, int SpecialKey) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputKey(Key, State, NormalKey, SpecialKey);
}

void Framework::InputMouse(std::string Tag, int button, int state, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputMouse(button, state, x, y);
}

void Framework::InputScroll(std::string Tag, int button, int Wheel, int x, int y) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		It->second->InputScroll(button, Wheel, x, y);
}

void Framework::AddObject(GameObject* Object, std::string Tag, Layer AddLayer, bool SetStaticObject, bool SetFloatingObject) {
	int DestLayer = static_cast<int>(AddLayer);

	ObjectDeque[DestLayer].push_back(Object);
	Object->ObjectTag = Tag;
	Object->PrevLayer = DestLayer;

	if (Tag != "MATA_ENGINE_CONTAINER_DUMMY") {
		ObjectList.insert(std::make_pair(Tag, Object));

		if (SetFloatingObject) 
			Object->FloatingObjectMarked = true;
		
		if (SetStaticObject) 
			Object->StaticObjectMarked = true;
	}
}

void Framework::SwapLayer(GameObject* Object, Layer TargetLayer) {
	int DestLayer = static_cast<int>(TargetLayer);

	if (Object->PrevLayer == DestLayer)
		return;

	Object->SwapLayerMarked = true;
	Object->DestLayer = DestLayer;
	Object->PrevLayer = DestLayer;
}

void Framework::DeleteSelf(GameObject* Object) {
	Object->DeleteObjectMarked = true;
}

void Framework::DeleteObject(std::string Tag, DeleteRange deleteRange) {
	if (deleteRange == DeleteRange::One) {
		auto It = ObjectList.find(Tag);
		if (It != end(ObjectList) && !It->second->DeleteObjectMarked) 
			It->second->DeleteObjectMarked = true;
	}

	else if (deleteRange == DeleteRange::All) {
		auto Range = ObjectList.equal_range(Tag);
		if (Range.first != Range.second) {
			for (auto It = Range.first; It != Range.second; ++It) {
				if (It->first == Tag && !It->second->DeleteObjectMarked) 
					It->second->DeleteObjectMarked = true;
			}
		}
	}
}

GameObject* Framework::Find(std::string Tag) {
	auto It = ObjectList.find(Tag);
	if (It != end(ObjectList) && !It->second->DeleteObjectMarked)
		return It->second;

	return nullptr;
}

GameObject* Framework::Find(std::string Tag, Layer LayerToSearch, int Index) {
	int layer = static_cast<int>(LayerToSearch);

	if (Index >= ObjectDeque[layer].size())
		return nullptr;

	if (ObjectDeque[layer][Index]->ObjectTag == Tag && !ObjectDeque[layer][Index]->DeleteObjectMarked)
		return ObjectDeque[layer][Index];

	return nullptr;
}

size_t Framework::Size(Layer TargetLayer) {
	return ObjectDeque[static_cast<int>(TargetLayer)].size();
}

void Framework::SetController(ControllerFunction Controller, ModeType Type) {
	Controller();
	if (Type == ModeType::Default)
		ControllerBuffer = Controller;
}

void Framework::Exit() {
	glutDestroyWindow(1);
}

//////// private ///////////////
void Framework::UpdateContainer(int i) {
	auto It = std::erase_if(ObjectList, [](const std::pair<std::string, GameObject*>& Object) {
		return Object.second->DeleteObjectMarked;
		});

	if (It == 0)
		return;

	for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]);) {
		if ((*It)->DeleteObjectMarked) {
			delete* It;
			*It = nullptr;
			It = ObjectDeque[i].erase(It);
			continue;
		}

		if ((*It)->SwapLayerMarked) {
			int DestLayer = (*It)->DestLayer;
			ObjectDeque[DestLayer].push_back(*It);
			ObjectDeque[DestLayer].back()->SwapLayerMarked = false;
			It = ObjectDeque[i].erase(It);
			continue;
		}

		++It;
	}
}

void Framework::ClearFloatingObject() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
			if ((*It)->FloatingObjectMarked && !(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}

void Framework::ClearAll() {
	for (int i = 0; i < Layers; ++i) {
		for (auto It = begin(ObjectDeque[i]); It != end(ObjectDeque[i]); ++It) {
			if (!(*It)->StaticObjectMarked)
				(*It)->DeleteObjectMarked = true;
		}
	}
}
