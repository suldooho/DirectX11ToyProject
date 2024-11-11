static const float3 kLightDirection = float3(1.0f, -1.0f, 0.0f);
//static const float4 diffuseColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 
//static const float4 specularColor = float4(1.0f, 1.0f, 1.0f, 1.0f); 
static const float kDirectionalLightSpecularPower = 100.0f;
static const float kPointLightSpecularPower = 1.0f;
static const float kSpotLightSpecularPower = 1.0f;
static const float kMinTessFactor = 1.0f;
static const float kMaxTessDistance = 50.0f; 

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

cbuffer LightCount : register(b2)
{
    unsigned int PointLightCount : packoffset(c0);
    unsigned int SpotLightCount : packoffset(c1);
}; 

struct PointLight
{
    float3 color;
    float3 position;
    float range; 
};
 
struct SpotLight
{
    float4 color;
    float3 position;
    float range;
    float3 direction;
    float angle;
    float time;
    float3 padding;
};
 
StructuredBuffer<PointLight> PointLights : register(t0);  
StructuredBuffer<SpotLight> SpotLights : register(t1);  
 
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);
Texture2D HeightMap : register(t2);
 
Texture2D<float4> GBufferPosition : register(t4);
Texture2D<float4> GBufferNormal : register(t5);
Texture2D<float4> GBufferDiffuse : register(t6);
Texture2D<float4> GBufferViewDirection : register(t7);

SamplerState Sampler : register(s0);

