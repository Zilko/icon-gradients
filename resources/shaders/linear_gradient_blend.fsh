#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;
uniform sampler2D u_texture;

uniform int stopAt;
uniform float stops[24];
uniform vec4 colors[24];

uniform vec2 startPoint;
uniform vec2 endPoint;
uniform vec2 uvMin;
uniform vec2 uvMax;

void main() {
    vec4 texColor = texture2D(u_texture, v_texCoord);

    if (stopAt <= 1) {
        vec4 c = colors[0];
        gl_FragColor = texColor * c * v_fragmentColor;
        return;
    }

    vec2 dir = endPoint - startPoint;
    float len = length(dir);
    if (len < 1e-6) {
        vec4 c = colors[0];
        gl_FragColor = texColor * c * v_fragmentColor;
        return;
    }

    vec2 unit = dir / len;
    vec2 uv  = (v_texCoord - uvMin) / (uvMax - uvMin);
    float proj = dot(uv - startPoint, unit);
    float t = clamp(proj / len, 0.0, 1.0);

    vec4 c0 = colors[0];
    if (t <= stops[0]) {
        gl_FragColor = texColor * c0 * v_fragmentColor;
        return;
    }

    for (int i = 0; i < 23; ++i) {
        if (i >= stopAt - 1) break;
        float a = stops[i];
        float b = stops[i+1];
        if (t <= b) {
            float localT = (t - a) / (b - a);
            vec4 c = mix(colors[i], colors[i+1], localT);
            gl_FragColor = texColor * c * v_fragmentColor;
            return;
        }
    }

    vec4 clast = colors[stopAt - 1];
    gl_FragColor = texColor * clast * v_fragmentColor;
}
