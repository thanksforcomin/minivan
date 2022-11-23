/*#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

#include "texture.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "indexBuffer.h"
#include "vertexBufferLayout.h"

struct vertex {
	glm::vec3 verticles;
	glm::vec3 normals;
	glm::vec2 tex_coords;
};

class Mesh {
public:
	std::vector<texture> m_textures;
	std::vector<vertex> m_vertexes;
	std::vector<uint32_t> m_indicies;

	Mesh(std::vector<vertex>& vertexes,
		std::vector<uint32_t>& indicies, 
		std::vector<texture>& textures);
	Mesh();
	~Mesh();
	void setup_mesh();
	void setup_mesh(std::vector<vertex>& vertexes,
		std::vector<uint32_t>& indicies,
		std::vector<texture>& textures);
	void draw(shader& sh);
	void configure(shader& sh);
private:
	vertexBuffer VBO;
	vertexArray VAO;
	vertexBufferLayout VBOL;
	indexBuffer IBO;
};*/
