#pragma once
#include <../Include/tiny_obj_loader.h>

#include <vector>
#include "ECS/Components/Component.h"
#include "Rendering/MeshVertex.h"

#include "Rendering/GraphicsEngine.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexShader.h"
#include "Rendering/PixelShader.h"
#include "Rendering/ConstantBuffer.h"


#include "Game Engine/EngineTime.h"

class MeshRenderer : public Component
{
	private:
		std::vector<MeshVertex> list_vertices;
		std::vector<unsigned int> list_indices;

		IndexBuffer* m_ib;
		VertexBuffer* m_vb;
		VertexShader* m_vs;
		PixelShader* m_ps;
		ConstantBuffer* m_cb;
		Texture* m_tex;

		std::string model_path;
		std::string tex_path;

	public:
		bool isTextured = false;
	public:
		inline MeshRenderer() : Component(ComponentType::RENDERER) {
			
		}
		inline MeshRenderer(Entity* newOwner) : Component(ComponentType::RENDERER, newOwner) {
			
		}
		inline void Update(constant cc) override {
			m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
			Draw();
		}
		inline Component* CreateSnapshot() override {
			MeshRenderer* newMesh = new MeshRenderer(this->owner);
			newMesh->Load(model_path, tex_path);
			return newMesh;
		}
		inline void Release() override {
			//this->m_ib->release();
			//this->m_vb->release();
			//this->m_vs->release();
			//this->m_ps->release();
			//this->m_cb->release();
			//delete this;
		}

		void Load(std::string model_path, std::string tex_path);
		void Draw();

};