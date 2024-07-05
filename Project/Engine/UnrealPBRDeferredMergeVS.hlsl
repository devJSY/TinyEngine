#include "struct.hlsli"
#include "global.hlsli"

PS_IN main(VS_IN _in)
{
    PS_IN output = (PS_IN) 0.f;
    
    output.vPosProj = float4(_in.vPos * 2.f, 1.f);
    output.vUV0 = _in.vUV0;
    
    return output;
}
