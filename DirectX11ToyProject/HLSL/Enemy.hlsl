#include "Header.hlsli"

struct VertexInput
{
    float3 positionL : POSITION;
    float2 texcoord : TEXCOORD;
    float4 instanceWorld0 : INSTANCEWORLD0;
    float4 instanceWorld1 : INSTANCEWORLD1;
    float4 instanceWorld2 : INSTANCEWORLD2;
    float4 instanceWorld3 : INSTANCEWORLD3;
    float3 instanceColor : INSTANCECOLOR;
};
 
struct PixelInput
{
    float4 positionH : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR0;
}; 

PixelInput main(VertexInput input)
{
    PixelInput output;
     
    float4x4 worldMatrix = float4x4(
        input.instanceWorld0,
        input.instanceWorld1,
        input.instanceWorld2,
        input.instanceWorld3
    );
    
    output.positionH = mul(float4(input.positionL, 1.0f), worldMatrix);
    output.positionH = mul(output.positionH, View);
    output.positionH = mul(output.positionH, Projection);
    
    output.texcoord = input.texcoord;
    output.color = input.instanceColor;
    
    return output;
}

float4 PS(PixelInput input) : SV_Target
{ 
    float4 alpha = DiffuseMap.Sample(Sampler, input.texcoord);
     
    
    return float4(input.color, alpha.a);
}