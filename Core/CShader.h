#pragma once

#include "SharedHeader.h"

static constexpr D3D11_INPUT_ELEMENT_DESC KInputElementDesc[]
{
	{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}, 
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

class CShader
{
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

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob{};
	Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_Shader{};
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout{};

	EShaderType m_eShaderType{};
};