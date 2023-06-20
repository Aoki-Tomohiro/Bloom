#include "Object3d.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gHighLum : register(t1);
Texture2D<float4> gShrinkHighLum : register(t2);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.color =
		gTexture.Sample(gSampler, input.texcoord) + gHighLum.Sample(gSampler, input.texcoord) +
		gShrinkHighLum.Sample(gSampler, input.texcoord);
	return output;
}