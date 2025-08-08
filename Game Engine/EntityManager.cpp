#include "ECS/Systems/EntityManager.h"
#include "Utilities.h"

#include "ECS/Components/CubeRenderer.h"
#include "ECS/Systems/SceneStateManager.h"
void EntityManager::Initialize()
{
}

void EntityManager::Update(Matrix4x4 view_mat, Matrix4x4 proj_mat, Entity* children)
{
	auto root_objects = ParentingManager::get().GetChildren(children);

	for (auto entity : root_objects) {
		auto e = (Entity*)entity;
		if (!e->enabled) continue;
		e->Update(view_mat, proj_mat);

		if (SceneStateManager::get().CurrentState() != SceneState::EDIT) {
			auto updatables = Utilities::Where<Component*>(e->GetComponents(), 
				[](Component* c) {return c->Type == ComponentType::SCRIPT || c->Type == ComponentType::PHYSICS; });
			for (auto u : updatables)
				u->Update(e->cc);
		}
		
		e->updated = true;
		Update(view_mat, proj_mat, e);
	}
}

void EntityManager::Draw(Entity* children)
{
	
	auto root_objects = ParentingManager::get().GetChildren(children);

	for (auto entity : root_objects) {
		auto e = (Entity*)entity;
		if (!e->enabled) continue;
		auto renderers = Utilities::Where<Component*>(e->GetComponents(), [](Component* c) {return c->Type == ComponentType::RENDERER; });
		for (auto r : renderers)
			r->Update(e->cc);
		Draw(e);
	}

}

void EntityManager::AddObject(Entity* object, Entity* parent)
{
	for (auto i : get().m_object_list) {
		if (object->m_name == i->m_name)
			return;
	}

	get().m_object_list.push_back(object);
	ParentingManager::get().AddObject(object, parent);
}

void EntityManager::Release()
{
	for (auto e : get().m_object_list) {
		e->Release();
		delete e;
	}
	get().m_object_list.clear();
	ParentingManager::get().Release();
}

void EntityManager::DoOnAll(std::function<void(Entity*)> callback)
{
	for (auto e : get().m_object_list)
		callback(e);
}

std::vector<Entity*> EntityManager::get_all()
{
	return std::vector<Entity*>(get().m_object_list);
}

void EntityManager::ReplaceHierarchy(std::vector<Entity*> entities)
{
	get().m_object_list = entities;
}
