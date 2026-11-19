#version 330 core

in vec3 passColor;

uniform vec3 uColor;
uniform bool useVertexColor;

out vec4 FragColor;

void main()
{
    if (useVertexColor)
        FragColor = vec4(passColor, 1.0);
    else
        FragColor = vec4(uColor, 1.0);
}