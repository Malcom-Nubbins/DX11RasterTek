cbuffer MatrixBuffer
{
    matrix worldMat;
    matrix viewMat;
    matrix projMat;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

PixelInputType main( VertexInputType input )
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMat);
    output.position = mul(output.position, viewMat);
    output.position = mul(output.position, projMat);

    output.tex = input.tex;

    output.normal = mul(input.normal, (float3x3) worldMat);

    output.normal = normalize(output.normal);

	return output;
}