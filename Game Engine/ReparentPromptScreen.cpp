#include "UI/ReparentPromptScreen.h"
#include "GameObject/ParentingManager.h"
#include  <algorithm>
void ReparentPromptScreen::draw()
{

	if(!invoker || selection) return;

	ImGui::SetNextWindowSize(ImVec2(200, 500));
	ImGui::Begin("Select Object");

	if(ImGui::Button("Cancel"))
		invoker = nullptr;

	if (invoker)
		this->drawParents(nullptr);

	ImGui::End();
}

void ReparentPromptScreen::drawParents(Entity* parent)
{
	auto root_objects = ParentingManager::get().GetChildren(parent);
	for (auto i : root_objects) {
		
		if(invoker == i)
			continue;

		const char* p_name = i->m_name.c_str();
		if (ImGui::Button(p_name)) {
			selection = (Entity*)i;
		}
		ImGui::Indent();
		this->drawParents((Entity*)i);
		ImGui::Unindent();
	}
}

