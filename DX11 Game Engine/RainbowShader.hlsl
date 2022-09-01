struct PS_INPUT {
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;

	float3 worldPosition: POSITION1;
};

float3 Hue(float H) {
	float R = abs(H * 6 - 3) - 1;
	float G = 2 - abs(H * 6 - 2);
	float B = 2 - abs(H * 6 - 4);
	return saturate(float3(R, G, B));
}

float3 HSVtoRGB(in float3 HSV) {
	return ((Hue(HSV.x) - 1) * HSV.y + 1) * HSV.z;
}

float4 main(PS_INPUT input) : SV_TARGET{
	float h = input.worldPosition.y / 20.0;
	float3 hsv = float3(h, 1, 1);
	float3 color = HSVtoRGB(hsv);
	return float4(color, 1.2 - h);
}