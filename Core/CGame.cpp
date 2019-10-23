#include "CGame.h"

using std::string;

void CGame::Create(HINSTANCE hInstance, WNDPROC WndProc, const string& WindowName)
{
	m_hInstance = hInstance;
	m_WndProc = WndProc;

	CreateWin32Window(WindowName);

	InitializeDirectX();
}

void CGame::BeginRendering(const float* ColorRGBA)
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);
}

void CGame::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

void CGame::CreateWin32Window(const string& WindowName)
{
	WNDCLASSEX wnd_cls{};
	wnd_cls.cbClsExtra = 0;
	wnd_cls.cbSize = sizeof(WNDCLASSEX);
	wnd_cls.cbWndExtra = 0;
	wnd_cls.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wnd_cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wnd_cls.hIcon = wnd_cls.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wnd_cls.hInstance = m_hInstance;
	wnd_cls.lpfnWndProc = m_WndProc;
	wnd_cls.lpszClassName = "Base";
	wnd_cls.lpszMenuName = nullptr;
	wnd_cls.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wnd_cls);

	m_hWnd = CreateWindowEx(0, "Base", WindowName.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		static_cast<int>(m_Width), static_cast<int>(m_Height), nullptr, nullptr, m_hInstance, nullptr);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);
}

void CGame::InitializeDirectX()
{
	CreateSwapChain();

	CreateAndSetRenderTargetView();
}

void CGame::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.Height = (UINT)m_Height;
	swap_chain_desc.BufferDesc.Width = (UINT)m_Width;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.Flags = 0;
	swap_chain_desc.OutputWindow = m_hWnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
		m_SwapChain.GetAddressOf(), m_Device.GetAddressOf(), nullptr, m_DeviceContext.GetAddressOf());
}

void CGame::CreateAndSetRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer{};

	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.GetAddressOf());

	m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.GetAddressOf());

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

