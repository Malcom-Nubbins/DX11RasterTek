struct PixelInputType
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

float4 ColorPixelShader(PixelInputType input) : SV_TARGET
{
	return input.color;
}