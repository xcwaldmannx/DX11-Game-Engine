
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT {
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
	float4 lightDirection;
	float4 cameraPosition;
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};

/*
cbuffer pointLight: register(b2) {
	float4 pointlightPosition;
	float4 pointlightDirection;
	float4 pointlightCameraPosition;
	float pointlightRadius;
	float pointlightAmbient;
	float pointlightDiffuse;
	float pointlightSpecular;
	float pointlightShininess;
};
*/

float4 main(PS_INPUT input) : SV_TARGET {

	float4 textureColor = Texture.Sample(TextureSampler, input.texcoord);
	clip(textureColor.a < 0.01 ? -1 : 1);

	float3 directionToCamera = normalize(input.worldPosition.xyz - cameraPosition.xyz);

	// start phong reflection
	float ambientStrength = ambient;
	float3 ambientColor = textureColor;
	float3 ambientLight = ambientStrength * ambientColor;

	float diffuseStrength = diffuse;
	float3 diffuseColor = textureColor;
	float diffuseAmount = max(0.0, dot(lightDirection.xyz, input.normal));
	float3 diffuseLight = diffuseStrength * diffuseAmount * diffuseColor;

	float specularStrength = specular;
	float3 specularColor = float3(1.0, 1.0, 1.0);
	float3 reflectedLight = reflect(lightDirection.xyz, input.normal);
	float specularShine = max(1.0, shininess);
	float specularAmount = pow(max(0.0, dot(reflectedLight, directionToCamera)), specularShine);
	float3 specularLight = specularStrength * specularAmount * specularColor;

	float3 finalLight = ambientLight + diffuseLight + specularLight;
	// end phong reflection

	return float4(finalLight, textureColor.a);
}