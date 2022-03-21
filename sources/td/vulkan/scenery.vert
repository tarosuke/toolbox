#version 450
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    // debugPrintfEXT("tex: %v2f", texCoord);
    gl_Position = vec4(inPosition, 1.0);
    fragColor = colors[gl_VertexIndex];
}
