static const float3 kLightDirection = float3(0.2182f, -0.9759f, 0.0f);
//static const float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 
//static const float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 
static const float kSpecularPower = 1000.0f;

cbuffer WorldMatrix : register(b0)
{
    matrix World : packoffset(c0);
};
 
cbuffer ViewProjectionMatrix : register(b1)
{
    matrix View : packoffset(c0);
    matrix Projection : packoffset(c4);
    float3 CameraPosition : packoffset(c8); 
}; 

Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1); 
 
Texture2D<float4> GBufferPosition : register(t4);
Texture2D<float4> GBufferNormal : register(t5);
Texture2D<float4> GBufferDiffuse : register(t6);
Texture2D<float4> GBufferViewDirection : register(t7);

SamplerState Sampler : register(s0);