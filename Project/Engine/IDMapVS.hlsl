
#include "struct.hlsli"
#include "global.hlsli"
#include "func.hlsli"

PS_IN main(VS_IN input)
{
    PS_IN output = (PS_IN) 0;
    float3 PrevPos = input.vPos;
    if (g_iAnim)
    {
        Skinning(input.vPos, PrevPos, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, 0);
    }
    
    output.vPosProj = mul(float4(input.vPos, 1.f), g_matWVP);

    return output;
}