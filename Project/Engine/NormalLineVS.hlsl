struct VS_IN
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct GS_IN
{
    float4 posModel : SV_POSITION;
    float3 normalWorld : NORMAL;
};

GS_IN main(VS_IN input)
{
    GS_IN output;

    output.posModel = float4(input.vPos, 1.0);
    output.normalWorld = input.vNormal;

    return output;
}
