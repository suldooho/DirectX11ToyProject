#include "Header.hlsli"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    int viewIndex : VIEWINDEX; // 각 텍스처 뷰 인덱스 (0, 1, 2, 3)
};

PixelInput main(uint vertexID : SV_VertexID) 
{
    PixelInput output;
   
    // 하드코딩된 위치와 텍스처 좌표 설정
    float2 pos[24] =
    {
    // 첫 번째 텍스처 (왼쪽 상단 모서리에 위치)
        float2(-1.0, 1.0), float2(-0.5, 1.0), float2(-1.0, 0.5),
        float2(-0.5, 1.0), float2(-0.5, 0.5), float2(-1.0, 0.5),

        // 두 번째 텍스처 (첫 번째 텍스처 오른쪽에 위치)
        float2(-0.5, 1.0), float2(0.0, 1.0), float2(-0.5, 0.5),
        float2(0.0, 1.0), float2(0.0, 0.5), float2(-0.5, 0.5),

        // 세 번째 텍스처 (첫 번째 텍스처 아래에 위치)
        float2(-1.0, 0.5), float2(-0.5, 0.5), float2(-1.0, 0.0),
        float2(-0.5, 0.5), float2(-0.5, 0.0), float2(-1.0, 0.0),

        // 네 번째 텍스처 (두 번째 텍스처 아래에 위치)
        float2(-0.5, 0.5), float2(0.0, 0.5), float2(-0.5, 0.0),
        float2(0.0, 0.5), float2(0.0, 0.0), float2(-0.5, 0.0)
    };

    float2 uv[24] =
    {
        // 각 사각형의 텍스처 좌표 (정점 6개씩 같은 UV 좌표)
        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1),

        float2(0, 0), float2(1, 0), float2(0, 1),
        float2(1, 0), float2(1, 1), float2(0, 1)
    };

    // 정점 ID에 따라 위치와 텍스처 좌표 설정
    output.position = float4(pos[vertexID], 0.0f, 1.0f);
    output.texcoord = uv[vertexID];
    output.viewIndex = vertexID / 6; // 각 텍스처의 인덱스

    return output;
}

float4 PS(PixelInput input) : SV_Target
{
    if (input.viewIndex == 0)
    {
        return GBufferPosition.Sample(Sampler, input.texcoord); // 왼쪽 위
    }
    else if (input.viewIndex == 1)
    {
        return GBufferNormal.Sample(Sampler, input.texcoord); // 오른쪽 위
    }
    else if (input.viewIndex == 2)
    {
        return GBufferDiffuse.Sample(Sampler, input.texcoord); // 왼쪽 아래
    }
    else if (input.viewIndex == 3)
    {
        return GBufferViewDirection.Sample(Sampler, input.texcoord); // 오른쪽 아래
    }
    
    return float4(0, 0, 0, 1); // 기본 값 (예외 처리)
}