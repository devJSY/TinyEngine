#include "global.hlsli"

struct PatchConstOutput
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct HullOut
{
    float3 pos : POSITION;
};

struct DomainOut
{
    float4 pos : SV_POSITION;
};

[domain("quad")]
DomainOut main(PatchConstOutput patchConst,
             float2 uv : SV_DomainLocation,
             const OutputPatch<HullOut, 4> quad)
{
    DomainOut dout;

	// Bilinear interpolation.
    float3 v1 = lerp(quad[0].pos, quad[1].pos, uv.x);
    float3 v2 = lerp(quad[2].pos, quad[3].pos, uv.x);
    float3 p = lerp(v1, v2, uv.y);
    
    dout.pos = mul(float4(p, 1.0), g_matWVP);
    
    return dout;
}
