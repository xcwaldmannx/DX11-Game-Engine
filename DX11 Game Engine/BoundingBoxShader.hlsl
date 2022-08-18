struct PS_INPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float3 worldPosition: POSITION1;
};

float4 main(PS_INPUT input) : SV_TARGET {
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}