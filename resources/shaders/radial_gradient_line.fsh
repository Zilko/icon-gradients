#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform sampler2D u_texture;

uniform int stopAt;
uniform vec2 positions[24];
uniform vec4 colors[24];

uniform vec2 uvMin;
uniform vec2 uvMax;

void main()
{
    vec4 texColor = texture2D(u_texture, v_texCoord);

    float gray = max(max(texColor.r, texColor.g), texColor.b);
    float mask = pow(2.0, -10.0 * gray);

    texColor = vec4(vec3(texColor.a * mask), texColor.a * mask);

    vec4 finalColor = vec4(0.0);
    float totalWeight = 0.0;

    for (int i = 0; i < stopAt; ++i) {
        float dist = distance((v_texCoord - uvMin) / (uvMax - uvMin), positions[i]);
        dist *= dist;

        float weight = dist != 0.0 ? 1.0 / dist : 0.0;

        finalColor += colors[i] * weight;
        totalWeight += weight;
    }

    gl_FragColor = finalColor / totalWeight * texColor;
}