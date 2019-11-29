#include "CObject2D.h"

void CObject2D::CModel2D::CreateRectangle(const DirectX::XMFLOAT2& Size)
{
	m_Size = Size;

	float HalfSizeX{ m_Size.x * 0.5f };
	float HalfSizeY{ m_Size.y * 0.5f };

	DirectX::XMFLOAT2 Pos0{ -HalfSizeX, +HalfSizeY };
	DirectX::XMFLOAT2 Pos1{ +HalfSizeX, +HalfSizeY };
	DirectX::XMFLOAT2 Pos2{ -HalfSizeX, -HalfSizeY };
	DirectX::XMFLOAT2 Pos3{ +HalfSizeX, -HalfSizeY };

	m_vVertices.emplace_back(Pos0, DirectX::XMFLOAT2(0.0f, 0.0f));
	m_vVertices.emplace_back(Pos1, DirectX::XMFLOAT2(1.0f, 0.0f));
	m_vVertices.emplace_back(Pos2, DirectX::XMFLOAT2(0.0f, 1.0f));
	m_vVertices.emplace_back(Pos3, DirectX::XMFLOAT2(1.0f, 1.0f));

	m_vTriangles.emplace_back(0, 1, 2);
	m_vTriangles.emplace_back(1, 3, 2);
	
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(SVertex2D) * 6;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_vVertices[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.ReleaseAndGetAddressOf());
	}

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		buffer_desc.ByteWidth = sizeof(STriangle) * m_vTriangles.size();
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_vTriangles[0];

		m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_IndexBuffer.ReleaseAndGetAddressOf());
	}
}

void CObject2D::CModel2D::UpdateRectangleTexCoord(const DirectX::XMFLOAT2& UVOffset, const DirectX::XMFLOAT2& UVSize)
{
	m_vVertices[0].TexCoord = UVOffset;
	m_vVertices[1].TexCoord = DirectX::XMFLOAT2(UVOffset.x + UVSize.x	, UVOffset.y);
	m_vVertices[2].TexCoord = DirectX::XMFLOAT2(UVOffset.x				, UVOffset.y + UVSize.y);
	m_vVertices[3].TexCoord = DirectX::XMFLOAT2(UVOffset.x + UVSize.x	, UVOffset.y + UVSize.y);

	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	if (SUCCEEDED(m_PtrDeviceContext->Map(m_VertexBuffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource)))
	{ 
		memcpy(mapped_subresource.pData, &m_vVertices[0], sizeof(SVertex2D) * m_vVertices.size());

		m_PtrDeviceContext->Unmap(m_VertexBuffer.Get(), 0);
	}
}

void CObject2D::CModel2D::Draw()
{
	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VBStride, &m_VBOffset);

	m_PtrDeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_PtrDeviceContext->DrawIndexed(m_vTriangles.size() * 3, 0, 0);
}

const DirectX::XMFLOAT2& CObject2D::CModel2D::GetSize() const
{
	return m_Size;
}

void CObject2D::Create(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName)
{
	m_Model2D = std::make_unique<CModel2D>(m_PtrDevice, m_PtrDeviceContext);
	m_Model2D->CreateRectangle(Size);

	m_Texture = std::make_unique<CTexture>(m_PtrDevice, m_PtrDeviceContext);
	m_Texture->CreateFromFile(TextureFileName);
	m_Texture->SetShaderType(EShaderType::PixelShader);
}

void CObject2D::CreateAsTextureSize(const std::string& TextureFileName)
{
	m_Texture = std::make_unique<CTexture>(m_PtrDevice, m_PtrDeviceContext);
	m_Texture->CreateFromFile(TextureFileName);
	m_Texture->SetShaderType(EShaderType::PixelShader);
	D3D11_TEXTURE2D_DESC Texture2DDesc{ m_Texture->GetTextureDesc() };

	m_Model2D = std::make_unique<CModel2D>(m_PtrDevice, m_PtrDeviceContext);
	m_Model2D->CreateRectangle(DirectX::XMFLOAT2((float)Texture2DDesc.Width, (float)Texture2DDesc.Height));
}

