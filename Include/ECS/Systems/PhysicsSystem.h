#pragma once

#include "reactphysics3d/reactphysics3d.h"
#include "ECS/Components/PhysicsComponent.h"
#include "Interfaces/Singleton.h"

#include "vector"
#include "Math/Vector3D.h"
#include "Game Engine/EngineTime.h"
#include <algorithm>

class PhysicsSystem : public Singleton<PhysicsSystem>
{
	public: 
		static void Initialize();
		static void AddPhysicsComponent(PhysicsComponent* component);
		static void RemovePhysicsComponent(PhysicsComponent* component);
		static std::vector<PhysicsComponent*> GetAllPhysicsComponents();
		static void Release();
	
		static void UpdateAllPhysicsComponents();
		static reactphysics3d::PhysicsWorld* GetPhysicsWorld();
		static reactphysics3d::PhysicsCommon* GetPhysicsCommon();
	
	private:
		std::vector<PhysicsComponent*> physicsComponentList;
		reactphysics3d::PhysicsWorld* physicsWorld = nullptr;
		reactphysics3d::PhysicsCommon* physicsCommon = nullptr;
};

