#include "Header.hlsli"

cbuffer CBSpace : register(b0)
{
	float4x4 World;
	float4x4 WorldProjection;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//Output.Position = mul(Input.Position, World);
	Output.Position = mul(Input.Position, WorldProjection);
	Output.TexCoord = Input.TexCoord;

	return Output;
}