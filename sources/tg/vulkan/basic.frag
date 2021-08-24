#version 450

// 入力
layout(location = 0) in vec2 texCoord;
layout(binding = 1) uniform sampler2D texSampler;

// 出力色(location=0が鍵？)
layout(location = 0) out vec4 out_color;

void main() {
    out_color = texture(texSampler, texCoord);
}
