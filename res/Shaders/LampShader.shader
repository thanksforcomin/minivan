#shader vertex
#version 330 core

layout(location = 0) in vec3 a_position;

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

void main()
{
	gl_Position = projection * camera * model * vec4(a_position, 1.0);
}

#shader fragment
#version 330 core

out vec4 fragColor;

uniform vec3 lamp_color;

void main()
{
	fragColor = vec4(lamp_color, 1.0);
}