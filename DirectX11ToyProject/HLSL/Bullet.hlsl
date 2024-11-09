#include "Header.hlsli"

struct VertexInput
{  
    float3 position : INSTANCE_POSITION;
    float3 prevPosition : INSTANCE_PREV_POSITION;
    float3 color : INSTANCECOLOR;
};

struct GeometryInput
{  
    float3 position : INSTANCE_POSITION;
    float3 prevPosition : INSTANCE_PREV_POSITION; // 이전 프레임의 위치
    float3 color : INSTANCECOLOR;
};

struct PixelInput
{
    float4 positionH : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD; 
};

GeometryInput VS(VertexInput input)
{
    GeometryInput output;
     
    output.position = input.position;
    output.prevPosition = input.prevPosition;
    output.color = input.color;

    return output;
}

[maxvertexcount(6)]
void main(point GeometryInput input[1], inout TriangleStream<PixelInput> triStream)
{ 
    if (all(input[0].position == input[0].prevPosition))
    {
        return;
    } 
    
    float3 startPos = input[0].position;
    float3 endPos = input[0].prevPosition;
    
    float3 lineDir = normalize(endPos - startPos);
    float lineWidth = 0.2f; // 라인의 두께를 설정합니다 (단위는 월드 스페이스)
    float3 cameraDir = normalize(CameraPosition - (startPos + endPos) * 0.5f);

    float3 up = normalize(cross(lineDir, cameraDir)) * lineWidth;

     // 사각형을 구성하기 위한 네 개의 정점 위치를 계산합니다
    float3 vertices[4];
    vertices[0] = startPos + up; // 시작점 위쪽
    vertices[1] = startPos - up; // 시작점 아래쪽
    vertices[2] = endPos + up; // 끝점 위쪽
    vertices[3] = endPos - up; // 끝점 아래쪽
     
    // 각 정점에 대해 PixelInput 구조체 생성
    PixelInput verts[4];
    float2 texCoords[4] = { float2(1, 0), float2(0, 0), float2(1, 1), float2(0, 1) };
    for (uint i = 0; i < 4; ++i)
    {
        verts[i].positionH = mul(float4(vertices[i], 1.0), View);
        verts[i].positionH = mul(verts[i].positionH, Projection);
        verts[i].color = input[0].color;
        verts[i].texCoord = texCoords[i];
    }
     
    // 궤적 사각형을 두 개의 삼각형으로 구성하여 스트림에 추가
    triStream.Append(verts[0]); // 첫 번째 삼각형
    triStream.Append(verts[2]);
    triStream.Append(verts[1]);

    triStream.Append(verts[2]); // 두 번째 삼각형
    triStream.Append(verts[1]);
    triStream.Append(verts[3]);
}


float4 PS(PixelInput input) : SV_Target
{
    float4 alpha = DiffuseMap.Sample(Sampler, input.texCoord);
    
    return float4(input.color, alpha.a);
}