void CObject2D::SetVisibleArea(const DirectX::XMFLOAT2& PixelSpaceOffset)
{
	D3D11_TEXTURE2D_DESC Texture2DDesc{ m_Texture->GetTextureDesc() };

	DirectX::XMFLOAT2 PixelSpaceTotalSize{ (float)Texture2DDesc.Width, (float)Texture2DDesc.Height };
	DirectX::XMFLOAT2 PixelSpaceSize{ m_Model2D->GetSize() };

	DirectX::XMFLOAT2 UVOffset{ PixelSpaceOffset.x / PixelSpaceTotalSize.x , PixelSpaceOffset.y / PixelSpaceTotalSize.y };
	DirectX::XMFLOAT2 UVSize{ PixelSpaceSize.x / PixelSpaceTotalSize.x, PixelSpaceSize.y / PixelSpaceTotalSize.y };

	m_Model2D->UpdateRectangleTexCoord(UVOffset, UVSize);
}

void CObject2D::Draw()
{
	m_Texture->Use();

	m_Model2D->Draw();
}

void CObject2D::Translate(const DirectX::XMVECTOR& Translation)
{
	using namespace DirectX;

	m_ComponentTransform.Translation += Translation;

	UpdateWorldMatrix();
}

void CObject2D::Rotate(const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_ComponentTransform.Rotation += Rotation;

	UpdateWorldMatrix();
}

void CObject2D::Scale(const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_ComponentTransform.Scaling += Scaling;

	UpdateWorldMatrix();
}

void CObject2D::TranslateTo(const DirectX::XMVECTOR& Translation)
{
	using namespace DirectX;

	m_ComponentTransform.Translation = Translation;

	UpdateWorldMatrix();
}

void CObject2D::RotateTo(const DirectX::XMVECTOR& Rotation)
{
	using namespace DirectX;

	m_ComponentTransform.Rotation = Rotation;

	UpdateWorldMatrix();
}

void CObject2D::ScaleTo(const DirectX::XMVECTOR& Scaling)
{
	using namespace DirectX;

	m_ComponentTransform.Scaling = Scaling;

	UpdateWorldMatrix();
}

const DirectX::XMMATRIX& CObject2D::GetWorldMatrix() const
{
	return m_ComponentTransform.WorldMatrix;
}

void CObject2D::UpdateWorldMatrix()
{
	DirectX::XMMATRIX Translation{ DirectX::XMMatrixTranslationFromVector(m_ComponentTransform.Translation) };
	DirectX::XMMATRIX Rotation{ DirectX::XMMatrixRotationRollPitchYawFromVector(m_ComponentTransform.Rotation) };
	DirectX::XMMATRIX Scaling{ DirectX::XMMatrixScalingFromVector(m_ComponentTransform.Scaling) };

	m_ComponentTransform.WorldMatrix = Scaling * Rotation * Translation;
}

void CObject2D::CTexture::CreateFromFile(const std::string& FileName)
{
	std::wstring wFileName{ FileName.begin(), FileName.end() };

	DirectX::CreateWICTextureFromFile(m_PtrDevice, wFileName.c_str(), 
		(ID3D11Resource**)m_Texture2D.ReleaseAndGetAddressOf(), m_ShaderResourceView.ReleaseAndGetAddressOf());
	
	m_Texture2D->GetDesc(&m_Texture2DDesc);
}

void CObject2D::CTexture::SetShaderType(EShaderType eShaderType)
{
	m_eShaderType = eShaderType;
}

void CObject2D::CTexture::SetSlot(UINT Slot)
{
	m_Slot = Slot;
}

void CObject2D::CTexture::Use()
{
	switch (m_eShaderType)
	{
	case EShaderType::VertexShader:
		m_PtrDeviceContext->VSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
		break;
	case EShaderType::PixelShader:
		m_PtrDeviceContext->PSSetShaderResources(m_Slot, 1, m_ShaderResourceView.GetAddressOf());
		break;
	default:
		break;
	}
}

const D3D11_TEXTURE2D_DESC& CObject2D::CTexture::GetTextureDesc() const
{
	return m_Texture2DDesc;
}
