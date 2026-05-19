// Author @patriciogv - 2015
// Title: Ikeda Data Stream
#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

out vec4 outputColor;

//adapted from code by Patricio Gonzalez Vivo https://thebookofshaders.com/edit.php#10/ikeda-03.frag

float random (in float x) {
    return fract(sin(x)*1e4);
}

float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}

float pattern(vec2 st, vec2 v, float t) {
    vec2 p = floor(st+v);
    return step(t, random(100.+p*.000001)+random(p.x)*0.5 );
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec2 grid = vec2(100.0,50.);
    st *= grid;

    vec2 ipos = floor(st);  // integer
    vec2 fpos = fract(st);  // fraction

    vec2 vel = vec2(u_time*0.800*max(grid.x,grid.y)); // time
    vel *= vec2(-1.,0.0) * random(1.000+ipos.y); // direction

    // Assign a random value base on the integer coord
    vec2 offset = vec2(0.1,0.);
    
	float density = 1.016;
    vec3 color = vec3(0.);
    color.r = pattern(st+offset,vel,density+1.0/u_resolution.x);
    color.g = pattern(st,vel,density+1.0/u_resolution.x);
    color.b = pattern(st-offset,vel,density+1.0/u_resolution.x);

    // Margins
    color *= step(-0.544,fpos.y);


    outputColor = vec4(color,color);
}
