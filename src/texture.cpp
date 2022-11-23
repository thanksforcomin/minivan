//#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h"
#include "renderer.h"

#include <iostream>
#include <future>

texture::texture(const std::string& filepath, std::string a_type, std::string a_name) :
	m_rendererId(0),
	m_img(filepath),
	type(a_type)
{
	glGenTextures(1, &m_rendererId);
	glBindTexture(GL_TEXTURE_2D, m_rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	std::cout << "Image received (maybe)! Height: " << m_img.height() << " Width: " << m_img.width() << "\n";

	if (m_img.isLoaded()) {
		if (m_img.channels() == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_img.width(), m_img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)m_img.getData());
		}
		else if (m_img.channels() == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_img.width(), m_img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, (void*)m_img.getData());
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << filepath << " succesfully loaded" << std::endl;
	} 
	else {
		std::cout << "Warning! Something went wrong, texture " << filepath << " not loaded" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

texture::texture(const image& img, std::string a_type, std::string a_name) :
	m_rendererId(0),
	m_img(img),
	type(a_type)
{
	glGenTextures(1, &m_rendererId);
	glBindTexture(GL_TEXTURE_2D, m_rendererId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	std::cout << "Image received (maybe)! Height: " << m_img.height() << " Width: " << m_img.width() << "\n";

	if (m_img.isLoaded()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_img.width(), m_img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)m_img.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture " << m_img.filepath() << " succesfully loaded" << std::endl;
	}
	else {
		std::cout << "Warning! Something went wrong, texture " << m_img.filepath() << " not loaded" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

texture::~texture()
{
	std::cout << "Texture " << m_img.filepath() << " was destroyed. Bye bye, texture!\n";
	glDeleteTextures(1, &m_rendererId);
 	m_img.~image();
}

void texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_rendererId);
}

void texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
