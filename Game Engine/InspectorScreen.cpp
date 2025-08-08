#include "ECS/Systems/PhysicsSystem.h"
#include "UI/InspectorScreen.h"
#include "ECS/Systems/TimelineManager.h"
#include "Utilities.h"
#include "Settings.h"
#include "ECS/Systems/SceneStateManager.h"
#include "../Include/ECS/Components/CubeRenderer.h"
#include "../Include/ECS/Components/SphereRenderer.h"

InspectorScreen::InspectorScreen(float width, float height)
{
	this->width = width;
	this->height = height;
	this->m_name = "Inspector";
}

InspectorScreen::~InspectorScreen()
{
}

void InspectorScreen::draw()
{

	if(!Settings::ShowEditorInterfaceInPlayMode && SceneStateManager::get().CurrentState() == SceneState::PLAY)
		return;

	this->getTrackedTransform();

	float windowWidth = 300;
	ImGui::SetNextWindowSize(ImVec2(windowWidth, 500));
	ImGui::SetNextWindowPos(ImVec2(width - windowWidth, 20));
	ImGui::Begin("Inspector");

	if (this->currTrackedObject != nullptr) {
		this->drawInspector();
		if (ImGui::Button("Reparent")) {
			prompt.invoker = currTrackedObject;
		}
		if (prompt.selection != nullptr) {
			ParentingManager::get().SetParent(currTrackedObject, prompt.selection);
			TimelineManager::get().SetDirty();
			prompt.invoker = nullptr;
			prompt.selection = nullptr;
		}
		if (ParentingManager::get().hasChild(currTrackedObject) && ImGui::Button("Unparent")) {
			ParentingManager::get().Unparent(currTrackedObject);
			TimelineManager::get().SetDirty();
		}
		this->showTextureOptions();

		if (!hasRB) {
			ImGui::Separator();
			if (ImGui::Button("Add RigidBody")) {
				this->hasRB = true;
			}
		}
		else {
			this->drawPhysicsComponent();
		}
	}

	ImGui::End();

	prompt.draw();
}

void InspectorScreen::drawInspector()
{
	ImGui::Text("%s", m_tracked_name.c_str());

	ImGui::Text("Translation");
	this->drawTranslateFields();

	ImGui::Text("Scale");
	this->drawScaleFields();

	ImGui::Text("Rotation");
	this->drawRotFields();

	if (ImGui::Checkbox("Enabled", &this->enabled)) {
		this->currTrackedObject->enabled = this->enabled;
		TimelineManager::get().SetDirty();
	}
}

void InspectorScreen::drawTranslateFields()
{
	float inputWidth = 60.0f;
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("##Translate_X", &this->m_translate_x)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Translate_Y", &this->m_translate_y)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Translate_Z", &this->m_translate_z)) {
		this->applyChanges();
	}
	ImGui::PopItemWidth();
}

void InspectorScreen::drawScaleFields()
{
	float inputWidth = 60.0f;

	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("##Scale_X", &this->m_scale_x)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Scale_Y", &this->m_scale_y)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Scale_Z", &this->m_scale_z)) {
		this->applyChanges();
	}
	ImGui::PopItemWidth();
}

void InspectorScreen::drawRotFields()
{
	float inputWidth = 60.0f;
	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::PushItemWidth(inputWidth);
	if (ImGui::InputFloat("##Rot_X", &this->m_rot_x)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Y");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Rot_Y", &this->m_rot_y)) {
		this->applyChanges();
	}
	ImGui::SameLine();

	ImGui::Text("Z");
	ImGui::SameLine();
	if (ImGui::InputFloat("##Rot_Z", &this->m_rot_z)) {
		this->applyChanges();
	}
	ImGui::PopItemWidth();
}

