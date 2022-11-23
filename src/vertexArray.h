#pragma once
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
class vertexArray
{
private:
	unsigned int m_rendererId;
	unsigned int m_count;
public:
	vertexArray();
	~vertexArray();

	void addBuffer(const vertexBuffer& vb, const vertexBufferLayout& layout);

	void bindArray() const;
	void unbindArray() const;

	inline unsigned int getCount() const { return m_count; }
};

