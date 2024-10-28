#include "Header.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};
 
VertexOutput main(VertexInput input)
{
    VertexOutput output = (VertexOutput) 0;
    output.position = mul(float4(input.position, 1.0f), World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.color = input.color; 
    return output;
}
 
float4 PS(VertexOutput input) : SV_Target
{
    return input.color; 
}