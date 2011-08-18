varying vec2 sampleCoord;

void main() {
     sampleCoord = gl_MultiTexCoord0.xy;
     gl_Position = ftransform();
}

