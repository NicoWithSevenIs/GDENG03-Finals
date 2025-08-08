#pragma once

#include "UI/UIScreen.h"


class SceneStateUI : public UIScreen {
	public:
		SceneStateUI(float width, float height);
		void draw() override;
};