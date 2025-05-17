#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
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
        gl_FragColor = texColor * colors[0];
        return;
    }

    vec2 dir = endPoint - startPoint;
    
    float len = length(dir);
    if (len < 1e-6) {
        gl_FragColor = texColor * colors[0];
        return;
    }

    vec2 unit = dir / len;
    vec2 uv  = (v_texCoord - uvMin) / (uvMax - uvMin);
    float proj = dot(uv - startPoint, unit);
    float t = clamp(proj / len, 0.0, 1.0);

    if (t <= stops[0]) {
        gl_FragColor = texColor * colors[0];
        return;
    }

    for (int i = 0; i < 23; ++i) {
        if (i >= stopAt - 1) break;
        float a = stops[i];
        float b = stops[i+1];
        if (t <= b) {
            float localT = (t - a) / (b - a);
            gl_FragColor = texColor * mix(colors[i], colors[i+1], localT);
            return;
        }
    }

    gl_FragColor = texColor * colors[stopAt - 1];
}
