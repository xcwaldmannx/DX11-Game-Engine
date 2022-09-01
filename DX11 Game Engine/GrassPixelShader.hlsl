Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float3 worldPosition: POSITION1;
};


float4 main(PS_INPUT input) : SV_TARGET{
	float4 color = Texture.Sample(TextureSampler, input.texcoord);
	return color;
}