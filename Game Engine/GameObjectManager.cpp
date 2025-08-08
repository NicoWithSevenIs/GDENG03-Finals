#include "GameObject/GameObjectManager.h"
#include <iostream>

void GameObjectManager::Initialize()
{
	

	/*

	Cube* child11 = new Cube("Cube 4");
	child11->load();
	child11->m_transform.m_translation = Vector3D(0, -2, 0);
	cubes.push_back(child11);
	ParentingManager::get().AddObject(child11, child1);

	Cube* child111 = new Cube("Cube 5");
	child111->load();
	child111->m_transform.m_translation = Vector3D(0, -5, 0);
	cubes.push_back(child111);
	ParentingManager::get().AddObject(child111, child11);

	Cube* child2 = new Cube("Cube 3");
	child2->load();
	child2->m_transform.m_translation = Vector3D(4, 0, 0);
	cubes.push_back(child2);
	ParentingManager::get().AddObject(child2, root);
	
	*/

}

void GameObjectManager::Update(Matrix4x4 view_mat, Matrix4x4 proj_mat)
{
	for (auto i : get().m_map_objects) {
		i.second->Update(EngineTime::deltaTime(), view_mat, proj_mat);
	}
}

void GameObjectManager::Draw()
{
	for (auto i : get().m_map_objects) {
		i.second->Draw();
	}
}

void GameObjectManager::AddObject(GameObject* object, GameObject* parent)
{
	if (!get().m_map_objects.contains(object->m_name)) {
		get().m_map_objects[object->m_name] = object;
		ParentingManager::get().AddObject(object, parent);
	}		
}

void GameObjectManager::Release()
{
	for (auto i : get().m_map_objects) {
		i.second->release();
	}
	get().m_map_objects.clear();
	ParentingManager::get().Release();
}

void GameObjectManager::MakeCube(std::string name)
{
	if (!get().m_map_objects.contains(name)) {
		Cube* root = new Cube(name);
		root->load();
		AddObject(root);
	}

}

void GameObjectManager::MakeQuad(std::string name)
{
	if (!get().m_map_objects.contains(name)) {
		Quad* root = new Quad(name);
		root->load();
		AddObject(root);
	}
}

void GameObjectManager::DoOnAll(std::function<void(GameObject*)> callback)
{
	for (auto i : get().m_map_objects) {
		callback(i.second);
	}
}

std::vector<GameObject*> GameObjectManager::get_all()
{
	std::vector<GameObject*> all;

	for (auto i : get().m_map_objects) {
		all.push_back(i.second);
	}

	return all;
}
