#include "image.h"

#include <iostream>
#include <future>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"
#include "stb_image_write.h"

/*std::string m_filepath; a filepath, how unobvious
unsigned char* m_data; local data buffer
int m_width, m_height, m_colorChanels; variables of image itself */

image::image(const std::string& filepath) :
	m_filepath(filepath),
	m_width(0), m_height(0),
	m_data(nullptr),
	m_colorChannels(0),
	m_imageId(0)
{
	stbi_set_flip_vertically_on_load(true);
	m_data = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_colorChannels, 0);
	if (m_data == NULL) {
		std::cout << "Fuck off ur image not loading\n";
	}
	else {
		std::cout << "Image " << &m_imageId << " is loaded via constructor withous resizing!\n";
	}
}

image::image(const std::string& filepath, const int& width, const int& height) :
	m_filepath(filepath),
	m_width(0), m_height(0),
	m_data(nullptr),
	m_colorChannels(0),
	m_imageId(0)
{
	stbi_set_flip_vertically_on_load(true);
	m_data = stbi_load(m_filepath.c_str(), &m_width, &m_height, &m_colorChannels, 0);
	if (m_data == NULL) {
		std::cout << "Fuck off ur image not loading\n";
	}
	else {
		std::cout << "Image " << &m_imageId << " is loaded via constructor with resizing!\n";
	}
	resize(width, height);
}

image::image(const image& source) {
	if (this != &source) {
		std::cout << "Copying image " << source.m_imageId << " to " << this->m_imageId << "\n";
		stbi_image_free(m_data);
		m_data = new unsigned char[(source.m_height * source.m_width * source.m_colorChannels)];
		m_filepath = source.m_filepath;
		std::memcpy(m_data, source.m_data, source.m_height * source.m_width * source.m_colorChannels);
		m_height = source.m_height;
		m_width = source.m_width;
		m_colorChannels = source.m_colorChannels;
	}
}

image::image(image&& source) {
	if (this != &source) {
		std::cout << "Moving image " << source.m_imageId << " to " << this->m_imageId << "\n";
		stbi_image_free(m_data);
		m_data = new unsigned char[(source.m_height * source.m_width * source.m_colorChannels)];
		m_filepath = source.m_filepath;
		std::memcpy(m_data, source.m_data, source.m_height * source.m_width * source.m_colorChannels);
		m_height = source.m_height;
		m_width = source.m_width;
		m_colorChannels = source.m_colorChannels;
	}
	source.~image();
}

image::~image() {
	std::cout << "Image " << &m_imageId << " is destroyed.\n";
	if (m_data != NULL) {
		if (m_data != nullptr)	{
			stbi_image_free(m_data);
			std::cout << "Free'd " << m_width * m_height * m_colorChannels << " bytes\n\n";
		}
	}
}

void image::resize(const int& width, const int& height) {
	unsigned char* temp = new unsigned char[width * height * m_colorChannels];
	stbir_resize_uint8(m_data, m_width, m_height, 0, temp, width, height, 0, m_colorChannels);
	m_width = width; 
	m_height = height;
	std::memcpy(m_data, temp, m_width * m_height * m_colorChannels);
	delete[] temp;
}
