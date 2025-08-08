#include "GameObject/ParentingManager.h"

void ParentingManager::AddObject(IGUID* object, IGUID* parent)
{
	this->descendants[object] = parent;
}

void ParentingManager::RemoveObject(IGUID* object)
{
	if (!this->descendants.contains(object))
		return;

	for (auto i : this->descendants) {
		Unparent(i.first);
	}

	this->descendants.erase(object);
}

void ParentingManager::SetParent(IGUID* object, IGUID* new_parent)
{
	if (this->descendants.contains(object)) {
		if (new_parent == nullptr)
			object->OnUnparent();
		this->descendants[object] = new_parent;
	}
}

void ParentingManager::Unparent(IGUID* object)
{
	if (this->descendants.contains(object)) {
		SetParent(object, nullptr);
	}
}

bool ParentingManager::hasChild(IGUID* object)
{
	if (this->descendants.contains(object)) {
		return this->descendants[object] != nullptr;
	}
	return false;
}


IGUID* ParentingManager::GetParent(IGUID* object)
{
	return this->descendants.contains(object) ? this->descendants[object] : nullptr;	
}

std::vector<IGUID*> ParentingManager::GetChildren(IGUID* object)
{
	std::vector<IGUID*> children;

	for (auto i : this->descendants) {
		if (i.second == object) {
			children.push_back(i.first);
		}
	}
	
	return children;
}
