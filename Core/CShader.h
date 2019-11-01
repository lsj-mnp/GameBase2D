#pragma once

#include "SharedHeader.h"

enum class EShaderType
{
	VertexShader,
	PixelShader
};

static constexpr D3D11_INPUT_ELEMENT_DESC KInputElementDesc[]
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class CShader
{
	class CConstantBuffer
	{
	public:
		CConstantBuffer(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
			m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
		{
			assert(PtrDevice);
			assert(PtrDeviceContext);
		}
		~CConstantBuffer() {}

	public:
		void Create(EShaderType eShaderType, UINT ByteWidth, const void* const pData);

		void Update();

		void Use();
	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer{};
		D3D11_SUBRESOURCE_DATA m_SubresourceData{};
		D3D11_BUFFER_DESC m_BufferDesc{};
		EShaderType m_eShadertype{};
	};
public:
	CShader(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CShader() {}

public:
	void Create(EShaderType eShaderType, const std::wstring& FileName);

	void Use();

public:
	void AddConstantBuffer(UINT ByteWidth, const void* const pData);

	void UpdateConstantBuffers();

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob{};
	Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_Shader{};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout{};

	std::vector<std::unique_ptr<CConstantBuffer>> m_vConstantBuffers{};

	EShaderType m_eShaderType{};
};