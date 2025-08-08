#include "ECS/Systems/SceneStateManager.h"

#include "ECS/Systems/EntityManager.h"
#include "GameObject/ParentingManager.h"
#include "ECS/Systems/TimelineManager.h"

SceneStateManager::SceneStateManager(): current_state(SceneState::EDIT){}

void SceneStateManager::Update()
{
	if(UpdateCallbacks.contains(current_state))
		UpdateCallbacks[current_state]();
}

void SceneStateManager::SwitchState(SceneState state)
{
	if(current_state == state)
		return;


	switch (current_state) {
		case SceneState::EDIT:
			switch (state) {
				case SceneState::PLAY:
					std::cout << "Playing" << std::endl;
					TimelineManager::get().CloneScene();
				break;
			}
		break;

		case SceneState::PLAY:
			switch (state) {
				case SceneState::EDIT:
					std::cout << "Stopping" << std::endl;
					for (auto e : EntityManager::get_all()) {
						e->Release();
						delete e;
					}
					TimelineManager::get().RefreshScene();
				break;
				case SceneState::PAUSED:
				break;
			}
		break;

		case SceneState::PAUSED:
			switch (state) {
				case SceneState::EDIT:
					for (auto e : EntityManager::get_all()) {
						e->Release();
						delete e;
					}
					TimelineManager::get().RefreshScene();
				break;
				case SceneState::PLAY:
					break;
			}
		break;
	}

	current_state = state;
	for (auto callback : OnStateChanged) {
		callback(state);
	}
}

void SceneStateManager::FrameStep()
{
	if (UpdateCallbacks.contains(SceneState::PLAY))
		UpdateCallbacks[SceneState::PLAY]();
}
