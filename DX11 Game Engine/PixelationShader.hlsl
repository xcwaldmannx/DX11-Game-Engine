
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: TEXCOORD1;
	float3 directionToCamera: TEXCOORD2;
};

cbuffer constant: register(b0) {
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
	float4 lightDirection;
	float4 cameraPosition;
};

float4 main(PS_INPUT input) : SV_TARGET{

	// start pixellation

	float3 color = Texture.Sample(TextureSampler, input.texcoord);
	return float4(color, 1.0);

	// end pixellation

	// start phong reflection
	float ambientStrength = 0.1;
	float3 ambientColor = Texture.Sample(TextureSampler, input.texcoord);
	float3 ambientLight = ambientStrength * ambientColor;

	float diffuseStrength = 1.0;
	float3 diffuseColor = Texture.Sample(TextureSampler, input.texcoord);
	float diffuseAmount = max(0.0, dot(lightDirection.xyz, input.normal));
	float3 diffuseLight = diffuseStrength * diffuseAmount * diffuseColor;

	float specularStrength = 1.0;
	float3 specularColor = float3(1.0, 1.0, 1.0);
	float3 reflectedLight = reflect(lightDirection.xyz, input.normal);
	float specularShine = 30.0;
	float specularAmount = pow(max(0.0, dot(reflectedLight, input.directionToCamera)), specularShine);
	float3 specularLight = specularStrength * specularAmount * specularColor;

	float3 finalLight = ambientLight + diffuseLight + specularLight;
	// end phong reflection

	return float4(finalLight, 1.0);
}