void InspectorScreen::drawPhysicsComponent()
{
	if (this->currTrackedObject->GetComponent<PhysicsComponent>() == nullptr) {
		PhysicsComponent* p6component = this->currTrackedObject->AddComponent<PhysicsComponent>(reactphysics3d::BodyType::STATIC);
		PhysicsSystem::AddPhysicsComponent(p6component);
	}

	ImGui::Separator();
	ImGui::Text("RigidBody");

	std::vector<std::string> items = { "STATIC", "DYNAMIC" };

	if (ImGui::BeginCombo("Dropdown", items[this->rbItem].c_str())) {
		for (int i = 0; i < items.size(); i++) {
			bool selected = (this->rbItem == i);
			if (ImGui::Selectable(items[i].c_str(), selected))
				this->rbItem = i;

			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (items[this->rbItem] == "STATIC") {
		PhysicsComponent* p6 = this->currTrackedObject->GetComponent<PhysicsComponent>();
		p6->SetBodyType(reactphysics3d::BodyType::STATIC);
	}

	if (items[this->rbItem] == "DYNAMIC") {
		PhysicsComponent* p6 = this->currTrackedObject->GetComponent<PhysicsComponent>();
		p6->SetBodyType(reactphysics3d::BodyType::DYNAMIC);
	}


	if (ImGui::Button("Remove RigidBody")) {
		this->hasRB = false;
		this->currTrackedObject->RemoveComponent<PhysicsComponent>();
		PhysicsComponent* p6 = this->currTrackedObject->GetComponent<PhysicsComponent>();
		PhysicsSystem::RemovePhysicsComponent(p6);
	}
}

void InspectorScreen::showTextureOptions()
{
	ImGui::Separator();
	if (ImGui::BeginMenu("Change Texture")) {
		if (ImGui::MenuItem("Brick")) {
			this->changeTextures("Assets/Textures/brick.png");
		}
		if (ImGui::MenuItem("Wood")) {
			this->changeTextures("Assets/Textures/wood.jpg");
		}
		if (ImGui::MenuItem("Grass")) {
			this->changeTextures("Assets/Textures/grass.jpg");
		}
		if (ImGui::MenuItem("Stars")) {
			this->changeTextures("Assets/Textures/stars_map.jpg");
		}
		if (ImGui::MenuItem("Ground")) {
			this->changeTextures("Assets/Textures/ground.jpg");
		}
		if (ImGui::MenuItem("None")) {
			//this->changeTextures("Assets/Textures/ground.jpg");
			this->currTrackedObject->cc.hasTex = false;
		}
		ImGui::EndMenu();
	}
}

void InspectorScreen::changeTextures(std::string tex_name)
{
	if (this->currTrackedObject != nullptr) {
		if (this->currTrackedObject->GetComponent<CubeRenderer>() != nullptr) {
			this->currTrackedObject->GetComponent<CubeRenderer>()->TextureChange(tex_name);
		}
		if (this->currTrackedObject->GetComponent<SphereRenderer>() != nullptr) {
			this->currTrackedObject->GetComponent<SphereRenderer>()->TextureChange(tex_name);
		}
	}
}

void InspectorScreen::getTrackedTransform()
{
	if(this->m_tracked_name == "")
		return;

	this->currTrackedObject = Utilities::Select<Entity*>(EntityManager::get_all(), [this](Entity* e){ return e->m_name == this->m_tracked_name;});
	if (this->currTrackedObject != nullptr) {
		this->m_translate_x = this->currTrackedObject->m_transform.m_translation.m_x;
		this->m_translate_y = this->currTrackedObject->m_transform.m_translation.m_y;
		this->m_translate_z = this->currTrackedObject->m_transform.m_translation.m_z;

		this->m_scale_x = this->currTrackedObject->m_transform.m_scale.m_x;
		this->m_scale_y = this->currTrackedObject->m_transform.m_scale.m_y;
		this->m_scale_z = this->currTrackedObject->m_transform.m_scale.m_z;

		this->m_rot_x = this->currTrackedObject->m_transform.m_rotation.m_x;
		this->m_rot_y = this->currTrackedObject->m_transform.m_rotation.m_y;
		this->m_rot_z = this->currTrackedObject->m_transform.m_rotation.m_z;

		this->m_tracked_name = this->currTrackedObject->m_name.c_str();
		this->enabled =  this->currTrackedObject->enabled;
	}

}

void InspectorScreen::applyChanges()
{
	if (this->currTrackedObject == nullptr) return;

	this->currTrackedObject->m_transform.m_translation.m_x = this->m_translate_x;
	this->currTrackedObject->m_transform.m_translation.m_y = this->m_translate_y;
	this->currTrackedObject->m_transform.m_translation.m_z = this->m_translate_z;

	this->currTrackedObject->m_transform.m_scale.m_x = this->m_scale_x;
	this->currTrackedObject->m_transform.m_scale.m_y = this->m_scale_y;
	this->currTrackedObject->m_transform.m_scale.m_z = this->m_scale_z;

	this->currTrackedObject->m_transform.m_rotation.m_x = this->m_rot_x;
	this->currTrackedObject->m_transform.m_rotation.m_y = this->m_rot_y;
	this->currTrackedObject->m_transform.m_rotation.m_z = this->m_rot_z;

	TimelineManager::get().SetDirty();
}


bool InspectorScreen::checkParentValid(std::string name)
{
	std::vector<IGUID*> parents;
	for (const auto& pair : ParentingManager::get().GetHierarchy()) {
		parents.push_back(pair.first);
	}

	for (IGUID* target : parents) {
		if (target->m_name == name) {
			return true;
		}
	}

	return false;
}

IGUID* InspectorScreen::getParentGUID(std::string name)
{
	std::vector<IGUID*> parents;
	for (const auto& pair : ParentingManager::get().GetHierarchy()) {
		parents.push_back(pair.first);
	}

	for (IGUID* target : parents) {
		if (target->m_name == name) {
			return target;
		}
	}

	return nullptr;
}




