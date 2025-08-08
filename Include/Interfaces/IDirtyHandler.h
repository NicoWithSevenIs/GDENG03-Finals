#pragma once
#include "ECS/Systems/SceneStateManager.h"
class IDirtyHandler {
	protected:
		bool isDirty = false;
	public:
		inline void SetDirty() {
			if (SceneStateManager::get().CurrentState() == SceneState::EDIT) {
				isDirty = true;
			}
		}
		inline bool IsDirty() { return isDirty; }
};