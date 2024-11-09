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
    float3 prevPosition : INSTANCE_PREV_POSITION; // ���� �������� ��ġ
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
    float lineWidth = 0.2f; // ������ �β��� �����մϴ� (������ ���� �����̽�)
    float3 cameraDir = normalize(CameraPosition - (startPos + endPos) * 0.5f);

    float3 up = normalize(cross(lineDir, cameraDir)) * lineWidth;

     // �簢���� �����ϱ� ���� �� ���� ���� ��ġ�� ����մϴ�
    float3 vertices[4];
    vertices[0] = startPos + up; // ������ ����
    vertices[1] = startPos - up; // ������ �Ʒ���
    vertices[2] = endPos + up; // ���� ����
    vertices[3] = endPos - up; // ���� �Ʒ���
     
    // �� ������ ���� PixelInput ����ü ����
    PixelInput verts[4];
    float2 texCoords[4] = { float2(1, 0), float2(0, 0), float2(1, 1), float2(0, 1) };
    for (uint i = 0; i < 4; ++i)
    {
        verts[i].positionH = mul(float4(vertices[i], 1.0), View);
        verts[i].positionH = mul(verts[i].positionH, Projection);
        verts[i].color = input[0].color;
        verts[i].texCoord = texCoords[i];
    }
     
    // ���� �簢���� �� ���� �ﰢ������ �����Ͽ� ��Ʈ���� �߰�
    triStream.Append(verts[0]); // ù ��° �ﰢ��
    triStream.Append(verts[2]);
    triStream.Append(verts[1]);

    triStream.Append(verts[2]); // �� ��° �ﰢ��
    triStream.Append(verts[1]);
    triStream.Append(verts[3]);
}


float4 PS(PixelInput input) : SV_Target
{
    float4 alpha = DiffuseMap.Sample(Sampler, input.texCoord);
    
    return float4(input.color, alpha.a);
}