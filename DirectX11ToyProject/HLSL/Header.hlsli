cbuffer cbWorldMatrix : register(b0)
{
    matrix gmtxWorld : packoffset(c0);
};
 
cbuffer cbViewProjectionMatrix : register(b1)
{
    matrix gmtxView : packoffset(c0);
    matrix gmtxProjection : packoffset(c4);
};