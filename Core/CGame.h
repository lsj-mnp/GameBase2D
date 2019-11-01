#pragma once

#include "SharedHeader.h"
#include "CShader.h"

class CGame
{
public:
	CGame(float Width, float Height) : m_Width{ Width }, m_Height{ Height }{}
	~CGame() {}

public:
	void Create(HINSTANCE hInstance, WNDPROC WndProc, const std::string& WindowName);

	void BeginRendering(const float* ColorRGBA);

	void EndRendering();


private:
	void CreateWin32Window(const std::string& WindowName);

	void InitializeDirectX();

private:
	void CreateSwapChain();

	void CreateAndSetRenderTargetView();

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
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain{};
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device{};
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext{};
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView{};
};