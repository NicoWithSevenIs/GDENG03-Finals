#pragma once
#include "GameObject/IGUID.h"
#include "Interfaces/Singleton.h"

#include <unordered_map>
#include <vector>

class ParentingManager: public Singleton<ParentingManager>
{
	std::unordered_map<IGUID*, IGUID*> descendants;

	public:
		void AddObject(IGUID* object, IGUID* parent = nullptr);
		void RemoveObject(IGUID* object);
		void SetParent(IGUID* object, IGUID* new_parent);
		void Unparent(IGUID* object);
		bool hasChild(IGUID* object);
	
		IGUID* GetParent(IGUID* object);
		std::vector<IGUID*> GetChildren(IGUID* object);

		inline const std::unordered_map<IGUID*, IGUID*>& GetHierarchy() const {
			return descendants;
		}

		inline void Release() {
			descendants.clear();
		}

		inline std::unordered_map<IGUID*, IGUID*> GetAncestry(){
			return std::unordered_map<IGUID*, IGUID*> (descendants);
		}

		inline void ReplaceHierarchy(std::unordered_map<IGUID*, IGUID*> hierarchy){
			descendants.clear();
			descendants = hierarchy;
		}
};