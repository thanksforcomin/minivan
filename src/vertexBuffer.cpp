#include "vertexBuffer.h"
#include "renderer.h"

vertexBuffer::vertexBuffer(const void* data, unsigned int size) : m_size(size), m_rendererId(0)
{
    init_buffer(data, size);
}

vertexBuffer::vertexBuffer() {}

void vertexBuffer::init_buffer(const void* data, unsigned int size) {
    glGenBuffers(1, &m_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

vertexBuffer::~vertexBuffer()
{
    glDeleteBuffers(1, &m_rendererId);
}

void vertexBuffer::bindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void vertexBuffer::unbindBuffer() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}