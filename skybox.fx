
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


cbuffer cbChangesEveryFrame : register(b0)
{
    matrix World;
}
cbuffer cbChangesEveryFrame : register(b1)
{
    matrix View;
}
cbuffer ConstantBuffer : register(b2)
{
    matrix Projection;
}




//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
    float2 Tex : TEXCOORD0;
    float4 Pos : SV_POSITION;

};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;
 
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    return txDiffuse.Sample(samLinear, input.Tex);
}

