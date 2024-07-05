#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV0 = _in.vUV0;
    
    return output;
}
