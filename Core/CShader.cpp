#include "CShader.h"

void CShader::Create(EShaderType eShaderType, const std::wstring& FileName)
{
	m_eShaderType = eShaderType;

	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.GetAddressOf(), nullptr);

		m_PtrDevice->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, (ID3D11VertexShader**)m_Shader.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.GetAddressOf(), nullptr);

		m_PtrDevice->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, (ID3D11PixelShader**)m_Shader.GetAddressOf());
		break;
	default:
		break;
	}
}

void CShader::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetShader((ID3D11VertexShader*)m_Shader.Get(), nullptr, 0);
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetShader((ID3D11PixelShader*)m_Shader.Get(), nullptr, 0);
		break;
	default:
		break;
	}
}
