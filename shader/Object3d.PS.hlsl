#include "Object3d.hlsli"

struct Material {
	float32_t4 color;
};

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	PixelShaderOutput output;
	output.color = float32_t4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}