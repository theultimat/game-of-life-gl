#version 330 core

out vec4 fsOut_Colour;

uniform sampler2D u_StateTexture;
uniform int u_Height;

void main()
{
    ivec2 pos = ivec2(gl_FragCoord.x, u_Height - gl_FragCoord.y - 1);
    fsOut_Colour = vec4(texelFetch(u_StateTexture, pos, 0).rrr, 1);
}
