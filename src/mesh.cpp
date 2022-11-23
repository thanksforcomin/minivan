/*#include "mesh.h"

Mesh::Mesh(std::vector<vertex>& vertexes,
	std::vector<uint32_t>& indicies,
	std::vector<texture>& textures) : VAO(), VBOL()
{
	m_indicies = indicies;
	m_textures = textures;
	m_vertexes = vertexes;

	setup_mesh();
}

Mesh::Mesh() : VAO(), VBOL() {}

Mesh::~Mesh() {
	m_indicies.clear();
	m_indicies.shrink_to_fit();
	m_textures.clear();
	m_textures.shrink_to_fit();
	m_vertexes.clear();
	m_vertexes.shrink_to_fit();
}

void Mesh::setup_mesh(std::vector<vertex>& vertexes,
	std::vector<uint32_t>& indicies,
	std::vector<texture>& textures) 
{
	m_indicies = indicies;
	m_textures = textures;
	m_vertexes = vertexes;

	setup_mesh();
}

void Mesh::setup_mesh() {
	VBO.init_buffer(&m_vertexes, sizeof(m_vertexes) * m_vertexes.size());
	VBOL.push<float>(3);
	VBOL.push<float>(3);
	VBOL.push<float>(2);
	VAO.addBuffer(VBO, VBOL);
	IBO.init_buffer(&m_indicies, m_indicies.size());
}

void Mesh::draw(shader& sh) {
	sh.bind();
	VAO.bindArray();
	glDrawArrays(GL_TRIANGLES, 0, m_vertexes.size());
}

void Mesh::configure(shader& sh) {
	for (uint8_t i = 0; i < m_textures.size(); i++) {
		m_textures[i].bind(i);
		sh.setUniform1i(m_textures[i].name, i);
	}
}*/
