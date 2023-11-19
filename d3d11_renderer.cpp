#include "d3d11_renderer.hpp"

#include "d3d11_1.h"

namespace onyx {

D3D11Renderer::D3D11Renderer(HWND window_handle) {
	D3D_FEATURE_LEVEL feature_levels[]{D3D_FEATURE_LEVEL_11_0};

	// @TODO: Check for errors
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
	                  D3D11_CREATE_DEVICE_BGRA_SUPPORT,
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
		.Height = static_cast<float>(depthbuffer_desc.Width),
	};
	device_context_->RSSetViewports(1, &viewport);

	FLOAT clear_color[4] = {.1f, .1f, .1f, 1.f};
	device_context_->ClearRenderTargetView(framebuffer_view_, clear_color);

	device_context_->ClearDepthStencilView(depthbuffer_view_, D3D11_CLEAR_DEPTH, 1.f, 0);
}

D3D11Renderer::~D3D11Renderer() {
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

void D3D11Renderer::render() {
	swap_chain_->Present(1, 0);
}

} // namespace onyx
