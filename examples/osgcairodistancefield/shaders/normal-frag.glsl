varying vec2 sampleCoord;

uniform sampler2D Texture;
uniform vec4      Color;
uniform float     AlphaMin;
uniform float     AlphaMax;

void main() {
	gl_FragColor = Color * smoothstep(AlphaMin, AlphaMax, texture2D(Texture, sampleCoord).a);
}

