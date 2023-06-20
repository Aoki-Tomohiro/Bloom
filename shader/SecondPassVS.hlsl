#include "Object3d.hlsli"

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = input.position;
	output.texcoord = input.texcoord;
	return output;
}