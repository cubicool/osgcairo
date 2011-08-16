varying vec2 sampleCoord;

uniform sampler2D Texture;
uniform vec4      Color;
uniform float     AlphaMin;
uniform float     AlphaMax;

void main() {
	float a    = smoothstep(AlphaMin, AlphaMax, texture2D(Texture, sampleCoord).a);
	vec4  frag = vec4(0.0, 0.0, 0.0, 0.0);
	vec4  col  = vec4(Color.rgb * a, a);

	gl_FragColor = (col + (1.0 - col.a) * frag) * Color.a;
}

