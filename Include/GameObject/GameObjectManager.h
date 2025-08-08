#pragma once

#include "Interfaces/Singleton.h"

#include "GameObject/GameObject.h"
#include "GameObject/ParentingManager.h"
#include "GameObject/Cube.h"
#include "GameObject/Quad.h"

#include "Game Engine/EngineTime.h"

#include "Rendering/Mesh.h"

#include <functional>
#include <vector>
#include <unordered_map>

class GameObjectManager: public Singleton<GameObjectManager>
{
	private:
		std::unordered_map<std::string, GameObject*> m_map_objects;

	public:
		static void Initialize();
		static void Update(Matrix4x4 view_mat, Matrix4x4 proj_mat);
		static void Draw();
		static void AddObject(GameObject* object, GameObject* parent = nullptr);
		static void Release();

		static void MakeCube(std::string name);
		static void MakeQuad(std::string name);

		static void DoOnAll(std::function<void(GameObject*)> callback);
		static std::vector<GameObject*> get_all();
};

