#pragma once
#include "UI/UIScreen.h"
#include <functional>
class MenuBarUI: public UIScreen
{
	private:
		std::function<void()> doOnPrompt = nullptr;
		char prompt_input[128];

		float cubeSpawnHeight = 1.5f;
		float cubeSpawnArea = 1.0f;

	public:
		MenuBarUI(float width, float height);
		void draw() override;
		void ShowPrompt();
		void Spawn100Cubes();


};

