#pragma once
#include "Math/Constant.h"

enum class ComponentType {
	SCRIPT,
	RENDERER,
	PHYSICS
};

class Entity;

class Component
{
	public:
		Entity* owner;

	public:
		const ComponentType Type;
		inline Component(ComponentType type) : Type(type) {}
		inline Component(ComponentType type, Entity* newOwner) : Type(type), owner(newOwner){}

		inline virtual Component* CreateSnapshot() = 0;

		virtual void Update(constant c) = 0;
		virtual void Release() = 0;
		virtual void Init() {}

		inline virtual std::string ExportToJSON() {
			return std::string();
		}
};

