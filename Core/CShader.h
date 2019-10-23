#pragma once

#include "SharedHeader.h"

enum class EShaderType
{
	VertexShader,
	PixelShader
};

class CShader
{
public:
	CShader(ID3D11Device* PtrDevice, ID3D11DeviceContext* PtrDeviceContext) : m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CShader() {}

public:
	void Create(EShaderType eShaderType, const std::wstring& FileName);

	void Use();

private:
	ID3D11Device* m_PtrDevice{};
	ID3D11DeviceContext* m_PtrDeviceContext{};

private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob{};
	Microsoft::WRL::ComPtr<ID3D11DeviceChild> m_Shader{};

	EShaderType m_eShaderType{};
};