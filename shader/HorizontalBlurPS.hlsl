#include "Object3d.hlsli"

struct PixelShaderOutput {
	float4 highLum : SV_TARGET0;//高輝度
};

struct Weights {
	float4 bkWeights[2];
};

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<Weights> gWeights : register(b0);

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.highLum = float4(0, 0, 0, 0);
	float4 color = gTexture.Sample(gSampler, input.texcoord);
	float w, h, level;
	gTexture.GetDimensions(0, w, h, level);
	//縮小バッファのサイズに合わせる
	w /= 8;
	h /= 8;
	float dx = 1.0f / w;
	float dy = 1.0f / h;

	output.highLum += gWeights.bkWeights[0] * color;
	output.highLum += gWeights.bkWeights[0][1] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 1, 0));
	output.highLum += gWeights.bkWeights[0][1] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 1, 0));
	output.highLum += gWeights.bkWeights[0][2] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 2, 0));
	output.highLum += gWeights.bkWeights[0][2] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 2, 0));
	output.highLum += gWeights.bkWeights[0][3] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 3, 0));
	output.highLum += gWeights.bkWeights[0][3] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 3, 0));
	output.highLum += gWeights.bkWeights[1][0] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 4, 0));
	output.highLum += gWeights.bkWeights[1][0] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 4, 0));
	output.highLum += gWeights.bkWeights[1][1] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 5, 0));
	output.highLum += gWeights.bkWeights[1][1] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 5, 0));
	output.highLum += gWeights.bkWeights[1][2] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 6, 0));
	output.highLum += gWeights.bkWeights[1][2] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 6, 0));
	output.highLum += gWeights.bkWeights[1][3] * gTexture.Sample(gSampler, input.texcoord + float2(dx * 7, 0));
	output.highLum += gWeights.bkWeights[1][3] * gTexture.Sample(gSampler, input.texcoord + float2(-dx * 7, 0));
	output.highLum.a = color.a;

	return output;
}