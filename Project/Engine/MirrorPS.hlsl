#include "struct.hlsli"
#include "global.hlsli"

#define MirrorColor g_vAlbedo

float4 main(PS_IN input) : SV_TARGET
{
    return MirrorColor;
}