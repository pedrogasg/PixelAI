#version 450

layout(location = 0) in vec4 position;

layout(location = 0) out vec4 cellColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    vec2 agent;
} ubo;

layout(push_constant) uniform Push {
    vec4 color;
    float size;
} push;

void main() {
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    gl_PointSize = 1.0;
    if(position.z == ubo.agent.x && position.w == ubo.agent.y) {
        cellColor = vec4(0.5f, 0.3f, 0.2f, 0.5f);
    } else {
        cellColor =  push.color;
    }
}