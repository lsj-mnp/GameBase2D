#pragma once

#include "SharedHeader.h"
#include "CShader.h"
#include "CObject2D.h"

class CGame
{
public:
	struct SCBVSSpace
	{
		DirectX::XMMATRIX ProjectionMatrix{};
	};

public:
	CGame(float Width, float Height) : m_Width{ Width }, m_Height{ Height }{}
	~CGame() {}

public:
	void Create(HINSTANCE hInstance, WNDPROC WndProc, const std::string& WindowName);

public:
	void AddObject2D(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName);

public:
	void BeginRendering(const float* ColorRGBA);

	void Draw();

	void EndRendering();

private:
	void CreateWin32Window(const std::string& WindowName);

	void InitializeDirectX();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

	void CreateViewports();

	void SetViewport();

	void CreateSamplers();

	void CreateBlendStates();

private:
	HINSTANCE m_hInstance{};
	HWND m_hWnd{};
	WNDPROC m_WndProc{};

	float m_Width{};
	float m_Height{};

private:
	std::unique_ptr<CShader> m_VertexShader{};
	std::unique_ptr<CShader> m_PixelShader{};

private:
	std::vector<std::unique_ptr<CObject2D>> m_vObject2Ds{};

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain{};
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};

	D3D11_VIEWPORT m_Viewport{};

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerStateLinearWrap{};
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAlpha{};

private:
	DirectX::XMMATRIX m_ProjectionMatrix{};
	SCBVSSpace m_CBVSSpace{};
};