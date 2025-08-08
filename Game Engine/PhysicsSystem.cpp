#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Entities/Entity.h"

void PhysicsSystem::Initialize()
{
    get().physicsCommon = new  reactphysics3d::PhysicsCommon();
    reactphysics3d::PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = 50;
    settings.gravity = reactphysics3d::Vector3(0, -9.81, 0);
    get().physicsWorld = get().physicsCommon->createPhysicsWorld(settings);
   // std::cout << "Created Physics System";
}

void PhysicsSystem::AddPhysicsComponent(PhysicsComponent* component)
{
    get().physicsComponentList.push_back(component);
}

void PhysicsSystem::RemovePhysicsComponent(PhysicsComponent* component)
{
    int index = -1;
    for (int i = 0; i < get().physicsComponentList.size(); i++) {
        if (component == get().physicsComponentList[i]) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        get().physicsComponentList.erase(get().physicsComponentList.begin() + index);
    }
}

std::vector<PhysicsComponent*> PhysicsSystem::GetAllPhysicsComponents()
{
    return get().physicsComponentList;
}

void PhysicsSystem::Release()
{
    delete get().physicsCommon;
}

void PhysicsSystem::UpdateAllPhysicsComponents()
{
    if (EngineTime::deltaTime() > 0.0f) {
        get().physicsWorld->update(EngineTime::deltaTime());
        for (PhysicsComponent* pc : get().physicsComponentList) {
            pc->Update(pc->owner->cc);
           // pc->Perform(EngineTime::deltaTime());
        }
    }
}

reactphysics3d::PhysicsWorld* PhysicsSystem::GetPhysicsWorld()
{
    return get().physicsWorld;
}

reactphysics3d::PhysicsCommon* PhysicsSystem::GetPhysicsCommon()
{
    return get().physicsCommon;
}
