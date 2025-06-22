#include "ControlWindow.h"

#include <imgui.h>

#include <iostream>
#include <vector>
#include <string>


ControlWindow::ControlWindow() {
	showDemoWindow = false;
	targetScene = nullptr;
}

auto ControlWindow::Initialize() -> bool {
	return true;
}

void ControlWindow::Display() {
	ImGui::Begin("Control");
	{
		ImGui::Checkbox("Demo Window", &showDemoWindow);

	}
	ImGui::End();

	// Show the big demo window or not (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);
}

