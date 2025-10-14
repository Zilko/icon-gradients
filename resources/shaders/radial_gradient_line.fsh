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
uniform vec2 u_pixelSize;
uniform float u_threshold;

void main()
{
    float closeBlack = 1.0;
    for(int x = -1; x < 2; x++) {
        for(int y = -1; y < 2; y++) {
            vec4 texColor = texture2D(u_texture, v_texCoord + vec2(x, y) * u_pixelSize);
            float pixelBlack = max(max(texColor.r, texColor.g), texColor.b) - 0.05;
            closeBlack = min(closeBlack, 1.0 - ((1.0 - (pixelBlack)) * texColor.a));
        }
    }
    vec4 texColor = texture2D(u_texture, v_texCoord);
    float mask = (1.0 - max(max(texColor.r, texColor.g), texColor.b)) * pow(2.0, closeBlack * u_threshold);

    texColor = vec4(texColor.a * mask);

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