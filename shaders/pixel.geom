#version 450

layout(triangle_strip, max_vertices = 4) out;

layout(points) in;

layout(location = 0) in vec4 cellColors [];
layout(location = 0) out vec4 cellColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    vec2 agent;
} ubo;

layout(push_constant) uniform Push {
    vec4 color;
    float size;
} push;

void main() {
    vec4 pos = gl_in[0].gl_Position;

    cellColor = cellColors[0];

    gl_Position = pos;
    EmitVertex();

    gl_Position = pos;
    gl_Position.y += push.size;
    gl_Position = gl_Position;
    EmitVertex();

    gl_Position = pos;
    gl_Position.x += push.size;
    gl_Position = gl_Position;
    EmitVertex();

    gl_Position = pos;
    gl_Position.x += push.size;
    gl_Position.y += push.size;
    gl_Position = gl_Position;
    EmitVertex();

    EndPrimitive();
}