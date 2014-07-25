varying vec2 sampleCoord;
varying vec2 shiftedSampleCoord;

uniform sampler2D Texture;
uniform vec4      Color;
uniform vec4      StyleColor;
uniform float     AlphaMin;
uniform float     AlphaMax;

void main() {
	float a = smoothstep(AlphaMin, AlphaMax, texture2D(Texture, sampleCoord).a);

	vec4 shifted = StyleColor * smoothstep(
		AlphaMin,
		AlphaMax,
		texture2D(Texture, shiftedSampleCoord).a
	);

	gl_FragColor = mix(shifted, Color, a);
}

