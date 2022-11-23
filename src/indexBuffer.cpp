#include "indexBuffer.h"
#include "renderer.h"

indexBuffer::indexBuffer(const void* data, unsigned int count) : m_count(count)
{
    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(GLuint), data, GL_STATIC_DRAW);
}

indexBuffer::indexBuffer() {}

indexBuffer::~indexBuffer()
{
    glDeleteBuffers(1, &m_rendererId);
}

void indexBuffer::init_buffer(const void* data, unsigned int count) {
    m_count = count;
    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

void indexBuffer::bindBuffer() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void indexBuffer::unbindBuffer() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}