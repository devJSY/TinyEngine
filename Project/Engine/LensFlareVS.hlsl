#include "struct.hlsli"
#include "global.hlsli"

struct GS_Input
{
    float4 pos : SV_POSITION;
    float3 PosView : POSITION;
};

GS_Input main(VS_IN input)
{
    GS_Input output;
    output.pos = float4(input.vPos, 1.f);
    output.PosView = mul(output.pos, g_matWV).xyz;
    return output;
}