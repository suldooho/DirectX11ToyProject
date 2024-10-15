struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

// 정점-쉐이더 
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.position = mul(input.position, gmtxWorld);
    output.position = mul(output.position, gmtxView);
    output.position = mul(output.position, gmtxProjection);
    output.color = input.color;
//입력되는 정점의 색상을 그대로 출력한다. 
    return output;
}

// 픽셀-쉐이더
float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
//입력되는 정점의 색상을 그대로 출력한다. 
}