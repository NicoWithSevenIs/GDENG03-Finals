#pragma once

#include "vector"
#include "unordered_map"
#include "string"

#include "ECS/Entities/Entity.h"
#include "ECS/Systems/EntityManager.h"
#include "UI/UIScreen.h"

#include <vector>


class HierarchyScreen : public UIScreen
{
	public:
		HierarchyScreen(float width, float height);
		~HierarchyScreen();

	public:
		virtual void draw() override;
	
	private:
		void drawParents(Entity* parent);

};

