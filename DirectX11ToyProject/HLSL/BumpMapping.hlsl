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
     
    // 월드 위치 계산
    float4 worldPosition = mul(float4(input.position, 1.0f), World);
    output.positionH = mul(worldPosition, View);
    output.positionH = mul(output.positionH, Projection);
     
    output.texcoord = input.texcoord;

    // 월드 공간의 접선(Tangent), 비노멀(Bitangent), 노멀(Normal) 벡터 계산
    output.tangentW = normalize(mul(input.tangent, (float3x3) World));
    output.bitangentW = normalize(mul(input.bitangent, (float3x3) World));
    output.normalW = normalize(mul(input.normal, (float3x3) World));
    
    // 월드 위치 전달
    output.positionW = worldPosition.xyz;
    
    // 뷰 방향 계산
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

    // 범프 맵에서 노멀 벡터 샘플링 및 [-1, 1] 범위로 변환
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xyz * 2.0f - 1.0f;

    // TBN 공간의 노멀을 월드 공간으로 변환
    float3 normal = normalize(mul(normalFromMap, TBN));

    // 픽셀 셰이더에서 받은 월드 위치 사용
    float3 worldPosition = input.positionW;

    // 뷰 방향 (정점 셰이더에서 계산한 값을 사용)
    float3 viewDir = input.viewDirection;

    // G-버퍼에 필요한 정보 저장
    output.pos = float4(worldPosition, 1.0); // 월드 위치
    output.normal = float4(normal, 0.0); // 노멀 벡터
    output.diffuse = DiffuseMap.Sample(Sampler, input.texcoord); // 디퓨즈 색상
    output.viewDir = float4(viewDir, 0.0); // 뷰 방향 벡터

    return output;
}
