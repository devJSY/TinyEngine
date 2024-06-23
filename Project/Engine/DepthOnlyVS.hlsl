#include "global.hlsli"
#include "func.hlsli"

float4 main(VS_IN input) : SV_Position
{
    if (g_iAnim)
    {
        Skinning(input.vPos, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, 0);
    }
    
    return mul(float4(input.vPos, 1.f), g_matWVP);
}