#version 450
layout(location = 0) out vec4 outColor;

layout(location = 0) in vec4 cellColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    vec2 agent;
} ubo;

layout(push_constant) uniform Push {
    vec4 color;
    float size;
} push;

void main() {
    outColor = cellColor;
}