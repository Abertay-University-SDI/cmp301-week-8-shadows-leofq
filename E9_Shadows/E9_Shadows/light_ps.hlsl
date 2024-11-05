// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambient[3];
    float4 diffuse[3];
    float4 position[3];
    float4 direction[3];
    float4 Colour[3];

    float spotAngle;
    float constantFactor;
    float linearFactor;
    float quadraticFactor;

    int LightType;
    bool Enabled;
    float padding;
    float padding2;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 ldiffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(ldiffuse * intensity);
    return colour;
}

float3 calculateAttenuation(float cFactor, float lFactor, float qFactor, float dist)
{
    return 1 / (cFactor + (lFactor * dist) + (qFactor * pow(dist, 2)));
}

float4 calcSpecular(float3 dir, float3 normal, float3 viewVec, float4 specColour, float specPower)
{
    float3 halfway = normalize(dir + viewVec);
    float specIntensity = pow(max(dot(normal, halfway), 0.0), specPower);
    return saturate(specColour * specIntensity);
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;

    for (int i = 0; i < 3; i++)
    {
        float dist = length(position[i].xyz - input.worldPosition);
        float attenuation = 1 / (0.5 + (0.125 * dist) + (0.0 * pow(dist, 2)));
        float4 diff = diffuse[i] * attenuation;
        
        textureColour = texture0.Sample(sampler0, input.tex);
        float3 lightVector = normalize(position[i].xyz - input.worldPosition);
        
        if (position[i].w == 0.1)
        {
            lightColour += ambient[i] + calculateLighting(lightVector, input.normal, diff);
        }
        if (position[i].w == 0.2)
        {
            lightColour += ambient[i] + calculateLighting(-direction[i].xyz, input.normal, diff[i]);
        }
    }
    return lightColour; // * textureColour;
}






