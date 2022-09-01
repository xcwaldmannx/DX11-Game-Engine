struct VS_INPUT {
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float4 offsets: POSITION1;
	float4 colors: COLOR0;
};

struct VS_OUTPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float4 colors: COLOR0;
};

cbuffer transform: register(b0) {
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
};

VS_OUTPUT main(VS_INPUT input) {

	VS_OUTPUT output = (VS_OUTPUT)0;

	input.offsets.w = 0.0;

	input.position += input.offsets;

	// world space coordinates
	output.position = mul(input.position, world);

	// view space coordinates
	output.position = mul(output.position, view);

	// screen space coordinates
	output.position = mul(output.position, proj);

	output.texcoord = float2(input.texcoord.x, 1.0 - input.texcoord.y);
	output.normal = normalize(mul(input.normal, world));

	output.colors = input.colors;

	return output;
}