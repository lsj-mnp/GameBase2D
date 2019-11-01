#include "CObject2D.h"

void CObject2D::CModel2D::CreateRectangle()
{
	DirectX::XMFLOAT2 Pos0{ -1.0f, +1.0f };
	DirectX::XMFLOAT2 Pos1{ +1.0f, +1.0f };
	DirectX::XMFLOAT2 Pos2{ -1.0f, -1.0f };
	DirectX::XMFLOAT2 Pos3{ +1.0f, -1.0f };

	m_vVertices.emplace_back(Pos0);
	m_vVertices.emplace_back(Pos1);
	m_vVertices.emplace_back(Pos2);

	m_vVertices.emplace_back(Pos1);
	m_vVertices.emplace_back(Pos3);
	m_vVertices.emplace_back(Pos2);

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.ByteWidth = sizeof(SVertex2D) * 6;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = &m_vVertices[0];

	m_PtrDevice->CreateBuffer(&buffer_desc, &subresource_data, m_VertexBuffer.GetAddressOf());
}

void CObject2D::CModel2D::Use()
{
	m_PtrDeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
}
