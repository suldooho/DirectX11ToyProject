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


    // 정점 ID에 따라 위치와 텍스처 좌표를 선택
    output.position = float4(pos[vertexID], 0.0f, 1.0f);
    output.texcoord = uv[vertexID]; 

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    // G-버퍼에서 정보 샘플링
    float3 position = GBufferPosition.Sample(Sampler, input.texcoord).xyz;
    float3 normal = normalize(GBufferNormal.Sample(Sampler, input.texcoord).xyz);
    float4 diffuse = GBufferDiffuse.Sample(Sampler, input.texcoord);
    float3 viewDir = normalize(GBufferViewDirection.Sample(Sampler, input.texcoord).xyz); // 뷰 방향

    // 초기 색상 설정
    float3 finalColor = float3(0.0, 0.0, 0.0);  

    // 1. Directional Light 계산
    float3 lightDir = normalize(-kLightDirection); // 빛의 반대 방향
    float NdotL = saturate(dot(normal, lightDir));
    float3 reflectDir = reflect(-lightDir, normal); // 반사 벡터
    float3 specular = pow(saturate(dot(viewDir, reflectDir)), kDirectionalLightSpecularPower);

    finalColor += diffuse.rgb * NdotL + specular;

    // 2. Point Light 계산
    [loop]
    for (uint i = 0; i < PointLights.Length; i++)
    { 
        // output.position = mul(worldPosition, View);
        // output.position = mul(output.position, Projection);
        float3 lightVec = PointLights[i].position - position; // 스크린 상의 xy좌표값 - 스크린상의 xy좌표값
        float distance = length(lightVec);
        float3 pointLightDir = normalize(lightVec);
        NdotL = saturate(dot(normal, pointLightDir));
        float attenuation = saturate(1.0 / (distance * distance) / (PointLights[i].range * PointLights[i].range) + 0.1f);

        // 스펙큘러 계산
        reflectDir = reflect(-pointLightDir, normal);
        specular = pow(saturate(dot(viewDir, reflectDir)), kPointLightSpecularPower);
        specular = PointLights[i].color.rgb * specular * attenuation;

        finalColor += PointLights[i].color.rgb * attenuation + (diffuse.rgb * NdotL + specular);
    }

    // 3. Spot Light 계산
    [loop]
    for (uint i = 0; i < SpotLights.Length; i++)
    {
        float3 lightVec = SpotLights[i].position - position;
        float distance = length(lightVec);
        float3 spotLightDir = normalize(lightVec);
        NdotL = saturate(dot(normal, spotLightDir));
        float attenuation = saturate(1.0 / (distance * distance / (SpotLights[i].range * SpotLights[i].range) + 0.1f));


        // Spot 각도 계산
        float spotEffect = saturate(dot(SpotLights[i].direction, -spotLightDir));
        float smoothSpotEffect = smoothstep(SpotLights[i].angle * 0.5, SpotLights[i].angle, spotEffect);

        // 스펙큘러 계산
        reflectDir = reflect(-spotLightDir, normal);
        specular = pow(saturate(dot(viewDir, reflectDir)), kSpotLightSpecularPower);
        specular = SpotLights[i].color.rgb * specular * attenuation * smoothSpotEffect;

        finalColor += SpotLights[i].color.rgb + diffuse.rgb * NdotL * attenuation * smoothSpotEffect + specular;
    }

    return float4(finalColor, 1.0); 
}
