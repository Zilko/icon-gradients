#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
uniform sampler2D u_texture;

uniform int stopAt;
uniform vec2 positions[24];
uniform vec4 colors[24];

uniform vec2 uvMin;
uniform vec2 uvMax;

void main() {
    vec2 uvNorm = (v_texCoord - uvMin) / (uvMax - uvMin);
    vec4 texColor = texture2D(u_texture, v_texCoord);

    vec4 finalColor = vec4(0.0);
    float totalWeight = 0.0;

    for (int i = 0; i < stopAt; ++i) {
        float dist = distance(uvNorm, positions[i]);
        float weight = (dist > 0.0) ? 1.0 / (dist * dist) : 0.0;
        finalColor += colors[i] * weight;
        totalWeight += weight;
    }

    vec4 blendColor = (finalColor / totalWeight) * texColor;
    gl_FragColor = blendColor * v_fragmentColor;
}
