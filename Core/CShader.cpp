#include "CShader.h"

void CShader::Create(EShaderType eShaderType, const std::wstring& FileName)
{
	m_eShaderType = eShaderType;

	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.ReleaseAndGetAddressOf(), nullptr);

		m_PtrDevice->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr,
			(ID3D11VertexShader**)m_Shader.ReleaseAndGetAddressOf());

		m_PtrDevice->CreateInputLayout(KInputElementDesc, ARRAYSIZE(KInputElementDesc), m_Blob->GetBufferPointer(),
			m_Blob->GetBufferSize(), m_InputLayout.ReleaseAndGetAddressOf());
		break;
	case EShaderType::PixelShader:
		D3DCompileFromFile(FileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_4_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, m_Blob.ReleaseAndGetAddressOf(), nullptr);

		m_PtrDevice->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, 
			(ID3D11PixelShader**)m_Shader.ReleaseAndGetAddressOf());
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

		m_PtrDeviceContext->IASetInputLayout(m_InputLayout.Get());

		for (auto& ConstantBuffer : m_vConstantBuffers)
		{
			ConstantBuffer->Use();
		}
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetShader((ID3D11PixelShader*)m_Shader.Get(), nullptr, 0);

		for (auto& ConstantBuffer : m_vConstantBuffers)
		{
			ConstantBuffer->Use();
		}
		break;
	default:
		break;
	}
}

void CShader::AddConstantBuffer(UINT ByteWidth, const void* const pData)
{
	m_vConstantBuffers.emplace_back(std::make_unique<CConstantBuffer>(m_PtrDevice, m_PtrDeviceContext));

	m_vConstantBuffers.back()->Create(m_eShaderType, ByteWidth, pData);
}

void CShader::UpdateConstantBuffers()
{
	for (auto& ConstantBuffer : m_vConstantBuffers)
	{
		ConstantBuffer->Update();
	}
}


void CShader::CConstantBuffer::Create(EShaderType eShaderType, UINT ByteWidth, const void* const pData)
{
	m_eShadertype = eShaderType;

	m_BufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	m_BufferDesc.ByteWidth = ByteWidth;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;
	m_BufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_SubresourceData.pSysMem = pData;

	m_PtrDevice->CreateBuffer(&m_BufferDesc, &m_SubresourceData, m_ConstantBuffer.ReleaseAndGetAddressOf());
}

void CShader::CConstantBuffer::Update()
{
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};

	if (SUCCEEDED(m_PtrDeviceContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{
		memcpy(mapped_subresource.pData, m_SubresourceData.pSysMem, m_BufferDesc.ByteWidth);

		m_PtrDeviceContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
}

void CShader::CConstantBuffer::Use()
{
	switch (m_eShadertype)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}
