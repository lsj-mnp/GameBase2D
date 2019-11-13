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

	m_vVertices.emplace_back(Pos1, DirectX::XMFLOAT2(1.0f, 0.0f));
	m_vVertices.emplace_back(Pos3, DirectX::XMFLOAT2(1.0f, 1.0f));
	m_vVertices.emplace_back(Pos2, DirectX::XMFLOAT2(0.0f, 1.0f));

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.ByteWidth = sizeof(SVertex2D) * 6;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = &m_vVertices[0];

	m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.ReleaseAndGetAddressOf());
}

void CObject2D::CModel2D::Draw()
{
	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);

	m_PtrDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_PtrDeviceContext->Draw(m_vVertices.size(), 0);
}

void CObject2D::Create(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName)
{
	m_Model2D = std::make_unique<CModel2D>(m_PtrDevice, m_PtrDeviceContext);
	m_Model2D->CreateRectangle(Size);

	m_Texture = std::make_unique<CTexture>(m_PtrDevice, m_PtrDeviceContext);
	m_Texture->CreateFromFile(TextureFileName);
	m_Texture->SetShaderType(EShaderType::PixelShader);
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
