#include "GameObject/Cube.h"
#include "Rendering/GraphicsEngine.h"
#include "Rendering/DeviceContext.h"
#include "GameObject/ParentingManager.h"

Cube::Cube() : GameObject("Cube") {}

Cube::Cube(std::string name): GameObject(name) {}



bool Cube::load()
{	
	this->cube_list = {
		{Vector3D(-0.5f, -0.5f, -0.5f), getRandom(2), getRandom(2)},
		{Vector3D(-0.5f,  0.5f, -0.5f), getRandom(2), getRandom(2)},
		{Vector3D(0.5f,   0.5f, -0.5f), getRandom(2), getRandom(2)},
		{Vector3D(0.5f,  -0.5f, -0.5f), getRandom(2), getRandom(2)},
		{Vector3D(0.5f,  -0.5f,  0.5f), getRandom(2), getRandom(2)},
		{Vector3D(0.5f,   0.5f,  0.5f), getRandom(2), getRandom(2)},
		{Vector3D(-0.5f,  0.5f,  0.5f), getRandom(2), getRandom(2)},
		{Vector3D(-0.5f, -0.5f,  0.5f), getRandom(2), getRandom(2)},
	};
	
	this->index_list =
	{
		// FRONT SIDE
		0, 1, 2,   // FIRST TRIANGLE
		2, 3, 0,   // SECOND TRIANGLE

		// BACK SIDE
		4, 5, 6,
		6, 7, 4,

		// TOP SIDE
		1, 6, 5,
		5, 2, 1,

		// BOTTOM SIDE
		7, 0, 3,
		3, 4, 7,

		// RIGHT SIDE
		3, 2, 5,
		5, 4, 3,

		// LEFT SIDE
		7, 6, 1,
		1, 0, 7
	};


	this->m_vb = GraphicsEngine::get()->createVertexBuffer();
	this->m_ib = GraphicsEngine::get()->createIndexBuffer();

	UINT size_list = cube_list.size();
	UINT size_index_list = index_list.size();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	this->m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	this->m_vb->load(cube_list.data(), sizeof(vertex), size_list, shader_byte_code, size_shader);
	this->m_ib->load(index_list.data(), size_index_list);

	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	this->m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	constant cc;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	pace = (float)(rand() % 120 + 20)/100 ;
	dir = (float)(rand()%8-4);
	dir = dir == 0? 1 : dir;

	return true;
}

bool Cube::release()
{
	this->m_ib->release();
	this->m_vb->release();
	this->m_vs->release();
	this->m_ps->release();
	this->m_cb->release();
	delete this;
	return true;
}

void Cube::Draw() {
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(this->m_ps, m_cb);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(this->m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(this->m_ps);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedList(m_ib->getSizeIndexList(), 0, 0);
}


#include <iostream>
void Cube::Update(float delta_time, Matrix4x4 view_matrix, Matrix4x4 projection_matrix)
{
	GameObject::Update(delta_time, view_matrix, projection_matrix);

	if(doOnUpdate)
		doOnUpdate();

	m_angle += delta_time;

	cc.m_angle = m_angle;
	cc.m_view = view_matrix;
	cc.m_proj = projection_matrix;
	cc.m_color = m_color;
	cc.isRandom = isRainbow;

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

}

