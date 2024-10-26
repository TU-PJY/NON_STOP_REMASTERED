#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;
uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;
uniform vec3 objectColor;

uniform bool UseBlur;
uniform float Radius; 
uniform vec2 TexelSize;

vec4 BlurResult;
vec4 DefaultResult;
vec4 FinalResult;

vec4 ComputeBlur() {
    vec4 BlurResultValue = vec4(0.0);
    vec4 BlurColor = vec4(0.0);
    float BlurTotal = 0.0;
    int BlurNum = int(Radius);

    for(int y = -BlurNum; y <= BlurNum; y++) {
        for(int x = -BlurNum; x <= BlurNum; x++) {
            vec2 offset = vec2(float(x), float(y)) * TexelSize * 3.0;
            vec4 sample = texture(outTexture, TexCoord + offset);
            BlurColor += sample;
            BlurTotal += 1.0;
        }
    }
    BlurResultValue = BlurColor / BlurTotal;

    return BlurResultValue;
}

void InitResult() {
    BlurResult = vec4(0.0);
    DefaultResult = vec4(0.0);
    FinalResult = vec4(0.0);
}

void main() {
    InitResult();

    BlurResult = mix(vec4(0.0), ComputeBlur(), float(UseBlur));
    DefaultResult = mix(texture(outTexture, TexCoord), vec4(0.0), float(UseBlur));

    FinalResult = mix(DefaultResult, BlurResult, float(UseBlur));
    fragColor = mix(vec4(FinalResult.rgb + objectColor.rgb, FinalResult.a * transparency), vec4(0.0), step(FinalResult.a, 0.01));
}