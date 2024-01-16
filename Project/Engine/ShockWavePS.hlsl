#include "struct.hlsli"
#include "global.hlsli"
//Use as you will.

float4 main(PS_IN input) : SV_TARGET
{
    //Sawtooth function to pulse from centre.
    float offset = (g_time - floor(g_time)) / g_time;
    float CurrentTime = (g_time) * (offset);
    
    float3 WaveParams = float3(10.0, 0.8, 0.1);
    
    float ratio = g_RenderResolution.y / g_RenderResolution.x;
    
    //Use this if you want to place the centre with the mouse instead
	//vec2 WaveCentre = vec2( iMouse.xy / iResolution.xy );
       
    float2 WaveCentre = float2(0.5, 0.5);
    WaveCentre.y *= ratio;
   
    float2 texCoord = input.vUV / g_RenderResolution.xy;
    texCoord.y *= ratio;
    float Dist = distance(texCoord, WaveCentre);
    
	
    float4 Color = g_postprocess_Tex.Sample(g_LinearSampler, texCoord);
    
    //Only distort the pixels within the parameter distance from the centre
    if ((Dist <= ((CurrentTime) + (WaveParams.z))) &&
	(Dist >= ((CurrentTime) - (WaveParams.z))))
    {
        //The pixel offset distance based on the input parameters
        float Diff = (Dist - CurrentTime);
        float ScaleDiff = (1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y));
        float DiffTime = (Diff * ScaleDiff);
        
        //The direction of the distortion
        float2 DiffTexCoord = normalize(texCoord - WaveCentre);
        
        //Perform the distortion and reduce the effect over time
        texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.0));
        Color = g_postprocess_Tex.Sample(g_LinearSampler, texCoord);
        
        //Blow out the color and reduce the effect over time
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0);
    }
    
    return Color;
}

