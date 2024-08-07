#version 450 core

uniform vec2 pixelsize;
uniform sampler2D gposition;
uniform sampler2D gnormal;
uniform sampler2D noise;

uniform int numSamples;

uniform float radius;
uniform float strength;

uniform vec3 samples[128];
uniform mat4 projection;

in vec2 coord;

out vec4 color;

void main()
{
    vec3 pos = texture(gposition, coord, -2.0).xyz;
    if(length(pos) == 0.0)
        return;

    vec3 normal = texture(gnormal, coord).xyz;
    vec3 random = texture(noise, coord * pixelsize * 1.0).xyz;

    vec3 t = normalize(random - normal * dot(random, normal));
    vec3 b = cross(normal, t);
    mat3 tbn = mat3(t, b, normal);

    float occlusion = 0.0;
    for(int i = 0; i < numSamples; i++)
    {
        vec3 samplePos = tbn * samples[i];
        samplePos = pos + samplePos * radius;
        
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gposition, offset.xy, -2.0).z;
        vec3 sampleNormal = texture(gnormal, offset.xy).xyz;
        if(sampleDepth == 0.0 || (dot(normal, sampleNormal) > 0.99)) continue;

        occlusion += (sampleDepth >= samplePos.z + 0.1 ? 1.0 : 0.0) * smoothstep(0.0, 1.0, radius / abs(pos.z - sampleDepth));
    }

    color = vec4(vec3(pow(1.0 - (occlusion / numSamples), strength)), 1.0);
}
