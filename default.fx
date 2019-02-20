
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
cbuffer cbChangesEveryFrame : register(b3)
{
    float3 direction;
    float3 radiance;
    float3 viewPos;
}





//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float3 norm : NORMAL;
};

struct PS_INPUT
{
    float2 Tex : TEXCOORD0;
    float4 Pos : SV_POSITION;
    float3 norm : TEXCOORD1;
    float3 wordPos : TEXCOORD2;

};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    output.Pos = mul(input.Pos, World);
    output.wordPos = output.Pos.xyz;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Tex = input.Tex;
    output.norm = mul(float4(input.norm, 0), World).xyz;
 
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{

    float ka =0.5;
    float kd = 0.5;
    float ks = 1;

    float3 color = txDiffuse.Sample(samLinear, input.Tex).rgb;
    float3 ambient = ka * color;
   
    float3 diffuse = float3(0, 0, 0);
    float3 specular = float3(0, 0, 0);
    float3 norm = normalize(input.norm);
    float3 V = viewPos - input.wordPos .xyz;

    //计算一个平行光
    float3 L = normalize(-direction);
    float symbol = max(0, dot(L, norm));
    diffuse = diffuse + kd * symbol * radiance;
    color = normalize(color);
    float3 H = normalize(L + V);
    symbol = max(0, dot(norm, H));
    specular = specular + ks * pow(symbol, 64) * radiance;


    return float4((ambient + diffuse)*color +specular  ,1);


}

