#include "vertexArray.h"
#include "renderer.h"

vertexArray::vertexArray()
{
	glGenVertexArrays(1, &m_rendererId);
}

vertexArray::~vertexArray()
{
	glDeleteVertexArrays(1, &m_rendererId);
}

void vertexArray::addBuffer(const vertexBuffer& vb, const vertexBufferLayout& layout)
{
	bindArray();
	vb.bindBuffer();
	const auto& elements = layout.getElements();
	unsigned offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset);
		offset += element.count * vertexBufferElement::getSizeOfType(element.type);
	}
	m_count = vb.getSize() / layout.getStride();
}

void vertexArray::bindArray() const
{
	glBindVertexArray(m_rendererId);
}

void vertexArray::unbindArray() const
{
	glBindVertexArray(0);
}