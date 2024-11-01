#include "Header.hlsli"

struct VertexInput
{
    float3 positionL : POSITION;
    float2 texcoord : TEXCOORD; 
};

struct HullInput
{
    float3 positionW : POSITION;
    float2 texcoord : TEXCOORD;
};

struct PatchTess
{
    float EdgeTess[4] : SV_TessFactor;
    float InsideTess[2] : SV_InsideTessFactor;
};

struct DomainInput
{
    float3 positionW : POSITIONT;
    float2 texcoord : TEXCOORD; 
};

struct PixelInput
{
    float3 positionW : POSITION;
    float2 texcoord : TEXCOORD;
    float4 positionH : SV_POSITION;
};

struct GBufferOutput
{
    float4 pos : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 diffuse : SV_TARGET2;
    float4 viewDir : SV_TARGET3;
};

HullInput VS(VertexInput input)
{
    HullInput output;
    
    output.positionW = mul(float4(input.positionL, 1.0f), World);
    output.texcoord = input.texcoord;  
    
    return output;
}
 
PatchTess ConstantHS(InputPatch<HullInput, 4> patch, uint PatchID : SV_PrimitiveID)
{
    PatchTess patchTess;
    
    // 카메라와 패치 중심 간 거리 계산
    float3 patchCenter = (patch[0].positionW + patch[1].positionW + patch[2].positionW + patch[3].positionW) * 0.25f;
    float distance = length(CameraPosition - patchCenter);
    
    // 거리 기반 테셀레이션 팩터 계산
    float tessFactor = lerp(64.0f, 0.0f, saturate(distance / kMaxTessDistance));
    
    patchTess.EdgeTess[0] = tessFactor;
    patchTess.EdgeTess[1] = tessFactor;
    patchTess.EdgeTess[2] = tessFactor;
    patchTess.EdgeTess[3] = tessFactor;
    patchTess.InsideTess[0] = tessFactor;
    patchTess.InsideTess[1] = tessFactor;

    return patchTess;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
DomainInput HS(InputPatch<HullInput, 4> controlPoint, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    DomainInput output;
    
    output.positionW = controlPoint[i].positionW;
    output.texcoord = controlPoint[i].texcoord;

    return output;
}

[domain("quad")]
PixelInput main(PatchTess patchTess, float2 uv : SV_DomainLocation, const OutputPatch<DomainInput, 4> quad)
{
    PixelInput output;

    // 텍스처 좌표와 월드 위치 보간
    output.positionW = lerp(lerp(quad[0].positionW, quad[1].positionW, uv.x), lerp(quad[2].positionW, quad[3].positionW, uv.x), uv.y);
    output.texcoord = lerp(lerp(quad[0].texcoord, quad[1].texcoord, uv.x), lerp(quad[2].texcoord, quad[3].texcoord, uv.x), uv.y);
      
    float4 positionH = mul(float4(output.positionW, 1.0f), View);
    output.positionH = mul(positionH, Projection);

    return output;

} 

GBufferOutput PS(PixelInput input)
{
    GBufferOutput output;
     
    // 노멀맵을 이용하여 표면 노멀 계산
    float3 normalFromMap = NormalMap.Sample(Sampler, input.texcoord).xzy * 2.0f - 1.0f; // TBN계산 생략
    float3 normalW = normalize(normalFromMap);

    // 뷰 방향 계산
    float3 viewDir = normalize(CameraPosition - input.positionW);

    // 카메라와 픽셀 위치 간 거리 계산
    float distance = length(viewDir);
    
    // 거리 기반으로 높이맵 강도 조절
    float heightFactor = (distance < kMaxTessDistance) ? 0.1f : 0.0f; // 가까울수록 강도 적용
    
    // 높이맵 적용
    float3 modifiedPositionW = input.positionW; // 높이맵 적용된 위치
    if (heightFactor > 0.0f)
    {
        float height = HeightMap.Sample(Sampler, input.texcoord).r;
        modifiedPositionW.y += height * heightFactor;
    }

    // GBuffer에 필요한 정보 설정
    output.pos = float4(modifiedPositionW, 1.0f); // G-버퍼에 저장되는 월드 좌표
    output.normal = float4(normalW, 0.0f);
    output.diffuse = DiffuseMap.Sample(Sampler, input.texcoord);
    output.viewDir = float4(viewDir, 0.0f);

    return output;
}
