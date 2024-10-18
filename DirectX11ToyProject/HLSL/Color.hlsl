#include "Header.hlsli"

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

// ����-���̴� 
VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(float4(input.position, 1.0f), gmtxWorld);
    output.position = mul(output.position, gmtxView);
    output.position = mul(output.position, gmtxProjection);
    output.color = input.color;
//�ԷµǴ� ������ ������ �״�� ����Ѵ�. 
    return output;
}

// �ȼ�-���̴�
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
//�ԷµǴ� ������ ������ �״�� ����Ѵ�. 
}