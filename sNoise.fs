#version 330 core
out vec4 FragColor;

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

// 2D Random
vec2 random2(vec2 st){
st *= u_time;
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.54531235);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;
    vec3 color = vec3(0.0);

    vec2 pos = vec2(st*4);

    color = vec3( noise(pos)*.5+.5 );

    pos = vec2(st*8);

    color = color + vec3( (noise(pos)*.5+.5)/4 - (0.5/2) );

    pos = vec2(st*16);

    color = color + vec3( (noise(pos)*.5+.5)/8 - (0.5/4));

    pos = vec2(st*32);

    color = color + vec3( (noise(pos)*.5+.5)/16 - (0.5/4));

    pos = vec2(st*64);

    color = color + vec3( (noise(pos)*.5+.5)/32 - (0.5/8));

    pos = vec2(st*128);

    color = color + vec3( (noise(pos)*.5+.5)/64 - (0.5/16));

    vec2 screenCoord = gl_FragCoord.xy/u_resolution.xy;

    screenCoord -= 0.5;

    screenCoord *= 2;

    screenCoord.x = 1 - clamp(pow(screenCoord.x,2),0.0,1.0);

    screenCoord.y = 1 - clamp(pow(screenCoord.y,2),0.0,1.0);

    color *= screenCoord.x * screenCoord.y;
    if(color.x > 2.0)
    {
    }
    else if(color.x < 0.0625)
    {
        color.x = 0;
        color.y = 0;
        color.z = pow(color.z+0.355,2);
    }
    else if(color.x < 0.125)
    {
        color.x = 0;
        color.y = sqrt(color.y);
        color.z = 0;
    }
    else if(color.x < 0.25)
    {
        color.x = sqrt(color.x)/1.2;
        color.y = sqrt(color.y)*1.2;
        color.z = 0;
    }
    else if(color.x < 0.35)
    {
        color.x = sqrt(color.x)/1.2;
        color.y = sqrt(color.y)/1.5;
    }
    else
    {
        color.x = 0.9;
        color.y = 0.9;
        color.z = 0.9;
    }


    gl_FragColor = vec4(color,1.0);
    //* ((1 - (pow(gl_FragCoord.x*2,2.0)) * (1 - (pow(gl_FragCoord.y*2,2.0)))));
}
