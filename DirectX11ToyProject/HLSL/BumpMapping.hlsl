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
};

PixelInput main(VertexInput input)
{
    PixelInput output;
     
    float4 worldPosition = mul(float4(input.position, 1.0f), World); 
    output.position = mul(worldPosition, View);
    output.position = mul(output.position, Projection);
     
    output.texcoord = input.texcoord;
    
    output.T = normalize(mul(input.tangent, (float3x3) World));
    output.B = normalize(mul(input.bitangent, (float3x3) World));
    output.N = normalize(mul(input.normal, (float3x3) World));
       
    output.viewDirection = normalize(CameraPosition - worldPosition.xyz);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3x3 TBN = float3x3(input.T, input.B, input.N);

    // ���� ���� ���� �븻 ���� ���� (�븻 �� [-1, 1]�� ��ȯ)
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xyz * 2.0f - 1.0f;

    // TBN ������ �븻�� ���� �������� ��ȯ
    float3 normal = normalize(mul(normalFromMap, TBN));

    // ��ǻ�� ���� ��� (�𷺼ų� ����Ʈ)
    float lightIntensity = saturate(dot(normal, -kLightDirection));

    // �⺻ �ؽ�ó ���ø�
    float4 baseColor = DiffuseMap.Sample(Sampler, input.texcoord);

    // ����ŧ�� ���� ���
    float3 viewDir = normalize(input.viewDirection); // �� ����
    float3 reflectDir = reflect(kLightDirection, normal); // �ݻ� ���� ���
     
    float specFactor = pow(saturate(dot(viewDir, reflectDir)), kSpecularPower);

    // ���� ���� ����  
    float4 finalColor = (baseColor * lightIntensity) + specFactor;

    return finalColor;
}