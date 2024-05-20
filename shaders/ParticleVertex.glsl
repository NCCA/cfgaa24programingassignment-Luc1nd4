#version 410 core
layout (location = 0) in vec3 inVert; // position
layout (location = 1) in vec3 inNorm; // normal
layout (location = 2) in vec2 inUV;   // texture coordinates

uniform mat4 MVP;  // model-view-projection matrix

out vec2 vertUV;   // pass UV to fragment shader

void main() {
    gl_Position = MVP * vec4(inVert, 1.0);
    vertUV = inUV;
}
