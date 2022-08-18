
Texture2D textures[1]: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 directionToCamera: TEXCOORD2;
};

float4 main(PS_INPUT input) : SV_TARGET {
	return textures[0].Sample(TextureSampler, input.texcoord);
}