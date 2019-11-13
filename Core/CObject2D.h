#pragma once

#include "SharedHeader.h"

struct SVertex2D
{
	SVertex2D() {}
	SVertex2D(DirectX::XMFLOAT2 _Position, DirectX::XMFLOAT2 _TexCoord) : Position{ _Position }, TexCoord{ _TexCoord }{}

	DirectX::XMFLOAT2 Position{};
	DirectX::XMFLOAT2 TexCoord{};
};

class CObject2D
{
public:
	struct SComponentTransform
	{
		DirectX::XMMATRIX WorldMatrix{};
		DirectX::XMVECTOR Translation{};
		DirectX::XMVECTOR Rotation{};
		DirectX::XMVECTOR Scaling{};
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

		void CreateRectangle(const DirectX::XMFLOAT2& Size);

		void Draw();

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		std::vector<SVertex2D> m_vVertices{};
		DirectX::XMFLOAT2 m_Size{};

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

	public:
		void CreateFromFile(const std::string& FileName);

		void SetShaderType(EShaderType eShaderType);

		void SetSlot(UINT Slot);

		void Use();

	private:
		ID3D11Device* const m_PtrDevice{};
		ID3D11DeviceContext* const m_PtrDeviceContext{};

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D{};
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView{};

	private:
		EShaderType m_eShaderType{};
		UINT m_Slot{};
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
	void Create(const DirectX::XMFLOAT2& Size, const std::string& TextureFileName);

	void Draw();

public:
	void Translate(const DirectX::XMVECTOR& Translation);

	void Rotate(const DirectX::XMVECTOR& Rotation);

	void Scale(const DirectX::XMVECTOR& Scaling);

	void TranslateTo(const DirectX::XMVECTOR& Translation);

	void RotateTo(const DirectX::XMVECTOR& Rotation);

	void ScaleTo(const DirectX::XMVECTOR& Scaling);

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
