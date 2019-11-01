#pragma once

#include "SharedHeader.h"

struct SVertex2D
{
	DirectX::XMFLOAT2 Position{};
};

class CObject2D
{
public:
	class CModel2D
	{
	public:
		CModel2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
			m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
		{
			assert(PtrDevice);
			assert(PtrDeviceContext);
		}
		~CModel2D() {}

		void CreateRectangle();

		void Use();

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		std::vector<SVertex2D> m_vVertices{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};
		UINT m_Stride{ sizeof(SVertex2D) };
		UINT m_Offset{};
	};

public:
	class CTexture
	{
	public:
		CTexture(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
			m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
		{
			assert(PtrDevice);
			assert(PtrDeviceContext);
		}
		~CTexture() {}

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:

	};

public:
	CObject2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CObject2D() {}



private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::unique_ptr<CModel2D> m_Model{};
	std::unique_ptr<CTexture> m_Texture{};
};
