#version 450

// 頂点情報入力
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 coord;

layout(binding = 0) uniform block{
	mat4 model;
	mat4 view_projection;
}world;

// テクスチャ座標出力
layout(location = 0) out vec2 texCoord;

// 変換
void main() {
	texCoord = coord;
	vec3 world_pos = vec3(world.model * vec4(position, 1.0));
	gl_Position = world.view_projection * vec4(world_pos, 1.0);
}
