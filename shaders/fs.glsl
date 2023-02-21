#version 330 core

out vec4 fsOut_Colour;

uniform sampler2D u_StateTexture;

void main()
{
    fsOut_Colour = vec4(texelFetch(u_StateTexture, ivec2(gl_FragCoord.xy), 0).rrr, 1);
}
