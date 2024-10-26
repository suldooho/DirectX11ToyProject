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

    // 범프 맵을 통해 노말 벡터 변형 (노말 맵 [-1, 1]로 변환)
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xyz * 2.0f - 1.0f;

    // TBN 공간의 노말을 월드 공간으로 변환
    float3 normal = normalize(mul(normalFromMap, TBN));

    // 디퓨즈 조명 계산 (디렉셔널 라이트)
    float lightIntensity = saturate(dot(normal, -kLightDirection));

    // 기본 텍스처 샘플링
    float4 baseColor = DiffuseMap.Sample(Sampler, input.texcoord);

    // 스페큘러 조명 계산
    float3 viewDir = normalize(input.viewDirection); // 뷰 방향
    float3 reflectDir = reflect(kLightDirection, normal); // 반사 방향 계산
     
    float specFactor = pow(saturate(dot(viewDir, reflectDir)), kSpecularPower);

    // 최종 조명 색상  
    float4 finalColor = (baseColor * lightIntensity) + specFactor;

    return finalColor;
}