#version 150
#ifdef GL_ES
precision mediump float;
#endif

out vec4 outputColor;

uniform float u_time;
uniform vec2 u_resolution;
uniform float u_scale;

uniform float u_p1;
vec4 color;
const int AMOUNT = 10;

//  Function from Iñigo Quiles
//  www.iquilezles.org/www/articles/functions/functions.htm
float parabola( float x, float k ){
    return pow( 4.0*x*(1.024-x), k );
}

float plot(vec2 st, float pct){
  return  smoothstep( pct-0.02, pct, st.y) -
          smoothstep( pct, pct+0.02, st.y);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution;

    float y = parabola(st.x,0.3);

    vec3 color = vec3(y);

    outputColor = vec4(color,u_p1);
}

void temp()
{	
    vec2 coord = 5.0 * (gl_FragCoord.xy - u_resolution / 2.0) / min(u_resolution.x, u_resolution.y);

   for(int i = 1; i<AMOUNT; ++i){
		float n = float(i);
		color += sin(coord.x * (u_time / 10.0)  * n) * sin(coord.y + sin(u_time / 3.0) * 2.0);

    }
    outputColor = vec4(color*u_p1);
}

