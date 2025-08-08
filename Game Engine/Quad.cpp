#include "GameObject/Quad.h"


Quad::Quad() : GameObject("Quad") {}

Quad::Quad(std::string name) : GameObject(name) {}


bool Quad::load()
{
	this->strip_list = 
	{
		{Vector3D(-0.5f, -0.5f, 0.0f), Vector3D(1,1,1), Vector3D(1,1,1)},
		{Vector3D(-0.5f, 0.5f, 0.0f), Vector3D(1,1,1), Vector3D(1,1,1)},
		{Vector3D(0.5f,  -0.5f, 0.0f), Vector3D(1,1,1), Vector3D(1,1,1)},
		{Vector3D(0.5f, 0.5f, 0.0f), Vector3D(1,1,1), Vector3D(1,1,1)},
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

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	return true;
}

bool Quad::release()
{
	this->m_vb->release();
	this->m_vs->release();
	this->m_ps->release();
	this->m_cb->release();
	delete this;
	return true;
}

void Quad::Draw() {
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(this->m_vb->getSizeVertexList(), 0);
}

void Quad::Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix)
{
	GameObject::Update(delta_time, view_matrix, projection_matrix);

	cc.m_angle = delta_time;
	cc.m_view = view_matrix;
	cc.m_proj = projection_matrix;
	cc.m_color = m_color;
	cc.isRandom = isRainbow;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

}