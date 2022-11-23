#pragma once

#include "renderer.h"
#include "shader.h"

class uniformBuffer
{
	unsigned int m_rendererId;
	unsigned int m_bindingPoint;
	unsigned char* data;
public:
	uniformBuffer(const void* data, const unsigned int& type);
	~uniformBuffer();

	void bind(unsigned int& index);
	void set_data(unsigned int& byte, unsigned int& size, unsigned char* data);
};

