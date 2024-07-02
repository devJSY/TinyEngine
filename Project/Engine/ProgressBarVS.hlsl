#include "global.hlsli"
#include "struct.hlsli"

PS_IN main(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.0;
    
    output.vPosProj = mul(float4(_in.vPos, 1.0), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV0 = _in.vUV0;
    
    output.vPosWorld = mul(float4(_in.vPos, 1.0), g_matWorld).xyz;
    
    return output;
}
