#version 330 core

out vec4 fsOut_Colour;

uniform sampler2D u_PreviousState;
uniform ivec2 u_StateSize;

void main()
{
    ivec2 pos = ivec2(gl_FragCoord.xy);

    // Sample the eight neighbours from the previous state.
    ivec2 offsets[8] = ivec2[](
        ivec2(-1, -1),
        ivec2(-1,  0),
        ivec2(-1,  1),
        ivec2( 0, -1),
        ivec2( 0,  1),
        ivec2( 1, -1),
        ivec2( 1,  0),
        ivec2( 1,  1)
    );

    int total = 0;
    for (int i = 0; i < 8; ++i)
    {
        ivec2 coords = (pos + offsets[i] + u_StateSize) % u_StateSize;
        float cell = texelFetch(u_PreviousState, coords, 0).r;
        total += int(cell);
    }

    // Apply the rules - live cells with two or three neighbours stay alive and
    // dead cells with three live neighbours becomes alive.
    float prev_cell = texelFetch(u_PreviousState, pos, 0).r;
    float cell = prev_cell;

    if (cell > 0.0f && (total < 2 || total > 3))
        cell = 0.0f;
    else if (cell < 1.0f && total == 3)
        cell = 1.0f;

    fsOut_Colour = vec4(cell, 0, 0, 1);
}
