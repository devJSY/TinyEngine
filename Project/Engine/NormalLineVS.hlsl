#include "global.hlsli"
#include "func.hlsli"

struct GS_Input
{
    float4 posModel : SV_POSITION;
    float3 normal : NORMAL;
};

GS_Input main(VS_IN input)
{
    GS_Input output;
    float3 PrevPos = input.vPos;
    if (g_iAnim)
    {
        Skinning(input.vPos, PrevPos, input.vTangent, input.vBitangent, input.vNormal
              , input.vWeights, input.vIndices, 0);
    }
    
    output.posModel = float4(input.vPos, 1.0);
    output.normal = input.vNormal;

    return output;
}
