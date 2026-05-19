#version 150
#ifdef GL_ES
precision mediump float;
#endif

out vec4 outputColor;

uniform float u_time;
uniform vec2 u_resolution;

uniform float u_p1;

const int AMOUNT = 3;

float map(float value, float min1, float max1, float min2, float max2) {
  return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}
/*CODE ADAPTED FROM LEWIS LEPTON TUTORIAL, orginal code to be found here: 
https://github.com/lewislepton/shadertutorialseries/tree/master/014_waterColor */
void main(){
	
	// normalized pixel coordinates (from 0 to 1)
    vec2 coord = (gl_FragCoord.xy/u_resolution);

    for (int n = 1; n < AMOUNT; n++){
    	float i = float(n);
        coord += vec2(1.0 / i * sin(i * coord.y + (u_time) + (i/2.0) * (i*3.624)) + 
		0.8, 3.0 * (i * 2.0) /i * sin(coord.x + (u_time*2.2) + 3.908 * (i*4.576)) + 1.6);
    }    
    vec3 color = vec3(u_p1 * sin(coord.x) + 0.5, u_p1 * sin(coord.y) + 0.5, u_p1 * sin(coord.x + coord.y));
	
	// output to screen
    outputColor = vec4(color,u_p1);
}
