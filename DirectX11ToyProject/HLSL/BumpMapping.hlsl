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
    float4 positionH : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 tangentW : TANGENT;
    float3 bitangentW : BITANGENT;
    float3 normalW : NORMAL;
    float3 viewDirection : VIEWDIRECTION;
    float3 positionW : POSITION;
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
    output.positionH = mul(worldPosition, View);
    output.positionH = mul(output.positionH, Projection);
     
    output.texcoord = input.texcoord;

    // ���� ������ ����(Tangent), ����(Bitangent), ���(Normal) ���� ���
    output.tangentW = normalize(mul(input.tangent, (float3x3) World));
    output.bitangentW = normalize(mul(input.bitangent, (float3x3) World));
    output.normalW = normalize(mul(input.normal, (float3x3) World));
    
    // ���� ��ġ ����
    output.positionW = worldPosition.xyz;
    
    // �� ���� ���
    output.viewDirection = normalize(CameraPosition - worldPosition.xyz);

    return output;
}

GBufferOutput PS(PixelInput input)
{
    GBufferOutput output;
    
    input.tangentW = normalize(input.tangentW);
    input.bitangentW = normalize(input.bitangentW);
    input.normalW = normalize(input.normalW);
    float3x3 TBN = float3x3(input.tangentW, input.bitangentW, input.normalW);

    // ���� �ʿ��� ��� ���� ���ø� �� [-1, 1] ������ ��ȯ
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xyz * 2.0f - 1.0f;

    // TBN ������ ����� ���� �������� ��ȯ
    float3 normal = normalize(mul(normalFromMap, TBN));

    // �ȼ� ���̴����� ���� ���� ��ġ ���
    float3 worldPosition = input.positionW;

    // �� ���� (���� ���̴����� ����� ���� ���)
    float3 viewDir = input.viewDirection;

    // G-���ۿ� �ʿ��� ���� ����
    output.pos = float4(worldPosition, 1.0); // ���� ��ġ
    output.normal = float4(normal, 0.0); // ��� ����
    output.diffuse = DiffuseMap.Sample(Sampler, input.texcoord); // ��ǻ�� ����
    output.viewDir = float4(viewDir, 0.0); // �� ���� ����

    return output;
}
