#pragma once

#include "SharedHeader.h"

struct SVertex2D
{
	SVertex2D() {}
	SVertex2D(DirectX::XMFLOAT2 _Position, DirectX::XMFLOAT2 _TexCoord) : Position{ _Position }, TexCoord{ _TexCoord }{}

	DirectX::XMFLOAT2 Position{};
	DirectX::XMFLOAT2 TexCoord{};
};

struct STriangle
{
	STriangle() {}
	STriangle(UINT _0, UINT _1, UINT _2) : Index{ _0, _1, _2 } {}

	UINT Index[3]{};
};

class CObject2D
{
public:
	struct SComponentTransform
	{
		DirectX::XMMATRIX WorldMatrix{ DirectX::XMMatrixIdentity() };
		DirectX::XMVECTOR Translation{};
		DirectX::XMVECTOR Rotation{};
		DirectX::XMVECTOR Scaling{ 1, 1, 1, 0 };
	};

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

	public:
		void CreateRectangle(const DirectX::XMFLOAT2& Size);

		void UpdateRectangleTexCoord(const DirectX::XMFLOAT2& UVOffset, const DirectX::XMFLOAT2& UVSize);

		void Draw();

	public:
		const DirectX::XMFLOAT2& GetSize() const;

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		std::vector<SVertex2D> m_vVertices{};
		std::vector<STriangle> m_vTriangles{};
		DirectX::XMFLOAT2 m_Size{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer{};
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer{};
		UINT m_VBStride{ sizeof(SVertex2D) };
		UINT m_VBOffset{};
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

	public:
		void CreateFromFile(const std::string& FileName);

		void SetShaderType(EShaderType eShaderType);

		void SetSlot(UINT Slot);

		void Use();

		const D3D11_TEXTURE2D_DESC& GetTextureDesc() const;

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView{};

	private:
		EShaderType m_eShaderType{};
		UINT m_Slot{};
		D3D11_TEXTURE2D_DESC m_Texture2DDesc{};
	};

public:
	CObject2D(ID3D11Device* const PtrDevice, ID3D11DeviceContext* const PtrDeviceContext) :
		m_PtrDevice{ PtrDevice }, m_PtrDeviceContext{ PtrDeviceContext }
	{
		assert(PtrDevice);
		assert(PtrDeviceContext);
	}
	~CObject2D() {}

public:
	void* operator new(size_t ByteSize)
	{
		return _aligned_malloc(ByteSize, 16);
	}

	void operator delete(void* Ptr)
	{
		_aligned_free(Ptr);
	}

public:
	void Create(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName);

	void CreateAsTextureSize(const std::string& TextureFileName);

	void SetVisibleArea(const DirectX::XMFLOAT2& PixelSpaceOffset);

	void Draw();

public:
	void Translate(const DirectX::XMVECTOR& Translation);

	void Rotate(const DirectX::XMVECTOR& Rotation);

	void Scale(const DirectX::XMVECTOR& Scaling);

	void TranslateTo(const DirectX::XMVECTOR& Translation);

	void RotateTo(const DirectX::XMVECTOR& Rotation);

	void ScaleTo(const DirectX::XMVECTOR& Scaling);

	const DirectX::XMMATRIX& GetWorldMatrix() const;

private:
	void UpdateWorldMatrix();

private:
	ID3D11Device* const m_PtrDevice{};
	ID3D11DeviceContext* const m_PtrDeviceContext{};

private:
	std::unique_ptr<CModel2D> m_Model2D{};
	std::unique_ptr<CTexture> m_Texture{};
	SComponentTransform m_ComponentTransform{};
};
