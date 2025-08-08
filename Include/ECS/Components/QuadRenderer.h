#pragma once
#include "ECS/Components/Component.h"

#include "Rendering/GraphicsEngine.h"
#include "Rendering/DeviceContext.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/VertexShader.h"
#include "Rendering/PixelShader.h"
#include "Rendering/ConstantBuffer.h"


#include "Game Engine/EngineTime.h"

class QuadRenderer : public Component {

private:
	IndexBuffer* m_ib;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;

	Texture* m_tex;

	std::vector<vertex> strip_list;
	std::vector<unsigned int> index_list;
public:
	bool isTextured = false;

public:
	inline QuadRenderer() : Component(ComponentType::RENDERER) {
		Load();
	}
	inline QuadRenderer(Entity* newOwner) : Component(ComponentType::RENDERER, newOwner) {
		Load();
	}
	inline void Update(constant cc) override {
		m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
		Draw();
	}
	inline Component* CreateSnapshot() override {
		return new QuadRenderer();
	}
	inline void Release() override {
		this->m_ib->release();
		this->m_vb->release();
		this->m_vs->release();
		this->m_ps->release();
		this->m_cb->release();
		delete this;
	}
private:
	inline void Load() {

		this->strip_list =
		{
			{Vector3D(-0.5f, -0.5f, 0.0f), Vector2D(0,0), Vector3D(1,1,1)},
			{Vector3D(-0.5f, 0.5f, 0.0f), Vector2D(0,1), Vector3D(1,1,1)},
			{Vector3D(0.5f,  -0.5f, 0.0f), Vector2D(1,0), Vector3D(1,1,1)},
			{Vector3D(0.5f, 0.5f, 0.0f), Vector2D(1,1), Vector3D(1,1,1)},
		};

		this->m_vb = GraphicsEngine::get()->createVertexBuffer();

		UINT size_list = strip_list.size();

		void* shader_byte_code = nullptr;
		size_t size_shader = 0;



		GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
		this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

		this->m_vb->load(strip_list.data(), sizeof(vertex), size_list, shader_byte_code, size_shader);

		GraphicsEngine::get()->releaseCompiledShader();

		GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
		this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

		GraphicsEngine::get()->releaseCompiledShader();

		constant cc;
		std::string texture_path = "brick.png";
		m_tex = new Texture();
		if (!texture_path.empty()) {
			cc.hasTex = m_tex->load(texture_path);
			isTextured = cc.hasTex;
			std::cout << "Has Tex Bool: " << cc.hasTex << std::endl;
		}
		else {
			cc.hasTex = false;
		}

		m_cb = GraphicsEngine::get()->createConstantBuffer();
		m_cb->load(&cc, sizeof(constant));
	}
	inline void Draw() {
		GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, m_cb);
		GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, m_cb);

		GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
		GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

		if (this->owner->cc.hasTex) {
			GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_vs, m_tex);
			GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(m_ps, m_tex);
		}

		GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
		GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(this->m_vb->getSizeVertexList(), 0);
	}

};