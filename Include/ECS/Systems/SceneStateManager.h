#pragma once

#include <functional>
#include <unordered_map>
#include <Interfaces/Singleton.h>
#include <Snapshot/SceneSnapshot.h>

enum class SceneState {
	EDIT,
	PLAY,
	PAUSED,
};

class SceneStateManager: public Singleton<SceneStateManager> {

	public:
		std::unordered_map<SceneState, std::function<void()>> UpdateCallbacks;
		std::vector<std::function<void(SceneState)>> OnStateChanged;

	private:
		SceneState current_state;

	public:
		SceneStateManager();
		void Update();
		void SwitchState(SceneState state);
		void FrameStep();

	public:
		SceneState CurrentState() {return current_state;}
};