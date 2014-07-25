varying vec2 sampleCoord;
varying vec2 shiftedSampleCoord;

void main() {
	sampleCoord        = gl_MultiTexCoord0.xy;
	shiftedSampleCoord = gl_MultiTexCoord0.xy - vec2(0.04, -0.02);
	gl_Position        = ftransform();
}

