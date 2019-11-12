#include "Header.hlsli"

cbuffer CBSpace : register(b0)
{
	float4x4 Projection;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = mul(Input.Position, Projection);
	Output.TexCoord = Input.TexCoord;

	return Output;
}