#include "Header.hlsli"

struct VertexInput
{
    float3 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float3 normal : NORMAL;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 T : TANGENT;
    float3 B : BITANGENT;
    float3 N : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPosition : TEXCOORD2; // �߰��� ���� ��ġ
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
    PixelInput output;
     
    // ���� ��ġ ���
    float4 worldPosition = mul(float4(input.position, 1.0f), World);
    output.position = mul(worldPosition, View);
    output.position = mul(output.position, Projection);
     
    output.texcoord = input.texcoord;

    // ���� ������ ����(Tangent), ����(Bitangent), ���(Normal) ���� ���
    output.T = normalize(mul(input.tangent, (float3x3) World));
    output.B = normalize(mul(input.bitangent, (float3x3) World));
    output.N = normalize(mul(input.normal, (float3x3) World));
    
    // ���� ��ġ ����
    output.worldPosition = worldPosition.xyz;
    
    // �� ���� ���
    output.viewDirection = normalize(CameraPosition - worldPosition.xyz);

    return output;
}

GBufferOutput PS(PixelInput input)
{
    GBufferOutput output;

    // TBN ��� ����
    float3x3 TBN = float3x3(input.T, input.B, input.N);

    // ���� �ʿ��� ��� ���� ���ø� �� [-1, 1] ������ ��ȯ
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xyz * 2.0f - 1.0f;

    // TBN ������ ����� ���� �������� ��ȯ
    float3 normal = normalize(mul(normalFromMap, TBN));

    // �ȼ� ���̴����� ���� ���� ��ġ ���
    float3 worldPosition = input.worldPosition;

    // �� ���� (���� ���̴����� ����� ���� ���)
    float3 viewDir = input.viewDirection;

    // G-���ۿ� �ʿ��� ���� ����
    output.pos = float4(worldPosition, 1.0); // ���� ��ġ
    output.normal = float4(normal, 0.0); // ��� ����
    output.diffuse = DiffuseMap.Sample(Sampler, input.texcoord); // ��ǻ�� ����
    output.viewDir = float4(viewDir, 0.0); // �� ���� ����

    return output;
}
