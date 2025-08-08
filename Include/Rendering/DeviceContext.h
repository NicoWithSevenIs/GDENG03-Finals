#pragma once
#include <d3d11.h>
#include "Rendering/Texture.h"
class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;

class DeviceContext {
	public:
		ID3D11DeviceContext* m_device_context;

	private:
		friend class ConstantBuffer;

	public:
		DeviceContext(ID3D11DeviceContext* device_context);
		~DeviceContext();

		bool clearRenderTargetColor(SwapChain* swap_chain,  float red, float green, float blue, float alpha);
		bool setVertexBuffer(VertexBuffer* vertex_buffer);
		void setIndexBuffer(IndexBuffer* index_buffer);

		void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
		void drawIndexedList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
		void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);
		void drawIndexedTriangleList(UINT index_count, UINT vertex_count, UINT start_vertex_index, UINT start_index_loc);

		void setViewportSize(UINT width, UINT height);

		void setVertexShader(VertexShader* vertex_shader);
		void setPixelShader(PixelShader* pixel_shader);

		void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
		void setConstantBuffer(PixelShader* vertex_shader, ConstantBuffer* buffer);

		void setTexture(VertexShader* vertex_shader, Texture* tex);
		void setTexture(PixelShader* pixel_shader, Texture* tex);


		bool release();

		ID3D11DeviceContext* getContext(){return m_device_context;}
};