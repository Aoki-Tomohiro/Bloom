#include "Object3d.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
	float32_t4 highLum : SV_TARGET1;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.color = gTexture.Sample(gSampler, input.texcoord);
	float y = output.color.r * 0.299 + output.color.g * 0.587 + output.color.b * 0.114;
	if (y > 0.99f) {
		output.highLum = y;
	}
	else {
		output.highLum = 0.0f;
	}
	return output;
}