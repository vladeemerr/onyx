#pragma once

#include "renderer.hpp"

typedef struct HWND__ *HWND;

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Device1;
struct ID3D11DeviceContext1;
struct IDXGIDevice1;
struct IDXGIAdapter;
struct IDXGIFactory2;
struct IDXGISwapChain1;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D10Blob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;
struct ID3D11RasterizerState1;

namespace onyx {

class D3D11Renderer final : public Renderer {
private:
	ID3D11Device *base_device_;
	ID3D11DeviceContext *base_device_context_;

	ID3D11Device1 *device_;
	ID3D11DeviceContext1 *device_context_;

	IDXGIDevice1 *dxgi_device_;
	IDXGIAdapter *dxgi_adapter_;
	IDXGIFactory2 *dxgi_factory_;

	IDXGISwapChain1 *swap_chain_;
	ID3D11Texture2D *framebuffer_;
	ID3D11RenderTargetView *framebuffer_view_;

	ID3D11Texture2D *depthbuffer_;
	ID3D11DepthStencilView *depthbuffer_view_;

	ID3D10Blob *vertex_shader_code_;
	ID3D10Blob *pixel_shader_code_;

	ID3D11VertexShader *vertex_shader_;
	ID3D11PixelShader *pixel_shader_;
	ID3D11InputLayout *shader_input_layout_;

	ID3D11Buffer *vertex_buffer_;

	ID3D11RasterizerState1 *rasterizer_state_;

public:
	D3D11Renderer(const HWND window_handle);
	~D3D11Renderer();

	void render() noexcept override;
};

} // namespace onyx
