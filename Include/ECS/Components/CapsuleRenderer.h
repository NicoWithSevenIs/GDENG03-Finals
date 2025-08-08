#pragma once


#include <filesystem>
#include "ECS/Components/Component.h"

#include "Rendering/GraphicsEngine.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexShader.h"
#include "Rendering/PixelShader.h"
#include "Rendering/ConstantBuffer.h"


#include "Game Engine/EngineTime.h"

class CapsuleRenderer : public Component {

private:
	IndexBuffer* m_ib;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

	Texture* m_tex;

	std::vector<vertex> cube_list;
	std::vector<unsigned int> index_list;
public:
	bool isTextured = false;

public:
	inline CapsuleRenderer() : Component(ComponentType::RENDERER) {
		//Load();
	}
	inline CapsuleRenderer(Entity* newOwner) : Component(ComponentType::RENDERER, newOwner) {
		//Load();
	}
	inline void Update(constant cc) override {
		m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
		Draw();
	}
	inline Component* CreateSnapshot() override {
		return new CapsuleRenderer(this->owner);
	}
	inline void Release() override {
		//this->m_ib->release();
		//this->m_vb->release();
		//this->m_vs->release();
		//this->m_ps->release();
		//this->m_cb->release();
		//delete this;
	}

	void Load();
	void Draw();

private:
	void MakeCapsule(vertex* list, unsigned int* index_list);
public:
	void TextureChange(std::string texture_path);
	inline std::string ExportToJSON() override {
		std::stringstream s;
		s << "\"Renderer\": \"{";
		s << "\\\"Type\\\": \\\"Capsule\\\",";
		s << "\\\"Texture\\\": \\\"";
		s << "\\\"}\"";
		return s.str();
	}


};