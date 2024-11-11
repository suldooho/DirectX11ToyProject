#include "Header.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    int viewIndex : VIEWINDEX; // �� �ؽ�ó �� �ε��� (0, 1, 2, 3)
};

PixelInput main(uint vertexID : SV_VertexID) 
{
    PixelInput output;
   
    // �ϵ��ڵ��� ��ġ�� �ؽ�ó ��ǥ ����
    float2 pos[24] =
    {
    // ù ��° �ؽ�ó (���� ��� �𼭸��� ��ġ)
        float2(-1.0, 1.0), float2(-0.5, 1.0), float2(-1.0, 0.5),
        float2(-0.5, 1.0), float2(-0.5, 0.5), float2(-1.0, 0.5),

        // �� ��° �ؽ�ó (ù ��° �ؽ�ó �����ʿ� ��ġ)
        float2(-0.5, 1.0), float2(0.0, 1.0), float2(-0.5, 0.5),
        float2(0.0, 1.0), float2(0.0, 0.5), float2(-0.5, 0.5),

        // �� ��° �ؽ�ó (ù ��° �ؽ�ó �Ʒ��� ��ġ)
        float2(-1.0, 0.5), float2(-0.5, 0.5), float2(-1.0, 0.0),
        float2(-0.5, 0.5), float2(-0.5, 0.0), float2(-1.0, 0.0),

        // �� ��° �ؽ�ó (�� ��° �ؽ�ó �Ʒ��� ��ġ)
        float2(-0.5, 0.5), float2(0.0, 0.5), float2(-0.5, 0.0),
        float2(0.0, 0.5), float2(0.0, 0.0), float2(-0.5, 0.0)
    };

    float2 uv[24] =
    {
        // �� �簢���� �ؽ�ó ��ǥ (���� 6���� ���� UV ��ǥ)
        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1)
    };

    // ���� ID�� ���� ��ġ�� �ؽ�ó ��ǥ ����
    output.position = float4(pos[vertexID], 0.0f, 1.0f);
    output.texcoord = uv[vertexID];
    output.viewIndex = vertexID / 6; // �� �ؽ�ó�� �ε���

    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    if (input.viewIndex == 0)
    {
        return GBufferPosition.Sample(Sampler, input.texcoord); // ���� ��
    }
    else if (input.viewIndex == 1)
    {
        return GBufferNormal.Sample(Sampler, input.texcoord); // ������ ��
    }
    else if (input.viewIndex == 2)
    {
        return GBufferDiffuse.Sample(Sampler, input.texcoord); // ���� �Ʒ�
    }
    else if (input.viewIndex == 3)
    {
        return GBufferViewDirection.Sample(Sampler, input.texcoord); // ������ �Ʒ�
    }
    
    return float4(0, 0, 0, 1); // �⺻ �� (���� ó��)
}