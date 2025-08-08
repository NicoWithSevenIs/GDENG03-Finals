#pragma once

#include <string>
#include "ECS/Entities/Entity.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui.h"

#include <vector>
class ReparentPromptScreen
{

	public:
		
		Entity* invoker = nullptr;
		Entity* selection = nullptr;

		bool aborted = false;
		bool invoked = false;


		void draw();
		void drawParents(Entity* obj);

};

