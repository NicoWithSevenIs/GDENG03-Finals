#pragma once
#include <vector>

#include "ECS/Components/Component.h"
#include "GameObject/IGUID.h"
#include "Game Engine/EngineTime.h"
#include "Math/Constant.h"
#include "Math/Transform.h"
#include "GameObject/ParentingManager.h"
class Entity: public IGUID{

	public:
		Transform m_transform;
		constant cc;
		bool enabled;
		bool updated;

	protected:
		std::vector<Component*> components;
		
	public:
		inline std::vector<Component*> GetComponents() { return std::vector<Component*>(components); }
		inline constant GetConstant() { return cc; }

	public:
		inline Entity(std::string name = "Entity") : IGUID(name), m_transform(this), enabled(true) {}
		inline void Release() {
			for(auto c : components)
				c->Release();
		}

		inline void SetActive(bool value) {
			if(enabled == value)
				return;

			enabled = value;
			auto children = ParentingManager::get().GetChildren(this);
			for (auto i : children) {
				((Entity*)i)->SetActive(value);
			}
		}

		inline Entity* CreateSnapshot() {
			
			Entity* copy = new Entity(m_name);
			for (auto c : components) {
				auto snapshot = c->CreateSnapshot();
				snapshot->owner = copy;
				copy->components.push_back(snapshot);
			}
			copy->m_transform.m_translation = m_transform.m_translation;
			copy->m_transform.m_scale = m_transform.m_scale;
			copy->m_transform.m_rotation = m_transform.m_rotation;

			return copy;
		}

	#pragma region component handling	
		template <typename T, typename... Args> inline
			typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
			AddComponent(Args&&... args)
		{
			for (auto c : components) {
				if (typeid(*c) == typeid(T))
					return nullptr;
			}
			T* component = new T(std::forward<Args>(args)...);
			component->owner = this;
			components.push_back(component);
			component->Init();
			return component;
		}

		template <typename T> inline
			typename std::enable_if<std::is_base_of<Component, T>::value, T*>::type
			GetComponent()
		{
			for (auto c : components) {
				if (typeid(*c) == typeid(T))
					return static_cast<T*>(c);
			}
			return nullptr;
		}

		template <typename T> inline
			typename std::enable_if<std::is_base_of<Component, T>::value, void>::type
			RemoveComponent()
		{
			std::erase_if(components, [](Component* c) {
				return typeid(*c) == typeid(T);
			});
		}
	#pragma endregion

	#pragma region parenting
	public:
		inline virtual void Update(Matrix4x4 view_matrix, Matrix4x4 projection_matrix) {
			cc.m_world = this->m_transform.GetTransformationMatrix();
			auto parent = (Entity*)ParentingManager::get().GetParent(this);
			auto child = this;

			while (parent != nullptr) {
				cc.m_world *= parent->m_transform.GetTransformationMatrix();
				child = parent;
				parent = parent = (Entity*)ParentingManager::get().GetParent(child);
			}

			//cc.hasTex = true;
			cc.isRandom = true;
			cc.m_angle = EngineTime::deltaTime();
			cc.m_proj = projection_matrix;
			cc.m_view = view_matrix;
			cc.m_color = Vector3D();
		}

		inline void OnUnparent() override {

			auto descendant = this;
			auto ancestry = (Entity*)ParentingManager::get().GetParent(descendant);
			Matrix4x4 ancestry_matrix = m_transform.GetTransformationMatrix();

			while (ancestry != nullptr) {
				ancestry_matrix *= ancestry->m_transform.GetTransformationMatrix();
				descendant = ancestry;
				ancestry = (Entity*)ParentingManager::get().GetParent(descendant);
			}

			auto mat = ancestry_matrix.m_mat;

			m_transform.m_translation = Vector3D(mat[3][0], mat[3][1], mat[3][2]);

			auto row_1 = Vector3D(mat[0][0], mat[0][1], mat[0][2]);
			auto row_2 = Vector3D(mat[1][0], mat[1][1], mat[1][2]);
			auto row_3 = Vector3D(mat[2][0], mat[2][1], mat[2][2]);

			auto scale = Vector3D(
				Vector3D::magnitude(row_1),
				Vector3D::magnitude(row_2),
				Vector3D::magnitude(row_3)
			);

			m_transform.m_scale = scale;

			float pitch, yaw, roll;

			auto parent = (Entity*)ParentingManager::get().GetParent(this);
			Entity* child = this;

			pitch = m_transform.m_rotation.m_x;
			yaw = m_transform.m_rotation.m_y;
			roll = m_transform.m_rotation.m_z;

			while (parent != nullptr) {
				pitch += parent->m_transform.m_rotation.m_x;
				yaw += parent->m_transform.m_rotation.m_y;
				roll += parent->m_transform.m_rotation.m_z;

				child = parent;
				parent = parent = (Entity*)ParentingManager::get().GetParent(child);
			}
			m_transform.m_rotation = Vector3D(pitch, yaw, roll);
		}

	#pragma endregion

	public:
		inline void ExportToJSON(std::string& output) {
			std::stringstream s;

			s << 
			"{\n" 
				<< "\"Name\": \"" <<m_name << "\",\n"
				<< "\"Enabled\": " << enabled << ",\n"
				<< m_transform.ExportToJSON() << ",\n"
				<< "\"Components\": {\n";

			for(int i =0; i < components.size(); i++){
				s << components[i]->ExportToJSON();
				if(i < components.size()-1)
					s << ",\n";
			}

			s << "\n}\n}";
			
			output = s.str();
		}

};