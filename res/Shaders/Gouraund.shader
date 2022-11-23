#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;
layout(location = 2) in vec3 a_normal;

out vec2 tex_coord;
out vec3 finalColor;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 normal_model;

uniform vec3 lamp_color;
uniform vec3 light_source_pos;
uniform vec3 camera_pos;

float ambient_strenght = 0.1;
float diffuse_strenght = 1.0;
float spectacular_strenght = 1.0;

void main()
{
	gl_Position = projection * camera * model * vec4(a_position, 1.0);
	tex_coord = a_texture;
	vec3 normal = mat3(normal_model) * a_normal;
	vec3 frag_pos = vec3(model * vec4(a_position, 1.0));

	vec3 ambient = lamp_color * ambient_strenght;

	vec3 normal_norm = normalize(normal);
	vec3 light_direction = normalize(light_source_pos - frag_pos);
	float diffusion = max(dot(light_direction, normal_norm), 0.0) * diffuse_strenght;
	vec3 diffuse_color = diffusion * lamp_color;

	vec3 camera_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_direction, normal_norm);
	float specular = pow(max(dot(camera_dir, reflect_dir), 0.0), 32);
	vec3 specular_color = spectacular_strenght * specular * lamp_color;

	finalColor = ambient + specular_color + diffuse_color;
}

#shader fragment
#version 330 core

in vec3 finalColor;
in vec2 tex_coord;

out vec4 fragColor;

uniform sampler2D obj_texture;

void main()
{
	fragColor = texture(obj_texture, tex_coord) * vec4(finalColor, 1.0);
}