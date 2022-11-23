#shader vertex
#version 430 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture;
layout(location = 2) in vec3 a_normal;

out vec2 tex_coord;
out vec3 normal;
out vec3 frag_pos;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;
uniform mat4 normal_model;

void main()
{
	gl_Position = projection * camera * model * vec4(a_position, 1.0);
	tex_coord = a_texture;
	normal = mat3(normal_model) * a_normal;
	frag_pos = vec3(model * vec4(a_position, 1.0));
}

#shader fragment
#version 430 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shiny;
};

struct Lighting {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float lin;
	float quad;
};

struct dirLighting {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct pointLighting {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float lin;
	float quad;
};

struct spotlightLighting {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float lin;
	float quad;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;
out vec4 fragColor;

uniform vec3 camera_pos;

uniform Material material;
uniform Lighting lighting;

//uniform float time;

float ambient_strenght = 0.3;
float diffuse_strenght = 1.1;
float spectacular_strenght = 1.2;

void main()
{
	vec3 specular_tx = vec3(texture(material.specular, tex_coord));
	vec3 light_direction = lighting.position - frag_pos;
	float light_dist = length(light_direction);
	float light_strenght = 1.0 / (lighting.constant + lighting.lin * light_dist + lighting.quad * (light_dist * light_dist));

	vec3 ambient = lighting.ambient * vec3(texture(material.diffuse, tex_coord)) * ambient_strenght * light_strenght;

	vec3 emission_color = vec3(0.0);
	/*if (specular_tx.r == 0.0) {
		emission_color = vec3(texture(material.emission, tex_coord + vec2(0.0, time)));
	}*/

	float theta = dot(normalize(light_direction), normalize(-lighting.direction));
	float epsilon = lighting.cutOff - lighting.outerCutOff;
	float intensity = clamp((theta - lighting.outerCutOff)/epsilon, 0.0, 1.0);

	if (theta > lighting.outerCutOff) {
		vec3 normal_norm = normalize(normal);
		vec3 light_direction_norm = normalize(light_direction);
		float diffusion = max(dot(light_direction_norm, normal_norm), 0.0) * (diffuse_strenght * light_strenght);
		vec3 diffuse_color = (diffusion * vec3(texture(material.diffuse, tex_coord))) * lighting.diffuse;
		diffuse_color *= intensity;

		vec3 camera_dir = normalize(camera_pos - frag_pos);
		vec3 reflect_dir = reflect(-light_direction_norm, normal_norm);
		float specular = pow(max(dot(camera_dir, reflect_dir), 0.0), material.shiny);
		vec3 specular_color = (spectacular_strenght * light_strenght * specular * specular_tx) * lighting.specular;
		specular_color *= intensity;

		fragColor = vec4(ambient + diffuse_color + specular_color + emission_color, 1.0);
	}
	else {
		fragColor = vec4(ambient + emission_color, 1.0);
	}
}

vec3 calculateDirLighting(dirLighting lighting) {
	vec3 specular_tx = vec3(texture(material.specular, tex_coord));
	vec3 light_direction = lighting.direction;
	float light_dist = length(light_direction);

	vec3 ambient = lighting.ambient * vec3(texture(material.diffuse, tex_coord)) * ambient_strenght;

	vec3 emission_color = vec3(0.0);
	/*if (specular_tx.r == 0.0) {
		emission_color = vec3(texture(material.emission, tex_coord + vec2(0.0, time)));
	}*/

	vec3 normal_norm = normalize(normal);
	vec3 light_direction_norm = normalize(light_direction);
	float diffusion = max(dot(light_direction_norm, normal_norm), 0.0) * (diffuse_strenght);
	vec3 diffuse_color = (diffusion * vec3(texture(material.diffuse, tex_coord))) * lighting.diffuse;

	vec3 camera_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_direction_norm, normal_norm);
	float specular = pow(max(dot(camera_dir, reflect_dir), 0.0), material.shiny);
	vec3 specular_color = (spectacular_strenght * specular * specular_tx) * lighting.specular;

	return ambient + diffuse_color + specular_color + emission_color;
}

vec3 calculatePointLight(pointLighting lighting) {
	vec3 specular_tx = vec3(texture(material.specular, tex_coord));
	vec3 light_direction = lighting.position - frag_pos;
	float light_dist = length(light_direction);
	float light_strenght = 1.0 / (lighting.constant + lighting.lin * light_dist + lighting.quad * (light_dist * light_dist));

	vec3 ambient = lighting.ambient * vec3(texture(material.diffuse, tex_coord)) * ambient_strenght * light_strenght;

	vec3 emission_color = vec3(0.0);
	/*if (specular_tx.r == 0.0) {
		emission_color = vec3(texture(material.emission, tex_coord + vec2(0.0, time)));
	}*/

	vec3 normal_norm = normalize(normal);
	vec3 light_direction_norm = normalize(light_direction);
	float diffusion = max(dot(light_direction_norm, normal_norm), 0.0) * (diffuse_strenght * light_strenght);
	vec3 diffuse_color = (diffusion * vec3(texture(material.diffuse, tex_coord))) * lighting.diffuse;

	vec3 camera_dir = normalize(camera_pos - frag_pos);
	vec3 reflect_dir = reflect(-light_direction_norm, normal_norm);
	float specular = pow(max(dot(camera_dir, reflect_dir), 0.0), material.shiny);
	vec3 specular_color = (spectacular_strenght * light_strenght * specular * specular_tx) * lighting.specular;

	return ambient + diffuse_color + specular_color + emission_color;
}

vec3 calculateSpotlightLighting(spotlightLighting lighting) {
	vec3 specular_tx = vec3(texture(material.specular, tex_coord));
	vec3 light_direction = lighting.position - frag_pos;
	float light_dist = length(light_direction);
	float light_strenght = 1.0 / (lighting.constant + lighting.lin * light_dist + lighting.quad * (light_dist * light_dist));

	vec3 ambient = lighting.ambient * vec3(texture(material.diffuse, tex_coord)) * ambient_strenght * light_strenght;

	vec3 emission_color = vec3(0.0);
	/*if (specular_tx.r == 0.0) {
		emission_color = vec3(texture(material.emission, tex_coord + vec2(0.0, time)));
	}*/

	float theta = dot(normalize(light_direction), normalize(-lighting.direction));
	float epsilon = lighting.cutOff - lighting.outerCutOff;
	float intensity = clamp((theta - lighting.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > lighting.cutOff) {
		vec3 normal_norm = normalize(normal);
		vec3 light_direction_norm = normalize(light_direction);
		float diffusion = max(dot(light_direction_norm, normal_norm), 0.0) * (diffuse_strenght * light_strenght);
		vec3 diffuse_color = (diffusion * vec3(texture(material.diffuse, tex_coord))) * lighting.diffuse * intensity;

		vec3 camera_dir = normalize(camera_pos - frag_pos);
		vec3 reflect_dir = reflect(-light_direction_norm, normal_norm);
		float specular = pow(max(dot(camera_dir, reflect_dir), 0.0), material.shiny);
		vec3 specular_color = (spectacular_strenght * light_strenght * specular * specular_tx) * lighting.specular * intensity;

		 return ambient + diffuse_color + specular_color + emission_color;
	}
	else {
		return ambient + emission_color;
	}
}
