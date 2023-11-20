#include "d3d11_renderer.hpp"

#include <cstdint>

#include <d3dcompiler.h>
#include <d3d11_1.h>

namespace onyx {

D3D11Renderer::D3D11Renderer(const HWND window_handle) {
	D3D_FEATURE_LEVEL feature_levels[]{D3D_FEATURE_LEVEL_11_0};

	// @TODO: Check for errors
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
	                  D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
	                  feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION,
	                  &base_device_, nullptr, &base_device_context_);

	base_device_->QueryInterface(IID_ID3D11Device1, reinterpret_cast<void **>(&device_));
	base_device_context_->QueryInterface(IID_ID3D11DeviceContext1, reinterpret_cast<void **>(&device_context_));

	device_->QueryInterface(IID_IDXGIDevice1, reinterpret_cast<void **>(&dxgi_device_));
	dxgi_device_->GetAdapter(&dxgi_adapter_);
	dxgi_adapter_->GetParent(IID_IDXGIFactory2, reinterpret_cast<void **>(&dxgi_factory_));

	DXGI_SWAP_CHAIN_DESC1 swap_chain_desc{
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		.SampleDesc{
			.Count = 1,
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 2,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
	};

	dxgi_factory_->CreateSwapChainForHwnd(device_, window_handle, &swap_chain_desc, nullptr, nullptr, &swap_chain_);

	swap_chain_->GetBuffer(0, IID_ID3D11Texture2D, reinterpret_cast<void **>(&framebuffer_));
	device_->CreateRenderTargetView(framebuffer_, nullptr, &framebuffer_view_);

	D3D11_TEXTURE2D_DESC depthbuffer_desc;
	framebuffer_->GetDesc(&depthbuffer_desc);

	depthbuffer_desc.Format =  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthbuffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	device_->CreateTexture2D(&depthbuffer_desc, nullptr, &depthbuffer_);
	device_->CreateDepthStencilView(depthbuffer_, nullptr, &depthbuffer_view_);

	device_context_->OMSetRenderTargets(1, &framebuffer_view_, depthbuffer_view_);
	device_context_->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	D3D11_VIEWPORT viewport{
		.Width = static_cast<float>(depthbuffer_desc.Width),
		.Height = static_cast<float>(depthbuffer_desc.Height),
	};
	device_context_->RSSetViewports(1, &viewport);

	D3DReadFileToBlob(L".\\shader_vs.cso", &vertex_shader_code_);
	D3DReadFileToBlob(L".\\shader_ps.cso", &pixel_shader_code_);

	SIZE_T size = pixel_shader_code_->GetBufferSize();

	HRESULT result;
	result = device_->CreateVertexShader(vertex_shader_code_->GetBufferPointer(), vertex_shader_code_->GetBufferSize(), 
	                            nullptr, &vertex_shader_);

	result = device_->CreatePixelShader(pixel_shader_code_->GetBufferPointer(), pixel_shader_code_->GetBufferSize(), 
	                           nullptr, &pixel_shader_);

	device_context_->VSSetShader(vertex_shader_, nullptr, 0);
	device_context_->PSSetShader(pixel_shader_, nullptr, 0);

	const float data[] = {
		+0.0f, +0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		+0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	D3D11_BUFFER_DESC vertex_buffer_desc{
		.ByteWidth = sizeof(data),
		.Usage = D3D11_USAGE_IMMUTABLE,
		.BindFlags = D3D11_BIND_VERTEX_BUFFER
	};

	D3D11_SUBRESOURCE_DATA vertex_buffer_data{static_cast<const void *>(data)};

	device_->CreateBuffer(&vertex_buffer_desc, &vertex_buffer_data, &vertex_buffer_);

	D3D11_INPUT_ELEMENT_DESC shader_varying_desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 3u * sizeof(float), D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device_->CreateInputLayout(shader_varying_desc, 2, 
	                           vertex_shader_code_->GetBufferPointer(), vertex_shader_code_->GetBufferSize(), 
	                           &shader_input_layout_);
	device_context_->IASetInputLayout(shader_input_layout_);

	uint32_t stride = 6u * sizeof(float);
	uint32_t offset = 0u;
	device_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
	device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_RASTERIZER_DESC1 rasterizer_desc{
		.FillMode = D3D11_FILL_SOLID,
		.CullMode = D3D11_CULL_BACK,
	};

	device_->CreateRasterizerState1(&rasterizer_desc, &rasterizer_state_);
	device_context_->RSSetState(rasterizer_state_);
}

D3D11Renderer::~D3D11Renderer() {
	rasterizer_state_->Release();
	vertex_buffer_->Release();
	shader_input_layout_->Release();
	pixel_shader_->Release();
	vertex_shader_->Release();
	pixel_shader_code_->Release();
	vertex_shader_code_->Release();
	depthbuffer_view_->Release();
	depthbuffer_->Release();
	framebuffer_view_->Release();
	framebuffer_->Release();
	swap_chain_->Release();
	dxgi_factory_->Release();
	dxgi_adapter_->Release();
	dxgi_device_->Release();
	device_context_->Release();
	device_->Release();
	base_device_context_->Release();
	base_device_->Release();
}

void D3D11Renderer::render() noexcept {
	const float clear_color[4] = {.025f, .025f, .025f, 1.f};

	device_context_->ClearRenderTargetView(framebuffer_view_, clear_color);
	device_context_->ClearDepthStencilView(depthbuffer_view_, D3D11_CLEAR_DEPTH, 1.f, 0);

	device_context_->Draw(3, 0);

	swap_chain_->Present(1, 0);
}

} // namespace onyx
