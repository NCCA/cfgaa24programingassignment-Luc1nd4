#version 410 core
uniform sampler2D tex; // Add this line to declare the texture uniform
in vec2 vertUV;
layout (location=0) out vec4 outColour;
void main ()
{
    // set the fragment colour to the current texture
    outColour = texture(tex, vertUV);
}
