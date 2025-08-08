#include "UI/SceneStateUI.h"
#include "ECS/Systems/SceneStateManager.h"


SceneStateUI::SceneStateUI(float width, float height)
{
	this->width = width;
	this->height = height;
}

void SceneStateUI::draw()
{
	ImGui::SetNextWindowSize(ImVec2(250,35));
	ImGui::SetNextWindowPos(ImVec2(width/2 - 200, 30));
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

	switch (SceneStateManager::get().CurrentState()) {
		case SceneState::EDIT:
			if(ImGui::Button("Play")){
				SceneStateManager::get().SwitchState(SceneState::PLAY);
			}
		break;

		case SceneState::PLAY:
			if (ImGui::Button("Pause")) {
				SceneStateManager::get().SwitchState(SceneState::PAUSED);
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				SceneStateManager::get().SwitchState(SceneState::EDIT);
			}
		break;

		case SceneState::PAUSED:
			if (ImGui::Button("Resume")) {
				SceneStateManager::get().SwitchState(SceneState::PLAY);
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				SceneStateManager::get().SwitchState(SceneState::EDIT);
			}
			ImGui::SameLine();
			if (ImGui::Button("Next Frame")) {
				SceneStateManager::get().FrameStep();
			}
		break;

	}
	ImGui::End();
}
