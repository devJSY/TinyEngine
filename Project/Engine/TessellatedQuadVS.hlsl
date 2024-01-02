#include "global.hlsli"
#include "struct.hlsli"

struct VertexOut
{
    float4 pos : POSITION;
};

VertexOut main(VS_IN input)
{
    VertexOut vout;
	
    vout.pos = float4(input.vPos, 1.0);

    return vout;
}
