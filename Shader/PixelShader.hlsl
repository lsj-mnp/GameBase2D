#include "Header.hlsli"

SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

float4 main(VS_OUTPUT Input) : SV_TARGET
{
	return Texture.Sample(Sampler, Input.TexCoord);
}