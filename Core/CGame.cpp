#include "CGame.h"

using std::string;

void CGame::Create(HINSTANCE hInstance, WNDPROC WndProc, const string& WindowName)
{
	m_hInstance = hInstance;
	m_WndProc = WndProc;

	CreateWin32Window(WindowName);

	InitializeDirectX();

	m_VertexShader = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_VertexShader->Create(EShaderType::VertexShader, L"Shader/VertexShader.hlsl");
	m_VertexShader->AddConstantBuffer(sizeof(SCBVSSpace), &m_CBVSSpace);
	m_CBVSSpace.ProjectionMatrix = m_ProjectionMatrix;
	m_VertexShader->UpdateConstantBuffers();

	m_PixelShader = std::make_unique<CShader>(m_Device.Get(), m_DeviceContext.Get());
	m_PixelShader->Create(EShaderType::PixelShader, L"Shader/PixelShader.hlsl");
}

void CGame::AddObject2D(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName)
{
	m_vObject2Ds.emplace_back(std::make_unique<CObject2D>(m_Device.Get(), m_DeviceContext.Get()));

	m_vObject2Ds.back()->Create(Size, TextureFileName);
}

void CGame::BeginRendering(const float* ColorRGBA)
{
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), ColorRGBA);
}

void CGame::Draw()
{
	m_VertexShader->Use();
	m_PixelShader->Use();

	m_DeviceContext->PSSetSamplers(0, 1, m_SamplerStateLinearWrap.GetAddressOf());

	m_DeviceContext->OMSetBlendState(m_BlendStateAlpha.Get(), nullptr, 0xFFFFFFFF);

	SetViewport();

	for (auto& Object2D : m_vObject2Ds)
	{
		Object2D->Draw();
	}
}

void CGame::EndRendering()
{
	m_SwapChain->Present(0, 0);
}

void CGame::CreateWin32Window(const std::string& WindowName)
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
	m_ProjectionMatrix = DirectX::XMMatrixOrthographicLH(m_Width, m_Height, 0.0f, 1.0f);

	CreateSwapChain();

	CreateAndSetRenderTargetView();

	CreateViewports();

	CreateSamplers();

	CreateBlendStates();
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
		m_SwapChain.ReleaseAndGetAddressOf(), m_Device.ReleaseAndGetAddressOf(), nullptr, m_DeviceContext.ReleaseAndGetAddressOf());
}

void CGame::CreateAndSetRenderTargetView()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer{};

	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)back_buffer.ReleaseAndGetAddressOf());

	m_Device->CreateRenderTargetView(back_buffer.Get(), nullptr, m_RenderTargetView.ReleaseAndGetAddressOf());

	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
}

void CGame::CreateViewports()
{
	m_Viewport.Height = m_Height;
	m_Viewport.Width = m_Width;
	m_Viewport.MaxDepth = 1.0f;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.TopLeftX = 0.0f;
	m_Viewport.TopLeftY = 0.0f;
}

void CGame::SetViewport()
{
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

void CGame::CreateSamplers()
{
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.AddressU = sampler_desc.AddressV = sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.BorderColor[0] = 0.0f;
	sampler_desc.BorderColor[1] = 0.0f;
	sampler_desc.BorderColor[2] = 0.0f;
	sampler_desc.BorderColor[3] = 1.0f;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	sampler_desc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.MaxAnisotropy = 1;
	sampler_desc.MaxLOD = 0.0f;
	sampler_desc.MinLOD = D3D11_FLOAT32_MAX;
	sampler_desc.MipLODBias = 0.0f;

	m_Device->CreateSamplerState(&sampler_desc, m_SamplerStateLinearWrap.ReleaseAndGetAddressOf());
}

void CGame::CreateBlendStates()
{
	D3D11_BLEND_DESC blend_desc{};
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].BlendOp = blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;
	
	m_Device->CreateBlendState(&blend_desc, m_BlendStateAlpha.ReleaseAndGetAddressOf());
}