#include "Ground.h"

Ground::Ground() {
	SetImage(Image, "ground");
}

void Ground::Render() {
	BeginProcess(ImageRenderMode::Default);
	SetPosition(0.0, FLOOR_HEIGHT);
	Scale(2.0, 2.0);
	RenderImage(Image, 1.0, 4500, 800);
}