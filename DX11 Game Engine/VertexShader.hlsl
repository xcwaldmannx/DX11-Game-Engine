
struct VS_INPUT {
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float3 worldPosition: POSITION1;
};

cbuffer transform: register(b0) {
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
};

cbuffer lighting: register(b1) {
	float4 lightingDirection;
	float4 cameraPosition;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

cbuffer pointLight: register(b2) {
	float4 pointlightPosition;
	float4 pointlightCameraPosition;
	float pointlightRadius;
};

VS_OUTPUT main(VS_INPUT input) {

	VS_OUTPUT output = (VS_OUTPUT) 0;

	// world space coordinates
	output.position = mul(input.position, world);
	output.worldPosition = input.position.xyz;

	// view space coordinates
	output.position = mul(output.position, view);

	// screen space coordinates
	output.position = mul(output.position, proj);

	output.texcoord = float2(input.texcoord.x, 1.0 - input.texcoord.y);
	output.normal = normalize(mul(input.normal, world));

	return output;
}