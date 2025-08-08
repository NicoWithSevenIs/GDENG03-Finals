#pragma once

#include "ECS/Entities/Entity.h"

struct SceneSnapshot {
	public:
		std::vector<Entity*> m_entities;
		std::unordered_map<IGUID*, IGUID*> m_hierarchy;

		SceneSnapshot(std::vector<Entity*> entities, std::unordered_map<IGUID*, IGUID*> hierarchy) {
			CreateSnapshot(entities, hierarchy);
		}

	private:
		void CreateSnapshot(std::vector<Entity*> entities, std::unordered_map<IGUID*, IGUID*> hierarchy) {
			std::unordered_map<IGUID*, IGUID*> copied;
			for (auto entity : entities) {
				Entity* e = entity->CreateSnapshot();
				copied[entity] = e;
				m_entities.push_back(e);
			}
			for (auto instance : hierarchy) {
				m_hierarchy[copied[instance.first]] = instance.second != nullptr ? copied[instance.second] : nullptr;
			}
		}

	public:
		void ReleaseSnapshot() {
			m_hierarchy.clear();
			for(auto e: m_entities)
				e->Release();
		}
};