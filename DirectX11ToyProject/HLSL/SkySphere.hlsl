#include "Header.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};
 
struct GBufferOutput
{
    float4 pos : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 diffuse : SV_TARGET2;
    float4 viewDir : SV_TARGET3;
};
 
PixelInput main(VertexInput input)
{
    PixelInput output = (PixelInput) 0;
    output.position = mul(float4(input.position, 1.0f), World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    output.texcoord = input.texcoord;
    
    return output;
}
 
GBufferOutput PS(PixelInput input)
{
    GBufferOutput output = (GBufferOutput) 0;
    output.diffuse = DiffuseMap.Sample(Sampler, input.texcoord);
    output.normal = float4(-kLightDirection, 1.0f);
     
    return output;
}