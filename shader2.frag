#version 150
#ifdef GL_ES
precision mediump float;
#endif

out vec4 outputColor;

uniform float u_time;
uniform vec2 u_resolution;

uniform float u_p1;

const int AMOUNT = 4;
float len = 0;

/*CODE ADAPTED FROM LEWIS LEPTON TUTORIAL, orginal code to be found here: 
https://github.com/lewislepton/shadertutorialseries/tree/master/015_waterColor02 */

void main(){

    // Normalized pixel coordinates (from 0 to 1)
    vec2 coord = (gl_FragCoord.xy - u_resolution / 2.0) / min(u_resolution.x, u_resolution.y);
    float len;
    for (int i = 0; i < AMOUNT; i++){
        len = length(vec2(coord.x, coord.y));
        for (int i = 0; i < 2; i++)
        {
        coord.x = coord.x + sin(coord.y + cos(len*1.0)*2.0) + sin(u_time / 13.0);
        coord.y = coord.y - cos(coord.x + sin(len*1.0)) + cos(u_time / 7.0);
        }
    }

    // Output to screen
    outputColor = vec4(cos(len*1.25), cos(len*0.5), cos(len*1.0), u_p1);
}