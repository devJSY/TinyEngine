struct VS_Input
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct GS_Input
{
    float4 posModel : SV_POSITION;
    float3 normalWorld : NORMAL;
};

GS_Input main(VS_Input input)
{
    GS_Input output;

    output.posModel = float4(input.vPos, 1.0);
    output.normalWorld = input.vNormal;

    return output;
}
