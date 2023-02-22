#version 330 core

out vec4 fsOut_Colour;

uniform sampler2D u_StateTexture;
uniform int u_Height;
uniform float u_Zoom;

void main()
{
    vec2 pos = vec2(gl_FragCoord.x, u_Height - gl_FragCoord.y - 1) / u_Zoom;
    fsOut_Colour = vec4(texelFetch(u_StateTexture, ivec2(pos), 0).rrr, 1);
}
