#include "Header.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

PixelInput main(uint vertexID : SV_VertexID)
{
    PixelInput output;
      
    float2 pos[6] =
    {
        float2(-1, 1), float2(1, 1), float2(-1, -1),  
        float2(1, 1), float2(1, -1), float2(-1, -1)  
    };

    float2 uv[6] =
    {
        float2(0, 0), float2(1, 0), float2(0, 1),  
        float2(1, 0), float2(1, 1), float2(0, 1)  
    };


    // ���� ID�� ���� ��ġ�� �ؽ�ó ��ǥ�� ����
    output.position = float4(pos[vertexID], 0.0f, 1.0f);
    output.texcoord = uv[vertexID]; 

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    // G-���ۿ��� ���� ���ø�
   float3 position = GBufferPosition.Sample(Sampler, input.texcoord).xyz;
   float3 normal = GBufferNormal.Sample(Sampler, input.texcoord).xyz;
   float4 diffuse = GBufferDiffuse.Sample(Sampler, input.texcoord);
   float3 viewDirection = GBufferViewDirection.Sample(Sampler, input.texcoord).xyz;

    // ���� ��� ���� 

    return float4(position, 1.0f); // ���� ���� ��ȯ
}