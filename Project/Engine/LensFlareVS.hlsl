#include "struct.hlsli"
#include "global.hlsli"

struct GS_Input
{
    float4 pos : SV_POSITION;
};

GS_Input main(VS_IN input)
{
    GS_Input output;
    output.pos = float4(input.vPos, 1.f);
    return output;
}