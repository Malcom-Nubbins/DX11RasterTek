Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
    float4 diffuseColour;
    float3 lightDirection;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 textureColour;
    float3 lightDir;
    float lightIntensity;
    float4 colour;

    //input.normal = normalize(input.normal);

    textureColour = shaderTexture.Sample(SampleType, input.tex);

    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    colour = saturate(diffuseColour * lightIntensity);

    colour = colour * textureColour;

	return colour;
}