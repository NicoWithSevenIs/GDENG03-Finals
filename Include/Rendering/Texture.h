#pragma once
#include <d3d11.h>
#include "DirectXTex.h"
#include <string>
class Texture
{
	public:
		Texture();
		~Texture();

		ID3D11Resource* m_texture = nullptr;
		ID3D11ShaderResourceView* m_shader_res_view = nullptr;
		ID3D11SamplerState* m_sampler_state = nullptr;

		bool load(std::string path);

};

