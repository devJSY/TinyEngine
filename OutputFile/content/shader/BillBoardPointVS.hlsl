#include "struct.hlsli"
#include "global.hlsli"

struct GS_IN
{
    float4 pos : POSITION;
};

GS_IN main(VS_IN input)
{
    GS_IN output;
    
    output.pos = mul(float4(input.vPos, 1.0), g_matWorld);
    
    return output;
}