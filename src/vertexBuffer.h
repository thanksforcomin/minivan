#pragma once

class vertexBuffer
{
private:
	unsigned int m_rendererId;
	unsigned int m_size;
public:
	vertexBuffer(const void* data, unsigned int size);
	vertexBuffer(); //костыль
	~vertexBuffer();
    void bindBuffer() const;
	void unbindBuffer() const;
	void init_buffer(const void* data, unsigned int size);
	inline unsigned int getSize() const { return m_size; };
};

