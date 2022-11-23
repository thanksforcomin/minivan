#pragma once

#include <string>

#include "image.h"
#include "defines.h"

class texture
{
private:
	unsigned int m_rendererId;
	image m_img;
public:
	texture(const std::string& filepath, std::string a_type = "universal", std::string a_name = "\n");
	texture(const image& img, std::string a_type = "universal", std::string a_name = "\n");
	~texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	inline int getWidth() { return m_img.width(); }
	inline int getHeight() { return m_img.height(); }
	std::string type;
	std::string name;
};

