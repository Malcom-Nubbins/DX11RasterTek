cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInputType
{
	float4 pos : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType ColorVertexShader( VertexInputType input )
{
	PixelInputType output;

	input.pos.w = 1.0f;

    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);

    output.color = input.color;
	return output;
}