#pragma once
class indexBuffer
{
private:
	unsigned int m_rendererId;
	unsigned int m_count;
	void* m_data;
public:
	indexBuffer(const void* data, unsigned int count);
	indexBuffer();
	~indexBuffer();
	void init_buffer(const void* data, unsigned int count);
	void bindBuffer() const;
	void unbindBuffer() const;
	inline unsigned int getCount() const { return m_count; }
};

