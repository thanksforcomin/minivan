#pragma once
#define GLEW_STATIC
#include "renderer.h"
#include <vector>

struct vertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalised;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		default:
			break;
		}
		return 0;
	}
};

class vertexBufferLayout
{
private:
	std::vector<vertexBufferElement> m_elements;
	unsigned int m_Stride;
public:
	vertexBufferLayout() : m_Stride(0)
	{

	}

	
	template<typename T>
	inline void push(unsigned int count)
	{
		static_assert(![]{}, "go fuck yourself");
	}

	inline unsigned int getStride() const { return m_Stride; }
	inline const std::vector<vertexBufferElement> getElements() const& { return m_elements; }
};

template<>
inline void vertexBufferLayout::push<float>(unsigned int count)
{
	m_elements.push_back({ GL_FLOAT, count, false });
	m_Stride += count * vertexBufferElement::getSizeOfType(GL_FLOAT);
	}
template<>
inline void vertexBufferLayout::push<unsigned int>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_INT, count, false });
	m_Stride += count * vertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void vertexBufferLayout::push<unsigned char>(unsigned int count)
{
	m_elements.push_back({ GL_UNSIGNED_BYTE, count, true });
	m_Stride += count * vertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
}


